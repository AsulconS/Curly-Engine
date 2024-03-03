#include <editor/window/editorWindow.hpp>

#include <engine/math.hpp>
#include <engine/system.hpp>
#include <engine/window.hpp>
#include <engine/graphics.hpp>

#include "../engine/core/GL/gl.h"

#include <iostream>

int main()
{
    wnd::EditorWindow window(1920, 1080, "Curly Engine");
    if (!window.isReady())
    {
        return 1;
    }

    wnd::InputHandler inputHandler;
    window.setInputHandler(inputHandler);
    while (window.isActive())
    {
        window.pollEvents();
        if (inputHandler.onKeyTriggered(wnd::KEY_ESCAPE))
        {
            window.close();
            continue;
        }
        glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        window.swapBuffers();
    }

    return 0;
}
