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

#include <editor/core/config.hpp>

#include <editor/core/appMainProc.hpp>

#include <engine/core/common.hpp>

#include <engine/window/iWindow.hpp>
#include <engine/window/inputEvents.hpp>
#include <engine/window/inputHandler.hpp>
#include <engine/window/customization.hpp>

namespace wnd
{
/**
 * @brief Window Tick Type Class that identifies the type of tick being called
 * 
 */
enum class CURLY_EDITOR_API WindowTickType : cfg::uint8
{
    NONE,
    PROC_TICK,
    EXTERNAL_TICK
};

/**
 * @brief Rendering Window Class that handle a window for render of the program
 * 
 */
class CURLY_EDITOR_API EditorWindow : public IWindow
{
public:
    using TickCallback = bool (AppMainProc::*)(EditorWindow*, const WindowTickType);
    using TickStaticCallback = bool (*)(EditorWindow*, const WindowTickType);

public:
    /**
     * @brief Construct a new EditorWindow object
     * 
     * @param t_title 
     * @param t_width 
     * @param t_height 
     */
    explicit EditorWindow(const cfg::uint32 t_width = 800u, const cfg::uint32 t_height = 600u, const char* t_title = "Untitled", WindowStyle t_style = WindowStyle::WINDOWED_STYLE, InputHandler* t_inputHandler = nullptr);
    /**
     * @brief Destroy the EditorWindow object
     * 
     */
    virtual ~EditorWindow();

    /**
     * @brief Starts Main Loop
     * 
     * @return int 
     */
    int startTicking();
    /**
     * @brief Starts Registered tick
     * 
     */
    bool tick();
    /**
     * @brief Ticks the main loop once
     * 
     */
    bool externalTick();

    /**
     * @brief Swap the framebuffers
     * 
     */
    void swapBuffers();

    /**
     * @brief Binds the Tick Callback Function to a member function pointer
     * 
     * @param obj 
     * @param callback 
     */
    void bindTickCallbackFunction(AppMainProc* appMainProc, TickCallback callback);
    /**
     * @brief Binds the Tick Callback Function to a static function pointer
     * 
     * @param callback 
     */
    void bindStaticTickCallbackFunction(TickStaticCallback callback);

    /**
     * @brief Check if the Window shouldn't close
     * 
     * @return true if the Window is active
     * @return false if not
     */
    virtual bool isActive() override;
    /**
     * @brief Check if the Window is able to start rendering
     * 
     * @return true if right
     * @return false if not
     */
    virtual bool isReady() override;
    /**
     * @brief Send signal to close window
     * 
     */
    virtual void close() override;

    /**
     * @brief Set the Input Handler object
     * 
     */
    virtual void setInputHandler(InputHandler& t_inputHandler) override;
    /**
     * @brief Poll the Events to process the input
     * 
     */
    virtual void pollEvents() override;
    /**
     * @brief Get the Aspect Ratio
     * 
     * @return float 
     */
    virtual float getAspectRatio() const override;
    /**
     * @brief Get the Window Rect object as Vec2i
     * 
     * @return Vec2i vector containing width and height of the window in pixels
     */
    virtual math::Vec2i getWindowRect() const override;
    /**
     * @brief Get the Workspace Rect object as Vec2i
     * 
     * @return Vec2i vector containing width and height of the viewport in pixels
     */
    virtual math::Vec2i getViewportRect() const override;

protected:
    /**
     * @brief Initializes the Window itself
     * 
     */
    virtual void initializeWindow() override;

private:
    /**
     * @brief Invokes bound tick callback
     * 
     */
    bool invokeTickCallback(EditorWindow* window, const WindowTickType tickType);

    /**
     * @brief Key Callback function
     * 
     * @param window 
     * @param event 
     * @param inputCode 
     * @param repeat 
     */
    static void eventCallback(IWindow* window, InputEvent event, struct WindowParams* params);
    /**
     * @brief Ticks the main loop once (Callback Version)
     * 
     */
    static bool externalTickCallback(IWindow* window);

private:
    TickCallback m_tickCallback;
    TickStaticCallback m_tickStaticCallback;
    AppMainProc* m_appMainProc;
};

} // namespace ewnd
