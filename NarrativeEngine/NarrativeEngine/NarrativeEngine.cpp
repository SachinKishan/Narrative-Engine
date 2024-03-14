
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <thread>
#include <vector>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<glm/gtx/norm.hpp>

#include "camera.h"

//camera
// settings
unsigned int SCR_WIDTH = 1200;
unsigned int SCR_HEIGHT = 800;

Camera camera(glm::vec3(0.0f, 0.0f, -5.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
glm::mat4 view;


std::shared_ptr<Camera> editViewCamera = std::make_shared<Camera>(camera);
std::shared_ptr<Camera> gameViewCamera = std::make_shared<Camera>(camera);
std::shared_ptr<Camera> currentCamera;

void setCamera(std::shared_ptr<Camera> cam)
{
    currentCamera = cam;
}

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

void render()
{
    if (Manager_Scene.sceneLoaded)
    {
        for (std::shared_ptr<GameObject> g : Manager_Scene.currentScene.gameObjectList)
        {
            Shader s = g->renderData->material.shader;
            s.use();
            if(g->objectType==ObjectType::type_Platform || g->objectType==ObjectType::type_Player)
            {
                s.setInt("numberoflights", Manager_Scene.currentScene.lightList.size());
                for (auto it = Manager_Scene.currentScene.lightList.begin(); it != Manager_Scene.currentScene.lightList.end(); ++it)
                {
                    int lightIndex = std::distance(Manager_Scene.currentScene.lightList.begin(), it);
                    std::string uniformPos = "pointLights[" + std::to_string(lightIndex) + "].position";
                    std::string uniformColor = "pointLights[" + std::to_string(lightIndex) + "].color";
                    s.setVec3(uniformPos, (*it)->transform.translation);
                    s.setVec3(uniformColor, (*it)->renderData->material.color*(*it)->getIntensity());
                }
            }
            s.setMat4("projection", projection);
            view = currentCamera->GetViewMatrix();
            s.setMat4("view", view);
            s.setMat4("model", g->transform.model);
            if (g->renderData->material.isTextured)
            {
                glBindTexture(GL_TEXTURE_2D, g->renderData->material.textureID);
                //glBindTexture(GL_TEXTURE_CUBE_MAP,g.renderData.material.textureID);
            }
            else
                s.setVec4("objColor", g->renderData->material.color);
            g->renderData->draw();
        }
    }
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

int main()
{
    renderInitalise();

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Engine", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    initialiseWindow(window);
    
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

	ImguiInitialise(window);

    glEnable(GL_DEPTH_TEST);
    //render buffer things
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH/1.5, SCR_HEIGHT/1.5, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    std::wstring exePath = GetExePath();
    std::filesystem::path shadersPath = std::filesystem::path(exePath) / "Shaders"; // Assuming Shaders folder is in the same directory as the executable


    //quad for framebuffer
	std::filesystem::current_path(shadersPath);
    Shader screenShader("screenShaderVert.vert", "textureFragShader.frag");
    std::shared_ptr<ScreenQuad> quad = std::make_shared<ScreenQuad>();
    Material m(glm::vec4(1, 1, 1, 1));
    quad->SetMaterial(m);
    quad->material.setShader(screenShader);
    quad->material.shader.setInt("texture1", 0);


	setCamera(editViewCamera);
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
    	// input
        // -----
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        glEnable(GL_DEPTH_TEST);

        //after clearing buffer, declare new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
       
        render();
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //screenShader.use();
        //glDisable(GL_DEPTH_TEST);
        render();
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        
        const float window_width = ImGui::GetContentRegionAvail().x;
        const float window_height = ImGui::GetContentRegionAvail().y;
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window_width, window_height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
        /*
        ImGui::Begin("BUFFER WINDOW FRAMEBUFFER ");

        ImVec2 imageSize(SCR_WIDTH / 1.5, SCR_HEIGHT / 1.5);  
        ImVec2 imagePos = ImGui::GetCursorScreenPos();

        ImGui::GetWindowDrawList()->AddImage(
            (void*)textureColorbuffer,
            imagePos,
            ImVec2(imagePos.x + imageSize.x, imagePos.y + imageSize.y),
            ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();
        */

    	if (manager_EditorState.getState() == state_EditorView)
        {
            //editor windows
            Window_Inventory();
            Window_ObjectSelection();
            Window_SceneTree();
        }
        if(manager_EditorState.getState()==state_GameView)
        {
            Window_PlayerInventory();
        }
        if(manager_UI.shouldDisplayText())
        {
            Window_Dialogue();
        }

    	Window_General();
        //Window_Basic();
        Window_Debug();
        ImGui::Render();

    	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
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
    if(glfwGetKey(window,GLFW_KEY_SPACE)==GLFW_PRESS)
    {
        std::cout << std::endl;
        std::cout << std::endl <<"X: " << xpos;
        std::cout << std::endl <<"Y: " << ypos;
    }


	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
    {
        currentCamera->ProcessMouseMovement(xoffset, yoffset);
    }
}

glm::vec3 convertMouseSpace(int x,int y)
{
	//viewport normalise to screen space
    float n_x = (2.0f * x) / SCR_WIDTH - 1.0f;
    float n_y = 1.0f - (2.0f * y) / SCR_HEIGHT;
    float n_z = 1.0f;
    glm::vec3 n_ray(n_x, n_y, n_z);
    glm::vec4 ray_clip(n_ray.x, n_ray.y, -1.0, 1.0);
    glm::vec4 eye_ray = glm::inverse(projection) * ray_clip;
    eye_ray = glm::vec4(eye_ray.x, eye_ray.y, -1.0, 0.0);
 
    glm::vec3 world_ray(glm::inverse(currentCamera->GetViewMatrix()) * eye_ray);
    world_ray = normalize(world_ray);
    return world_ray;//this is the direction of the ray
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
        else if(manager_EditorState.getState()==state_GameView)
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