
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

#include "CallBack.h"

#include "model_loading.h"


void modelRenderTest(Model m)
{
    std::wstring exePath = GetExePath();
    std::filesystem::path shadersPath = std::filesystem::path(exePath) / "Shaders"; // Assuming Shaders folder is in the same directory as the executable

    std::filesystem::current_path(shadersPath);

    Shader s("defaultShader.vert", "colorShader.frag");
    s.use();

    s.setMat4("projection", projection);
    view = currentCamera->GetViewMatrix();
    s.setMat4("view", view);


    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
    s.setMat4("model", model);

	m.Draw(s);


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

    exePath = GetExePath();
    std::filesystem::current_path(exePath); //setting path

    Model bp("backpack/backpack.obj");




    // render loop
    // -----------
    while (!glfwWindowShouldClose(window) && shouldRun)
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
       
        modelRenderTest(bp);

    	//render();




        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //screenShader.use();
        //glDisable(GL_DEPTH_TEST);

    	//render();
        modelRenderTest(bp);

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
            Window_GameBuilder();

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

