/********************************************************************************
 *                                                                              *
 * Curly Engine                                                                 *
 * Copyright (c) 2021-2024 Adrian Bedregal                                      *
 *                                                                              *
 * This software is provided 'as-is', without any express or implied            *
 * warranty. In no event will the authors be held liable for any damages        *
 * arising from the use of this software.                                       *
 *                                                                              *
 * Permission is granted to anyone to use this software for any purpose,        *
 * including commercial applications, and to alter it and redistribute it       *
 * freely, subject to the following restrictions:                               *
 *                                                                              *
 * 1. The origin of this software must not be misrepresented; you must not      *
 *    claim that you wrote the original software. If you use this software      *
 *    in a product, an acknowledgment in the product documentation would be     *
 *    appreciated but is not required.                                          *
 * 2. Altered source versions must be plainly marked as such, and must not be   *
 *    misrepresented as being the original software.                            *
 * 3. This notice may not be removed or altered from any source distribution.   *
 *                                                                              *
 ********************************************************************************/

#include "editorWindowManagerPlatform.hpp"

#include <window/inputBindings.hpp>

#include <iostream>

namespace wnd
{
cfg::uint32 EditorWindowManager::s_activeSessions  {0u};
cfg::uint32 EditorWindowManager::s_wmInstanceCount {0u};
sys::LazyPtr<EditorWindowManager> EditorWindowManager::s_wmInstances[MAX_WINDOW_INSTANCES] {};

sys::SafePtr<Map<HWND, cfg::uint32>> EditorWindowManager::s_hwndMap {};

WNDCLASSEXA EditorWindowManager::s_appWndClass {};
const char* EditorWindowManager::s_appWndClassName {"CurlyApp"};

PIXELFORMATDESCRIPTOR EditorWindowManager::s_pfd {};
const int EditorWindowManager::s_attribs[ATTRIB_LIST_SIZE]
{
    WGL_DRAW_TO_WINDOW_ARB  , GL_TRUE,
    WGL_SUPPORT_OPENGL_ARB  , GL_TRUE,
    WGL_DOUBLE_BUFFER_ARB   , GL_TRUE,
    WGL_ACCELERATION_ARB    , WGL_FULL_ACCELERATION_ARB,
    WGL_PIXEL_TYPE_ARB      , WGL_TYPE_RGBA_ARB,
    WGL_COLOR_BITS_ARB      , 32,
    WGL_DEPTH_BITS_ARB      , 24,
    WGL_STENCIL_BITS_ARB    , 8,
    0
};

int EditorWindowManager::s_mouseTrackCount {0};
int EditorWindowManager::s_keyPhysicStates[NUM_KEYS_SIZE] {};

MSG EditorWindowManager::s_msg {};
HMODULE EditorWindowManager::s_ogl32Module {nullptr};
HINSTANCE EditorWindowManager::s_procInstanceHandle {nullptr};

bool EditorWindowManager::s_vSyncCompat {true};
bool EditorWindowManager::s_attribCtxCompat {true};
bool EditorWindowManager::s_pixelFormatCompat {true};

PFNWGLCHOOSEPIXELFORMATARBPROC EditorWindowManager::wglChoosePixelFormatARB {nullptr};
PFNWGLGETEXTENSIONSSTRINGARBPROC EditorWindowManager::wglGetExtensionsStringARB {nullptr};
PFNWGLCREATECONTEXTATTRIBSARBPROC EditorWindowManager::wglCreateContextAttribsARB {nullptr};

PFNWGLSWAPINTERVALEXTPROC EditorWindowManager::wglSwapIntervalEXT {nullptr};
PFNWGLGETSWAPINTERVALEXTPROC EditorWindowManager::wglGetSwapIntervalEXT {nullptr};

EditorWindowManager* EditorWindowManager::createInstance()
{
    if(!s_wmInstanceCount)
    {
#if defined(C__CURLY_FREE_CONSOLE)
        FreeConsole();
#endif
        s_procInstanceHandle = GetModuleHandleW(nullptr);
        registerAppWndClass();
        loadGLExtensions();

        s_wmInstances[0u].init(0u);
        ++s_wmInstanceCount;

        return s_wmInstances[0u];
    }

    if(s_wmInstanceCount >= MAX_WINDOW_INSTANCES)
    {
        return nullptr;
    }

    s_wmInstances[s_wmInstanceCount].init(s_wmInstanceCount);
    return s_wmInstances[s_wmInstanceCount++];
}

EditorWindowManager* EditorWindowManager::getInstance(const cfg::uint32 index)
{
    if(index > 0 && index < (MAX_WINDOW_INSTANCES - 1))
    {
        if(s_wmInstances[index] != nullptr)
        {
            return s_wmInstances[index];
        }
        else
        {
            return nullptr;
        }
    }
    return nullptr;
}

bool EditorWindowManager::isActive()
{
    return m_active;
}

WindowRectParams EditorWindowManager::createRenderingWindow(const char* title, int x, int y, int width, int height, WindowStyle style)
{
    if(!m_active)
    {
        DWORD windowStyle = WS_VISIBLE;
        switch(style)
        {
            case WindowStyle::WINDOWED_STYLE:
                {
                    windowStyle |= WS_OVERLAPPEDWINDOW;
                }
                break;
            case WindowStyle::BORDERLESS_STYLE:
                {
                    windowStyle |= WS_POPUP | WS_BORDER;
                    x = y = 0; // Disable positioning since its borderless
                }
                break;
            case WindowStyle::BORDERLESS_FULLSCREEN_STYLE:
                {
                    windowStyle |= WS_POPUP | WS_BORDER;
                    width  = GetSystemMetrics(SM_CXSCREEN); // Fix width  to fullscreen
                    height = GetSystemMetrics(SM_CYSCREEN); // Fix height to fullscreen
                    x = y = 0; // Disable positioning since its borderless
                }
                break;
            default:
                break;
        };

        LPCSTR className = s_appWndClass.lpszClassName;
        m_windowHandle = CreateWindowExA
        (
            0L,           // Extended Window Style
            className,    // Window Class Name
            title,        // Window Title
            windowStyle,  // Window Style

            x, y, width, height,

            nullptr,              // Parent Window Handle
            nullptr,              // Menu Handle
            s_procInstanceHandle, // Handle to current instance
            nullptr               // Additional Application Data
        );
        m_active = true;
        ++s_activeSessions;
        (*s_hwndMap)[m_windowHandle] = m_index;
    }

    RECT clientRect{};
    RECT windowRect{};
    WindowRectParams rectParams{};
    GetClientRect(m_windowHandle, &clientRect);
    GetWindowRect(m_windowHandle, &windowRect);
    rectParams.clientWidth = clientRect.right - clientRect.left;
    rectParams.clientHeight = clientRect.bottom - clientRect.top;
    rectParams.windowWidth = windowRect.right - windowRect.left;
    rectParams.windowHeight = windowRect.bottom - windowRect.top;
    return rectParams;
}

void EditorWindowManager::destroyWindow()
{
    if(m_active)
    {
        DestroyWindow(m_windowHandle);
        m_active = false;
    }
}

void EditorWindowManager::setEventCallbackFunction(IWindow* t_windowCallbackInstance, EventCallbackFunction tf_eventCallbackFunction)
{
    m_windowCallbackInstance = t_windowCallbackInstance;
    mf_eventCallbackFunction = tf_eventCallbackFunction;
}

void EditorWindowManager::pollEvents()
{
    if(PeekMessageW(&s_msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&s_msg);
        DispatchMessageW(&s_msg);
    }
}

void EditorWindowManager::swapBuffers()
{
    if(m_active)
    {
        if(s_vSyncCompat)
        {
            wglSwapIntervalEXT(1);
        }
        wglSwapLayerBuffers(m_deviceContextHandle, WGL_SWAP_MAIN_PLANE);
    }
}

EditorWindowManager::EditorWindowManager(const cfg::uint32 t_index)
    : m_active                   {false},
      m_index                    {t_index},
      m_windowHandle             {nullptr},
      m_deviceContextHandle      {nullptr},
      m_glRenderingContextHandle {nullptr}
{
}

EditorWindowManager::~EditorWindowManager()
{
}

void EditorWindowManager::registerAppWndClass()
{
    s_appWndClass.cbSize        = sizeof(WNDCLASSEXA);
    s_appWndClass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    s_appWndClass.lpfnWndProc   = CurlyProc;
    s_appWndClass.cbClsExtra    = 0;
    s_appWndClass.cbWndExtra    = 0;
    s_appWndClass.hInstance     = s_procInstanceHandle;
    s_appWndClass.hIcon         = nullptr;
    s_appWndClass.hCursor       = LoadCursorA(nullptr, IDC_ARROW);
    s_appWndClass.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
    s_appWndClass.lpszMenuName  = nullptr;
    s_appWndClass.lpszClassName = s_appWndClassName;
    s_appWndClass.hIconSm       = nullptr;

    if(!RegisterClassExA(&s_appWndClass))
    {
        fatalError("Failed to register Rendering Window.");
    }
}

void EditorWindowManager::loadGLExtensions()
{
    WNDCLASSEXA dWindowClass;
    dWindowClass.cbSize         = sizeof(WNDCLASSEXA);
    dWindowClass.style          = 0u;
    dWindowClass.lpfnWndProc    = DefWindowProcA;
    dWindowClass.cbClsExtra     = 0;
    dWindowClass.cbWndExtra     = 0;
    dWindowClass.hInstance      = 0;
    dWindowClass.hIcon          = nullptr;
    dWindowClass.hCursor        = nullptr;
    dWindowClass.hbrBackground  = nullptr;
    dWindowClass.lpszMenuName   = nullptr;
    dWindowClass.lpszClassName  = "DWC";
    dWindowClass.hIconSm        = nullptr;

    if(!RegisterClassExA(&dWindowClass))
    {
        fatalError("Failed to register dummy OpenGL window.");
    }

    HWND dWindow = CreateWindowExA
    (
        0L,                         // Extended Window Style
        dWindowClass.lpszClassName, // Window Class Name
        "",                         // Window Title
        0,                          // Window Style

        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        nullptr,     // Parent Window Handle
        nullptr,     // Menu Handle
        nullptr,     // Handle to current instance
        nullptr      // Additional Application Data
    );

    if(!dWindow)
    {
        fatalError("Failed to create dummy OpenGL window.");
    }

    HDC ddc = GetDC(dWindow);

    PIXELFORMATDESCRIPTOR dpfd;
    dpfd.nSize           = sizeof(PIXELFORMATDESCRIPTOR);
    dpfd.nVersion        = 1;
    dpfd.dwFlags         = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    dpfd.iPixelType      = PFD_TYPE_RGBA;
    dpfd.cColorBits      = 32;
    dpfd.cRedBits        = 0;
    dpfd.cRedShift       = 0;
    dpfd.cBlueBits       = 0;
    dpfd.cBlueShift      = 0;
    dpfd.cGreenBits      = 0;
    dpfd.cGreenShift     = 0;
    dpfd.cAlphaBits      = 0;
    dpfd.cAlphaShift     = 0;
    dpfd.cAccumBits      = 0;
    dpfd.cAccumRedBits   = 0;
    dpfd.cAccumBlueBits  = 0;
    dpfd.cAccumGreenBits = 0;
    dpfd.cAccumAlphaBits = 0;
    dpfd.cDepthBits      = 24;
    dpfd.cStencilBits    = 8;
    dpfd.cAuxBuffers     = 0;
    dpfd.iLayerType      = PFD_MAIN_PLANE;
    dpfd.bReserved       = 0;
    dpfd.dwLayerMask     = 0;
    dpfd.dwVisibleMask   = 0;
    dpfd.dwDamageMask    = 0;

    int dPixelformat = ChoosePixelFormat(ddc, &dpfd);
    if(!dPixelformat)
    {
        fatalError("Failed to find a suitable pixel format.");
    }
    if(!SetPixelFormat(ddc, dPixelformat, &dpfd))
    {
        fatalError("Failed to set the pixel format.");
    }

    HGLRC dContext = wglCreateContext(ddc);
    if(!dContext)
    {
        fatalError("Failed to create a dummy OpenGL rendering context.");
    }

    if(!wglMakeCurrent(ddc, dContext))
    {
        fatalError("Failed to activate dummy OpenGL rendering context.");
    }

    wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
    const char* wglExtensions = wglGetExtensionsStringARB(ddc);

    if(!isExtensionSupported(wglExtensions, "WGL_ARB_create_context"))
    {
        s_attribCtxCompat = false;
        warning("WGL_ARB_create_context not supported.");
    }
    else
    {
        wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    }

    if(!isExtensionSupported(wglExtensions, "WGL_ARB_pixel_format"))
    {
        s_pfd = dpfd;
        s_pixelFormatCompat = false;
        warning("WGL_ARB_pixel_format not supported.");
    }
    else
    {
        wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
    }

    if(!isExtensionSupported(wglExtensions, "WGL_EXT_swap_control"))
    {
        s_vSyncCompat = false;
        warning("WGL_EXT_swap_control not supported (V-Sync not supported).");
    }
    else
    {
        wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
        wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
    }

    wglMakeCurrent(ddc, nullptr);
    wglDeleteContext(dContext);
    ReleaseDC(dWindow, ddc);
    DestroyWindow(dWindow);
}

void EditorWindowManager::warning(const char* msg)
{
    MessageBoxA(nullptr, msg, "Warning", MB_OK | MB_ICONWARNING);
}

void EditorWindowManager::fatalError(const char* msg)
{
    MessageBoxA(nullptr, msg, "Fatal Error", MB_OK | MB_ICONERROR);
    exit(EXIT_FAILURE);
}

bool EditorWindowManager::isInvalidFuncAddress(void* funcAddress)
{
    return  (funcAddress == 0) ||
            (funcAddress == (void*)0x1) ||
            (funcAddress == (void*)0x2) ||
            (funcAddress == (void*)0x3) ||
            (funcAddress == (void*)-1);
}

bool EditorWindowManager::isExtensionSupported(const char* extList, const char* extension)
{
	const char* start;
	const char* where;
    const char* terminator;

	where = strchr(extension, ' ');
	if(where || *extension == '\0')
    {
	    return false;
    }

    start = extList;
	while(true)
    {
	    where = strstr(start, extension);

	    if(!where)
        {
	 	    break;
        }

        terminator = where + strlen(extension);

        if(where == start || *(where - 1) == ' ')
        {
            if(*terminator == ' ' || *terminator == '\0' )
            {
                return true;
            }
	    }

	    start = terminator;
	}

	return false;
}

void* EditorWindowManager::CurlyGetProcAddress(const char* name)
{
    void* gpa = (void*)wglGetProcAddress(name);
    if (isInvalidFuncAddress(gpa))
    {
        if (!s_ogl32Module)
        {
            s_ogl32Module = LoadLibraryA("opengl32.dll");
        }
        gpa = (void*)GetProcAddress(s_ogl32Module, name);
    }
    return gpa;
}

LRESULT CALLBACK EditorWindowManager::CurlyProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_CREATE:
            {
                EditorWindowManager* windowInstance = s_wmInstances[(*s_hwndMap)[hWnd]];
                HDC& hdc = windowInstance->m_deviceContextHandle;
                hdc = GetDC(hWnd);

                int pixelFormat;

                if(s_pixelFormatCompat)
                {
                    cfg::uint32 numFormats;
                    wglChoosePixelFormatARB(hdc, s_attribs, nullptr, 1, &pixelFormat, &numFormats);
                    DescribePixelFormat(hdc, pixelFormat, sizeof(s_pfd), &s_pfd);
                }
                else
                {
                    pixelFormat = ChoosePixelFormat(hdc, &s_pfd);
                }

                SetPixelFormat(hdc, pixelFormat, &s_pfd);

                HGLRC& glContext = windowInstance->m_glRenderingContextHandle;

                if(s_attribCtxCompat)
                {
                    int glContextAttribs[] =
                    {
                        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
                        WGL_CONTEXT_MINOR_VERSION_ARB, 6,
                        WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                        0
                    };

                    glContext = wglCreateContextAttribsARB(hdc, 0, glContextAttribs);
                }
                else
                {
                    glContext = wglCreateContext(hdc);
                }
                wglMakeCurrent(hdc, glContext);

                gladLoadGL((GLADloadfunc)CurlyGetProcAddress);

                memset(s_keyPhysicStates, 0, sizeof(s_keyPhysicStates));

                std::cout << "OpenGL " << (char*)glGetString(GL_VERSION);
                std::cout << " Renderer: " << (char*)glGetString(GL_RENDERER) << std::endl;
                std::cout << "GLSL Version: " << (char*)glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
            }
            break;

        case WM_DESTROY:
            {
                EditorWindowManager* windowInstance = s_wmInstances[(*s_hwndMap)[hWnd]];
                --s_activeSessions;
                windowInstance->m_active = false;
                wglMakeCurrent(windowInstance->m_deviceContextHandle, nullptr);
                wglDeleteContext(windowInstance->m_glRenderingContextHandle);
                if(!s_activeSessions)
                {
                    PostQuitMessage(0);
                }
            }
            break;

        case WM_KEYDOWN:
            {
                EditorWindowManager* windowInstance = s_wmInstances[(*s_hwndMap)[hWnd]];
                KeyboardParams params;
                params.code = static_cast<InputCode>(wParam);
                windowInstance->mf_eventCallbackFunction(windowInstance->m_windowCallbackInstance, KEY_PRESSED, &params);
                s_keyPhysicStates[wParam] = 1;
            }
            break;

        case WM_KEYUP:
            {
                s_keyPhysicStates[wParam] = 0;
                EditorWindowManager* windowInstance = s_wmInstances[(*s_hwndMap)[hWnd]];
                KeyboardParams params;
                params.code = static_cast<InputCode>(wParam);
                windowInstance->mf_eventCallbackFunction(windowInstance->m_windowCallbackInstance, KEY_RELEASED, &params);
            }
            break;

        case WM_KILLFOCUS:
            {
                EditorWindowManager* windowInstance = s_wmInstances[(*s_hwndMap)[hWnd]];
                for(cfg::uint32 i = 0; i < NUM_KEYS_SIZE; ++i)
                {
                    if(s_keyPhysicStates[i])
                    {
                        s_keyPhysicStates[i] = 0;
                        KeyboardParams params;
                        params.code = static_cast<InputCode>(i);
                        windowInstance->mf_eventCallbackFunction(windowInstance->m_windowCallbackInstance, KEY_RELEASED, &params);
                    }
                }
                MouseParams lparam;
                MouseParams rparam;
                lparam.code = InputCode::MOUSE_BUTTON_LEFT;
                rparam.code = InputCode::MOUSE_BUTTON_RIGHT;
                windowInstance->mf_eventCallbackFunction(windowInstance->m_windowCallbackInstance, BUTTON_RELEASED, &lparam);
                windowInstance->mf_eventCallbackFunction(windowInstance->m_windowCallbackInstance, BUTTON_RELEASED, &rparam);
            }
            break;

        case WM_LBUTTONDOWN:
            {
                if(!(s_mouseTrackCount++))
                    SetCapture(hWnd);
                EditorWindowManager* windowInstance = s_wmInstances[(*s_hwndMap)[hWnd]];
                MouseParams params;
                params.code = InputCode::MOUSE_BUTTON_LEFT;
                windowInstance->mf_eventCallbackFunction(windowInstance->m_windowCallbackInstance, BUTTON_PRESSED, &params);
            }
            break;

        case WM_LBUTTONUP:
            {
                EditorWindowManager* windowInstance = s_wmInstances[(*s_hwndMap)[hWnd]];
                MouseParams params;
                params.code = InputCode::MOUSE_BUTTON_LEFT;
                windowInstance->mf_eventCallbackFunction(windowInstance->m_windowCallbackInstance, BUTTON_RELEASED, &params);
                if(!(--s_mouseTrackCount))
                    ReleaseCapture();
            }
            break;

        case WM_RBUTTONDOWN:
            {
                if(!(s_mouseTrackCount++))
                    SetCapture(hWnd);
                EditorWindowManager* windowInstance = s_wmInstances[(*s_hwndMap)[hWnd]];
                MouseParams params;
                params.code = InputCode::MOUSE_BUTTON_RIGHT;
                windowInstance->mf_eventCallbackFunction(windowInstance->m_windowCallbackInstance, BUTTON_PRESSED, &params);
            }
            break;

        case WM_RBUTTONUP:
            {
                EditorWindowManager* windowInstance = s_wmInstances[(*s_hwndMap)[hWnd]];
                MouseParams params;
                params.code = InputCode::MOUSE_BUTTON_RIGHT;
                windowInstance->mf_eventCallbackFunction(windowInstance->m_windowCallbackInstance, BUTTON_RELEASED, &params);
                if(!(--s_mouseTrackCount))
                    ReleaseCapture();
            }
            break;

        case WM_MBUTTONDOWN:
            {
                if(!(s_mouseTrackCount++))
                    SetCapture(hWnd);
                EditorWindowManager* windowInstance = s_wmInstances[(*s_hwndMap)[hWnd]];
                MouseParams params;
                params.code = InputCode::MOUSE_BUTTON_MIDDLE;
                windowInstance->mf_eventCallbackFunction(windowInstance->m_windowCallbackInstance, BUTTON_PRESSED, &params);
            }
            break;

        case WM_MBUTTONUP:
            {
                EditorWindowManager* windowInstance = s_wmInstances[(*s_hwndMap)[hWnd]];
                MouseParams params;
                params.code = InputCode::MOUSE_BUTTON_MIDDLE;
                windowInstance->mf_eventCallbackFunction(windowInstance->m_windowCallbackInstance, BUTTON_RELEASED, &params);
                if(!(--s_mouseTrackCount))
                    ReleaseCapture();
            }
            break;

        case WM_XBUTTONDOWN:
            {
                if(!(s_mouseTrackCount++))
                    SetCapture(hWnd);
                EditorWindowManager* windowInstance = s_wmInstances[(*s_hwndMap)[hWnd]];
                MouseParams params;
                params.code = InputCode::MOUSE_BUTTON_04;
                windowInstance->mf_eventCallbackFunction(windowInstance->m_windowCallbackInstance, BUTTON_PRESSED, &params);
            }
            break;

        case WM_XBUTTONUP:
            {
                EditorWindowManager* windowInstance = s_wmInstances[(*s_hwndMap)[hWnd]];
                MouseParams params;
                params.code = InputCode::MOUSE_BUTTON_04;
                windowInstance->mf_eventCallbackFunction(windowInstance->m_windowCallbackInstance, BUTTON_RELEASED, &params);
                if(!(--s_mouseTrackCount))
                    ReleaseCapture();
            }
            break;

        case WM_MOUSEMOVE:
            {
                EditorWindowManager* windowInstance = s_wmInstances[(*s_hwndMap)[hWnd]];
                MouseParams params;
                params.pos.x = GET_X_LPARAM(lParam);
                params.pos.y = GET_Y_LPARAM(lParam);
                windowInstance->mf_eventCallbackFunction(windowInstance->m_windowCallbackInstance, MOUSE_MOVE, &params);
            }
            break;

        case WM_SYSKEYDOWN:
            {
                std::cout << "WM_SYSKEYDOWN" << std::endl;
            }
            break;

        case WM_SYSKEYUP:
            {
                std::cout << "WM_SYSKEYUP" << std::endl;
            }
            break;

        default:
            break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

} // namespace wnd
