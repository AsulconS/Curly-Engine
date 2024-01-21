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

#pragma once

#include <Windows.h>
#include <Windowsx.h>

#include "../../core/GL/gl.h"
#include "../../core/GL/wglext.h"

#include <core/config.hpp>
#include <core/common.hpp>

//#include <system/dstr/map.hpp>
#include <map>
#define Map std::map

#include <window/inputEvents.hpp>
#include <window/customization.hpp>

#include "../safePtr.hpp"
#include "../wmLazyPtr.hpp"
#include "../windowParams.hpp"

#define NUM_KEYS_SIZE 256u
#define ATTRIB_LIST_SIZE 17u
#define MAX_WINDOW_INSTANCES 4u

namespace wnd
{
class IWindow;

using EventCallbackFunction = void (*)(IWindow*, InputEvent, WindowParams*);

class CURLY_API WindowManager final
{
    friend class CURLY_API WMLazyPtr;
public:
    static WindowManager* createInstance();
    static WindowManager* getInstance(const cfg::uint32 index);

    bool isActive();
    WindowRectParams createRenderingWindow(const char* title, int x, int y, int width, int height, WindowStyle style);
    void destroyWindow();

    void setEventCallbackFunction(IWindow* t_windowCallbackInstance, EventCallbackFunction tf_eventCallbackFunction);

    void pollEvents();
    void swapBuffers();

private:
    bool m_active;

    cfg::uint32 m_index;
    HWND m_windowHandle;

    HDC m_deviceContextHandle;
    HGLRC m_glRenderingContextHandle;

    IWindow* m_windowCallbackInstance;
    EventCallbackFunction mf_eventCallbackFunction;

    /* Privated constructor and destructor */

    WindowManager(const cfg::uint32 t_index);
    ~WindowManager();

    /* Static Instances */

    static cfg::uint32 s_activeSessions;
    static cfg::uint32 s_wmInstanceCount;
    static WMLazyPtr s_wmInstances[MAX_WINDOW_INSTANCES];

    /**
     * @brief   Window Hash Table <Window Handler, Instance ID>
     */
    static SafePtr<Map<HWND, cfg::uint32>> s_hwndMap;

    /* Satatic Win32 API Internal Data */

    static WNDCLASSEXA s_appWndClass;
    static const char* s_appWndClassName;

    static PIXELFORMATDESCRIPTOR s_pfd;
    static const int s_attribs[ATTRIB_LIST_SIZE];

    static int s_mouseTrackCount;
    static int s_keyPhysicStates[NUM_KEYS_SIZE];

    static MSG s_msg;
    static HMODULE s_ogl32Module;
    static HINSTANCE s_procInstanceHandle;

    static bool s_vSyncCompat;
    static bool s_attribCtxCompat;
    static bool s_pixelFormatCompat;

    static void registerAppWndClass();
    static void loadGLExtensions();
    static void warning(const char* msg);
    static void fatalError(const char* msg);
    static bool isInvalidFuncAddress(void* funcAddress);

    static PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
    static PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;
    static PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

    static PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
    static PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT;

    static void* CurlyGetProcAddress(const char* name);
    static LRESULT CALLBACK CurlyProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    /* Deleted Constructors and assignment */

    WindowManager(const WindowManager&) = delete;
    WindowManager(WindowManager&&) = delete;

    WindowManager& operator=(const WindowManager&) = delete;
};

} // namespace wnd
