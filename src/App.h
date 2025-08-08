#pragma once

#include "Window.h"
#include "Renderer.h"

#include <iostream>

#include <glad/glad.h>

class App
{
    Window window;
    Renderer renderer;

public:
    App() : window(800, 600, "My openGL app")
    {
        renderer.init();
    }

    void onKeyPressed(int key)
    {
        // if (key == GLFW_KEY_UP)
        // {
        //     renderer.setSomeUniform(renderer.getSomeUniform() + 1.0f);
        // }
    }

    void onResize(int width, int height)
    {
        std::cout << "resized!" << width << "," << height << std::endl;
    }

    void run()
    {
        while (!window.shouldClose())
        {
            renderer.render();
            window.swapBuffers();
            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwPollEvents();
        }
    }
};
