#include <editor/window/editorWindow.hpp>

#include <engine/math.hpp>
#include <engine/system.hpp>
#include <engine/window.hpp>
#include <engine/graphics.hpp>

#include "../engine/core/GL/gl.h"

#include <iostream>

static sys::Timer timer(true);

bool mainLoop(wnd::EditorWindow* window)
{
    timer.tick();
    window->pollEvents();
    glClearColor(0.15f, 0.15f, 0.174f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    window->swapBuffers();
    return window->isActive();
}

int main()
{
    wnd::EditorWindow window(1920, 1080, "Curly Engine");
    if (!window.isReady())
    {
        return 1;
    }

    wnd::InputHandler inputHandler;
    window.setInputHandler(inputHandler);
    window.setMainLoopCallbackFunction(mainLoop);
    return window.mainLoop();
}
