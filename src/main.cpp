#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "tools/matrix_stack.hpp"
#include "includes/display.hpp"

#include <iostream>
#include <cmath>

/* Window size */
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 800;

using namespace glbasimac;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;
static float aspectRatio = 1.0f;
static const float GL_VIEW_SIZE = 1.0f;

/* Error handling function */
void onError(int error, const char *description)
{
    std::cout << "GLFW Error (" << error << ") : " << description << std::endl;
}

void onWindowResized(GLFWwindow * /*window*/, int width, int height)
{
    aspectRatio = width / (float)height;
    glViewport(0, 0, width, height);

    if (aspectRatio > 1.0)
    {
        myEngine.set2DProjection(-GL_VIEW_SIZE * aspectRatio / 2., GL_VIEW_SIZE * aspectRatio / 2., -GL_VIEW_SIZE / 2., GL_VIEW_SIZE / 2.);
    }
    else
    {
        myEngine.set2DProjection(-GL_VIEW_SIZE / 2., GL_VIEW_SIZE / 2., -GL_VIEW_SIZE / (2. * aspectRatio), GL_VIEW_SIZE / (2. * aspectRatio));
    }
}

void onKey(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    // int isPressed = (action == GLFW_PRESS);
    key_callback(window, key, scancode, action, mods);
    switch (key)
    {
    case GLFW_KEY_A: // equals to q because well qwerty and azerty keyboards are different
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        break;

    case GLFW_KEY_T:
        myEngine.activateTexturing(true);
        break;

    case GLFW_KEY_Y:
        myEngine.activateTexturing(false);
        break;
    default:
        break;
    }
};

int main()
{
    // Initialize the library
    if (!glfwInit())
    {
        return -1;
    }

    /* Callback to a function if an error is rised by GLFW */
    glfwSetErrorCallback(onError);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "IMACANARD", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // -- Callbacks --
    glfwSetWindowSizeCallback(window, onWindowResized);

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Intialize glad (loads the OpenGL functions)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }

    // Initialize Rendering Engine
    myEngine.initGL();

    onWindowResized(window, WINDOW_WIDTH, WINDOW_HEIGHT);

    // glfwSetKeyCallback(window, key_callback);
    glfwSetKeyCallback(window, onKey);

    initScene();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Get time (in second) at loop beginning */
        double currentTime = glfwGetTime();

        /* Render here */
        glClearColor(0.f, 0.0f, 0.2f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);

        renderScene();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        /* Elapsed time computation from loop begining */
        double elapsedTime = glfwGetTime() - currentTime;
        /* If to few time is spend vs our wanted FPS, we wait */
        while (elapsedTime < FRAMERATE_IN_SECONDS)
        {
            glfwWaitEventsTimeout(FRAMERATE_IN_SECONDS - elapsedTime);
            elapsedTime = glfwGetTime() - currentTime;
        }
    }

    glfwTerminate();
    return 0;
}