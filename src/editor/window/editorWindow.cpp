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

#include <editor/window/editorWindow.hpp>

#include <engine/exception/window/windowException.hpp>

#include <engine/window/windowParams.hpp>

#include <iostream>

#include "windowManager.hpp"
namespace wnd
{
//--------------------------------------------------------------------------------
EditorWindow::EditorWindow(const cfg::uint32 t_width, const cfg::uint32 t_height, const char* t_title, WindowStyle t_style, InputHandler* t_inputHandler)
    : wnd::IWindow {t_width, t_height, t_title, t_style, t_inputHandler},
      m_appMainProc { nullptr },
      m_tickCallback { nullptr },
      m_tickStaticCallback { nullptr }
{
    m_windowManager = WindowManager::createInstance();
    m_windowManager->registerWindowInstance(this);
    m_windowManager->setEventCallbackFunction(eventCallback);
    m_windowManager->setExternalTickCallbackFunction(externalTickCallback);
    try
    {
        initializeWindow();
        m_windowManager->pollEvents();
        m_ready = true;
    }
    catch (const exc::GenericException& e)
    {
        std::cerr << "An Exception has occurred: " << e.what() << std::endl;
        m_ready = false;
    }
}

//--------------------------------------------------------------------------------
EditorWindow::~EditorWindow()
{
    std::cout << "Destructing Window " << m_title << std::endl;
    if (isActive())
    {
        close();
    }
    std::cout << "Window " << m_title << " destroyed" << std::endl;
}

//--------------------------------------------------------------------------------
int EditorWindow::startTicking()
{
    while (tick());
    return 0;
}

//--------------------------------------------------------------------------------
bool EditorWindow::tick()
{
    return invokeTickCallback(this, WindowTickType::PROC_TICK);
}

//--------------------------------------------------------------------------------
bool EditorWindow::externalTick()
{
    return invokeTickCallback(this, WindowTickType::EXTERNAL_TICK);
}

//--------------------------------------------------------------------------------
bool EditorWindow::isActive()
{
    return m_windowManager->isActive();
}

//--------------------------------------------------------------------------------
bool EditorWindow::isReady()
{
    return m_ready;
}

//--------------------------------------------------------------------------------
void EditorWindow::close()
{
    m_windowManager->destroyWindow();
}

//--------------------------------------------------------------------------------
void EditorWindow::setInputHandler(InputHandler& t_inputHandler)
{
    m_inputHandler = &t_inputHandler;
}

//--------------------------------------------------------------------------------
void EditorWindow::pollEvents()
{
    if(m_inputHandler != nullptr)
    {
        m_inputHandler->_tick();
    }
    m_windowManager->pollEvents();
}

//--------------------------------------------------------------------------------
void EditorWindow::swapBuffers()
{
    m_windowManager->swapBuffers();
}

//--------------------------------------------------------------------------------
void EditorWindow::bindTickCallbackFunction(AppMainProc* appMainProc, TickCallback callback)
{
    m_appMainProc = appMainProc;
    m_tickCallback = callback;
    m_tickStaticCallback = nullptr;
}

//--------------------------------------------------------------------------------
void EditorWindow::bindStaticTickCallbackFunction(TickStaticCallback callback)
{
    m_appMainProc = nullptr;
    m_tickCallback = nullptr;
    m_tickStaticCallback = callback;
}

//--------------------------------------------------------------------------------
float EditorWindow::getAspectRatio() const
{
    return static_cast<float>(m_windowWidth) / static_cast<float>(m_windowHeight);
}

//--------------------------------------------------------------------------------
math::Vec2i EditorWindow::getWindowRect() const
{
    return { m_windowWidth, m_windowHeight };
}

//--------------------------------------------------------------------------------
math::Vec2i EditorWindow::getViewportRect() const
{
    return { m_viewportWidth, m_viewportHeight };
}

//--------------------------------------------------------------------------------
void EditorWindow::initializeWindow()
{
    WindowRectParams rectParams{ m_windowManager->createEditorWindow(m_title, 0, 0, m_windowWidth, m_windowHeight, m_style) };
    m_windowWidth = rectParams.windowWidth;
    m_windowHeight = rectParams.windowHeight;
    m_viewportWidth = rectParams.clientWidth;
    m_viewportHeight = rectParams.clientHeight;
    if(!m_windowManager->isActive())
    {
        throw exc::WindowInitException();
    }
}

//--------------------------------------------------------------------------------
bool EditorWindow::invokeTickCallback(EditorWindow* window, const WindowTickType tickType)
{
    if (m_tickStaticCallback != nullptr)
    {
        return m_tickStaticCallback(window, tickType);
    }
    if ((m_appMainProc == nullptr) || (m_tickCallback == nullptr))
    {
        return false;
    }
    return (m_appMainProc->*m_tickCallback)(window, tickType);
}

//--------------------------------------------------------------------------------
void EditorWindow::eventCallback(IWindow* window, InputEvent event, WindowParams* params)
{
    EditorWindow* eWindow{ static_cast<EditorWindow*>(window) };
    if (eWindow->m_inputHandler == nullptr)
    {
        return;
    }
    switch (event)
    {
        case KEY_PRESSED:
        case KEY_RELEASED:
        {
            eWindow->m_inputHandler->_updateKeyEvent(static_cast<KeyboardParams*>(params)->code, event);
            break;
        }

        case BUTTON_PRESSED:
        case BUTTON_RELEASED:
        {
            eWindow->m_inputHandler->_updateMouseEvent(static_cast<MouseParams*>(params)->code, event);
            break;
        }

        case MOUSE_MOVE:
        {
            eWindow->m_inputHandler->_updateMousePosition(static_cast<MouseParams*>(params)->pos);
            break;
        }

        default:
            break;
    }
}

//--------------------------------------------------------------------------------
bool EditorWindow::externalTickCallback(IWindow* window)
{
    EditorWindow* eWindow{ static_cast<EditorWindow*>(window) };
    if (eWindow == nullptr)
    {
        return false;
    }
    return eWindow->externalTick();
}

} // namespace wnd
