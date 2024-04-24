#pragma once
#include "camera.h"
#include "panels.h"



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
}

void processInput(GLFWwindow* window)
{

    bool isIMGUIInputActive = ImGui::GetIO().WantCaptureKeyboard;

    // If no IMGUI text input is active, process your GLFW keyboard bindings
    if (!isIMGUIInputActive) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            currentCamera->ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            currentCamera->ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            currentCamera->ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            currentCamera->ProcessKeyboard(RIGHT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            currentCamera->ProcessKeyboard(UP, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            currentCamera->ProcessKeyboard(DOWN, deltaTime);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        std::cout << std::endl;
        std::cout << std::endl << "X: " << xpos;
        std::cout << std::endl << "Y: " << ypos;
    }


    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
    {
        currentCamera->ProcessMouseMovement(xoffset, yoffset);
    }
}



void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        if (ImGui::GetIO().WantCaptureMouse | ImGui::GetIO().WantCaptureKeyboard) //inside an imgui window
        {
            return;
        }
        if (manager_EditorState.getState() == state_EditorView)
        {

            double x = 0, y = 0;
            glfwGetCursorPos(window, &x, &y);
            glm::vec3 converted = convertMouseSpace(x, y);
            std::shared_ptr<GameObject> obj = nullptr;
            ray_collision(currentCamera->Position, converted, Manager_Scene.currentScene.gameObjectList, obj);
            manager_Selection.changeSelection(obj);
        }
        else if (manager_EditorState.getState() == state_GameView)
        {
            double x = 0, y = 0;
            glfwGetCursorPos(window, &x, &y);
            glm::vec3 converted = convertMouseSpace(x, y);
            std::shared_ptr<MovementPoint> obj = nullptr;
            ray_collision(currentCamera->Position, converted, Manager_Scene.currentScene.movementPointList, obj);

            if (obj != nullptr && Manager_Scene.currentScene.hasPlayer)//player has clicked a movement point
            {

                manager_GameManager.MovePlayer(obj);

            }
        }
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    currentCamera->ProcessMouseScroll(static_cast<float>(yoffset));
}

void renderInitalise()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void ImguiInitialise(GLFWwindow* window)
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::GetIO().FontGlobalScale = 1.3;

}

void initialiseWindow(GLFWwindow* window)
{
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
}

