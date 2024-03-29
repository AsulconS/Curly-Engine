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

#include <core/config.hpp>
#include <core/common.hpp>

#include <window/inputHandler.hpp>
#include <window/customization.hpp>

namespace wnd
{
/**
 * @brief Window Class that handle a Window of the program
 * 
 */
class CURLY_API IWindow
{
public:
    /**
     * @brief Construct a new IWindow object
     * 
     * @param t_width 
     * @param t_height 
     * @param t_title 
     * @param t_eventHandler 
     */
    IWindow(const cfg::uint32 t_width, const cfg::uint32 t_height, const char* t_title, WindowStyle t_style, InputHandler* t_inputHandler) : m_windowWidth {t_width}, m_windowHeight {t_height}, m_title {t_title}, m_style {t_style}, m_ready {false}, m_inputHandler {t_inputHandler} {}
    /**
     * @brief Destroy the Window object
     * 
     */
    virtual ~IWindow() {}

    /**
     * @brief Check if the Window shouldn't close
     * 
     * @return true if the Window is active
     * @return false if the Windows is not active
     */
    virtual bool isActive() = 0;
    /**
     * @brief Check if the Window is able to start rendering
     * 
     * @return true if the Window is able
     * @return false if the Window is not able
     */
    virtual bool isReady() = 0;
    /**
     * @brief Send signal to close window
     * 
     */
    virtual void close() = 0;

    /**
     * @brief Set the Input Handler object
     * 
     */
    virtual void setInputHandler(InputHandler& t_inputHandler) = 0;
    /**
     * @brief Poll the Events to process the input
     * 
     */
    virtual void pollEvents() = 0;
    /**
     * @brief Get the Aspect Ratio
     * 
     * @return float value containing the current aspect ratio
     */
    virtual float getAspectRatio() const = 0;
    /**
     * @brief Get the Window Rect object as Vec2i
     * 
     * @return Vec2i vector containing width and height of the window in pixels
     */
    virtual math::Vec2i getWindowRect() const = 0;
    /**
     * @brief Get the Workspace Rect object as Vec2i
     * 
     * @return Vec2i vector containing width and height of the viewport in pixels
     */
    virtual math::Vec2i getViewportRect() const = 0;

protected:
    /**
     * @brief Initializes the Window itself
     * 
     */
    virtual void initializeWindow() = 0;

    cfg::uint32 m_windowWidth;
    cfg::uint32 m_windowHeight;
    cfg::uint32 m_viewportWidth;
    cfg::uint32 m_viewportHeight;
    const char* m_title;
    WindowStyle m_style;

    bool m_ready;

    InputHandler* m_inputHandler;
    class WindowManager* m_windowManager;
};

} // namespace wnd
