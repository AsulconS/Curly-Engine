/**
 * Curly Engine
 * Copyright (c) 2019-2024 Adrian Bedregal
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#pragma once

#include <engine/core/GL/gl.h>

#include <engine/core/config.hpp>
#include <engine/core/common.hpp>

#include <engine/system/utils/lazyPtr.hpp>
#include <engine/system/utils/safePtr.hpp>

//#include <engine/system/dstr/map.hpp>
#include <map>
#define Map std::map

#include <engine/window/compatUtils.hpp>
#include <engine/window/inputEvents.hpp>
#include "window/windowParams.hpp"
#include <engine/window/inputBindings.hpp>
#include <engine/window/customization.hpp>

#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>

#include <GL/glx.h>
#include <GL/glxext.h>

#define NUM_KEYS_SIZE 256
#define NUM_BUTTONS_SIZE 10

#define ATTRIB_LIST_SIZE 23u
#define MAX_WINDOW_INSTANCES 4u

typedef XID XWND;

namespace wnd
{
class IWindow;

typedef void (*EventCallbackFunction)(IWindow*, InputEvent, WindowParams*);
typedef void (*PFNGLXSWAPINTERVALPROC1)(Display*, GLXDrawable, int);
typedef int  (*PFNGLXSWAPINTERVALPROC2)(int);

class CURLY_API WindowManager final
{
    friend class sys::LazyPtr<class WindowManager>;
    friend void compat::forceGlxContextToVersion(const int major, const int minor);

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
    XWND m_windowHandle;
    Atom m_atomWmDeleteWindow;

    GLXFBConfig m_fbConfig;
    GLXContext m_context;

    XVisualInfo* m_visual;
    XSetWindowAttributes m_windowAttributes;

    IWindow* m_windowCallbackInstance;
    EventCallbackFunction mf_eventCallbackFunction;

    void createContext();

    /* Privated constructor and destructor */

    WindowManager(const cfg::uint32 t_index);
    ~WindowManager();

    /* ---- Static Instances ---- */

    /* Instance Tracking variables */

    static cfg::uint32 s_activeSessions;
    static cfg::uint32 s_wmInstanceCount;
    static sys::LazyPtr<WindowManager> s_wmInstances[MAX_WINDOW_INSTANCES];

    /**
     * @brief   Window Hash Table <Window Handler, Instance ID>
     */
    static sys::SafePtr<Map<XWND, cfg::uint32>> s_hwndMap;

    /* Static Internal Data */

    static const int s_glxAttribs[ATTRIB_LIST_SIZE];

    static int s_keyCodesMap[NUM_KEYS_SIZE];
    static int s_keyPhysicStates[NUM_KEYS_SIZE];
    static int s_mouseButtonsMap[NUM_BUTTONS_SIZE];

    static XEvent s_event;
    static XkbDescPtr s_kbDesc;
    static Display* s_display;
    static Screen*  s_screen;
    static int s_screenID;

    static int s_fbCount;
    static GLXFBConfig* s_fbConfigs;

    static bool s_vSyncCompat;
    static bool s_attribCtxCompat;
    static int s_glxCtxVersionMajorCompat;
    static int s_glxCtxVersionMinorCompat;

    static PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB;

    static bool glXSwapIntervalEXTMode;
    static PFNGLXSWAPINTERVALPROC1 glXSwapInterval1;
    static PFNGLXSWAPINTERVALPROC2 glXSwapInterval2;

    static void internalSetGlxContextVersion(const int major, const int minor);

    static void loadInputMap();
    static int rawToStandard(int rawCode);

    static void loadGLExtensions();
    static GLXFBConfig chooseBestFBC();

    static void fatalError(const char* msg);

    static bool isExtensionSupported(const char* extList, const char* extension);

    static void CurlyProc();

    /* Deleted Constructors and assignment */

    WindowManager(const WindowManager&) = delete;
    WindowManager(WindowManager&&) = delete;

    WindowManager& operator=(const WindowManager&) = delete;
};

} // namespace wnd

#endif // CURLY_LINUX_WINDOW_MANAGER_HPP
