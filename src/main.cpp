#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "tools/matrix_stack.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "includes/game_state.hpp"
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
        clearScene();    
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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    io.FontGlobalScale = 2.8f;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Initialize Rendering Engine
    myEngine.initGL();

    onWindowResized(window, WINDOW_WIDTH, WINDOW_HEIGHT);

    // glfwSetKeyCallback(window, key_callback);
    glfwSetKeyCallback(window, onKey);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    bool gameNeedsInit = false;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Get time (in second) at loop beginning */
        double currentTime = glfwGetTime();

        /* Render here */
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        ImVec2 menuSize = ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT);
        ImVec2 menuPos = ImVec2((display_w - menuSize.x) * 0.5f, (display_h - menuSize.y) * 0.5f);
        ImGui::SetNextWindowPos(menuPos);
        ImGui::SetNextWindowSize(menuSize);

        if(currentPage != Page::GAME) {
            ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

            if(currentPage == Page::MAIN_MENU) {
                ImGui::Dummy(ImVec2(0.0f, 100.0f));
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 200) * 0.24f);
                ImGui::Text("Bienvenue dans IMACANARD !");

                ImGui::Dummy(ImVec2(0.0f, 75.0f));
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 200) * 0.45f);
                if(ImGui::Button("Jouer", ImVec2(250, 70))) {
                    gameNeedsInit = true;
                    currentPage = Page::GAME;
                }

                ImGui::Dummy(ImVec2(0.0f, 25.0f));
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 200) * 0.45f);
                if(ImGui::Button("Paramètres", ImVec2(250, 70))) {
                    currentPage = Page::SETTINGS;
                }

                ImGui::Dummy(ImVec2(0.0f, 20.0f));
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 200) * 0.45f);
                if(ImGui::Button("Règles", ImVec2(250, 70))) {
                    currentPage = Page::RULES;
                }

                ImGui::Dummy(ImVec2(0.0f, 35.0f));
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 200) * 0.45f);
                if(ImGui::Button("Quitter", ImVec2(250, 70))) {
                    clearScene();
                    glfwSetWindowShouldClose(window, GLFW_TRUE);
                }
            } else if (currentPage == Page::SETTINGS) {
                static int mapWidth = width, nbEnemies = numberOfEnemies;

                if (ImGui::Button("Retour")) {
                    currentPage = Page::MAIN_MENU;
                }

                ImGui::Dummy(ImVec2(0.0f, 25.0f));
                ImGui::Text("Paramètres du jeu :");

                ImGui::Dummy(ImVec2(0.0f, 20.0f));
                ImGui::Text("Largeur de la carte :");
                ImGui::SameLine();
                ImGui::SetNextItemWidth(200);
                ImGui::InputInt("##mapWidth", &mapWidth);
                mapWidth = std::max(10, std::min(mapWidth, 50));
                
                ImGui::Dummy(ImVec2(0.0f, 20.0f));
                ImGui::Text("Nombre d'ennemis :");
                ImGui::SameLine();
                ImGui::SetNextItemWidth(200);
                ImGui::InputInt("##nb_enemies", &nbEnemies);
                nbEnemies = std::max(2, std::min(nbEnemies, 5));
                
                ImGui::Dummy(ImVec2(0.0f, 25.0f));
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 200) * 0.25f);
                if(ImGui::Button("Réinitialiser")) {
                    mapWidth = 25; width = 25;
                    nbEnemies = 2; numberOfEnemies = 2;
                }
                ImGui::SameLine();
                if(ImGui::Button("Enregistrer")) {
                    width = mapWidth;
                    numberOfEnemies = nbEnemies;
                }
            } else if (currentPage == Page::RULES) {
                if (ImGui::Button("Retour")) {
                    currentPage = Page::MAIN_MENU;
                }

                ImGui::Dummy(ImVec2(0.0f, 25.0f));
                ImGui::Text("Règles du jeu :");

                ImGui::Dummy(ImVec2(0.0f, 20.0f));
                ImGui::Bullet();
                ImGui::TextWrapped("Déplacez votre canard avec les flèches.");

                ImGui::Dummy(ImVec2(0.0f, 20.0f));
                ImGui::Bullet();
                ImGui::TextWrapped("Vous pouvez cassez les nénupahres en appuyant sur 'A'");

                ImGui::Dummy(ImVec2(0.0f, 20.0f));
                ImGui::Bullet();
                ImGui::TextWrapped("Évitez les tourbillons et survivez au ennemis !");
                
                ImGui::Dummy(ImVec2(0.0f, 20.0f));
                ImGui::Bullet();
                ImGui::TextWrapped("Manger les graines et les poissons pour marquer des points.");
            } else if (currentPage == Page::END_SCREEN) {
                ImGui::Dummy(ImVec2(0.0f, 150.0f));
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 200) * 0.5f);
                if(gameStatus == GameStatus::WIN) {
                    ImGui::Text("VICTOIRE");
                } else if(gameStatus == GameStatus::LOSE) {
                    ImGui::Text("DEFAITE");
                }

                ImGui::Dummy(ImVec2(0.0f, 100.0f));
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 200) * 0.5f);
                if(ImGui::Button("Rejouer")) {
                    gameNeedsInit = true;
                    clearScene();
                    currentPage = Page::GAME;
                }

                ImGui::Dummy(ImVec2(0.0f, 50.0f));
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 200) * 0.45f);
                if(ImGui::Button("Menu")) {
                    currentPage = Page::MAIN_MENU;
                }
                ImGui::SameLine();
                if(ImGui::Button("Quitter")) {
                    clearScene();
                    glfwSetWindowShouldClose(window, GLFW_TRUE);
                }
            }

            ImGui::End();
        }

        if(currentPage == Page::GAME) {
            if (gameNeedsInit) {
                clearScene();
                initScene();
                gameNeedsInit = false;
            }
            glClearColor(0.f, 0.0f, 0.2f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glDisable(GL_DEPTH_TEST);

            renderScene();

            ImGui::SetNextWindowBgAlpha(0.1f);       // Transparence de la fenêtre
            ImGui::Begin("Score", nullptr,
                        ImGuiWindowFlags_NoDecoration | 
                        ImGuiWindowFlags_AlwaysAutoResize |
                        ImGuiWindowFlags_NoFocusOnAppearing |
                        ImGuiWindowFlags_NoNav);
            ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + 500);
            ImGui::TextWrapped("Nombre d'objets restant pour gagner : %d", remainingObjects);
            ImGui::PopTextWrapPos();
            ImGui::End();
        }
        else
        {
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}