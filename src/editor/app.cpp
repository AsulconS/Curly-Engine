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

#include <engine/math.hpp>
#include <engine/system.hpp>
#include <engine/window.hpp>
#include <engine/graphics.hpp>

#include "../engine/core/GL/gl.h"

#include <iostream>

static sys::Timer timer(true);

//--------------------------------------------------------------------------------
bool mainLoop(wnd::EditorWindow* window, const wnd::WindowTickType tickType)
{
    timer.tick();
    window->pollEvents();
    glClearColor(0.15f, 0.15f, 0.174f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    window->swapBuffers();
    return window->isActive();
}

//--------------------------------------------------------------------------------
int main()
{
    wnd::EditorWindow window(1600, 900, "Curly Engine");
    if (!window.isReady())
    {
        return 1;
    }

    wnd::InputHandler inputHandler;
    window.setInputHandler(inputHandler);
    window.setTickCallbackFunction(mainLoop);
    return window.startTicking();
}
