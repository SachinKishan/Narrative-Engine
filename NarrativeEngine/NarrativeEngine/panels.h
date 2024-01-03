#pragma once

#include <functional>
#include "scene.h"
/**
 * Every window has the following
 * Name- placed in ImGui:Begin(name)
 * Some stuff in it
 * End
 *
 */

class Selector
{

public:
    std::shared_ptr<GameObject> currentObject;
    Selector()
    {
        currentObject = std::make_shared<GameObject>();
    }

    void changeSelection(std::shared_ptr<GameObject>& obj)
    {
        if (obj != nullptr)
            currentObject = (obj);
        //else
            //std::cerr <<std::endl <<"ERROR: OBJECT SELECTION IS NULL!"<<std::endl;
    }
}manager_Selection;

enum editor_state
{
	state_GameView,
    state_EditorView
};
class EditorState
{
private:
    editor_state state = editor_state::state_EditorView;

public:
    void setState(editor_state new_state)
    {
        state = new_state;
    }
	editor_state getState()
    {
        return state;
    }
}manager_EditorState;

inline void Window_Basic()
{
    ImGui::Begin("Hello 222");
    ImGui::Text("Hello world!");


    ImGui::BeginGroup();
    ImGui::Button("hello");
    ImGui::Text("whoopee");


	ImGui::EndGroup();

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            // File menu items
            if (ImGui::MenuItem("Open")) {
                // Handle open action
            }
            if (ImGui::MenuItem("Save")) {
                // Handle save action
            }
            if (ImGui::MenuItem("Exit")) {
                // Handle exit action
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit")) {
            // Edit menu items
            if (ImGui::MenuItem("Cut")) {
                // Handle cut action
            }
            if (ImGui::MenuItem("Copy")) {
                // Handle copy action
            }
            if (ImGui::MenuItem("Paste")) {
                // Handle paste action
            }

            ImGui::EndMenu();
        }

        // Add more menus as needed

        ImGui::EndMainMenuBar();
    }

    // Rest of your ImGui rendering code
    ImGui::Text("Hello, world!");

	ImGui::End();



}

inline void Window_SceneTree()
{

    
    ImGui::Begin("Scene Tree");
    

    if (!Manager_Scene.sceneLoaded)
    {
        if (ImGui::Button("Create Scene"))
        {
            std::cout << "Create scene pressed\n";
            CreateNewScene();
        }
        else if (ImGui::Button("Load Scene"))
        {
            std::cout << "Load scene pressed\n";
            LoadScene();
        }
    }
    else
    {

        //scene name
        ImGui::Text(Manager_Scene.currentScene.sceneName.c_str());

        
        if(ImGui::BeginMenu("Add"))
        {
            //add to scene contents

            if (ImGui::Button("Add Light"))
            {
                const std::shared_ptr<Light> newLight = std::make_shared<Light>("light" + std::to_string(Manager_Scene.currentScene.lightList.size()));
                Manager_Scene.currentScene.AddLight(newLight);
                Manager_Scene.currentScene.AddToScene(newLight);
            }
            if (ImGui::Button("Add platform"))
            {
                const std::shared_ptr<Platform> newGameObject = std::make_shared<Platform>("platform" + std::to_string(Manager_Scene.currentScene.gameObjectList.size()));
                Manager_Scene.currentScene.AddToScene(newGameObject);
            }
            if (ImGui::Button("Add Player") && Manager_Scene.currentScene.player==nullptr)
            {
                const std::shared_ptr<Player> newGameObject = std::make_shared<Player>("Player");
                Manager_Scene.currentScene.AddToScene(newGameObject);
                Manager_Scene.currentScene.player = newGameObject;
            }
            if (ImGui::Button("Add Movement Position"))
            {
                const std::shared_ptr<MovementPoint> newGameObject = std::make_shared<MovementPoint>("move");
                Manager_Scene.currentScene.AddToScene(newGameObject);
                Manager_Scene.currentScene.AddMovementPoint(newGameObject);
            }
            if (ImGui::Button("Add Interactable"))
            {

            }
            ImGui::EndMenu();
        }
        if(ImGui::Button("Remove object"))
        {
	        if(manager_Selection.currentObject!=nullptr)
	        {
                std::vector<std::shared_ptr<GameObject>>& currentVector = Manager_Scene.currentScene.gameObjectList;

                Manager_Scene.currentScene.gameObjectList.erase(std::find(
                    Manager_Scene.currentScene.gameObjectList.begin(), Manager_Scene.currentScene.gameObjectList.end(), 
                    manager_Selection.currentObject));

                if(manager_Selection.currentObject->objectType==ObjectType::type_Light)
                {
                    Manager_Scene.currentScene.lightList.erase(std::find(
                        Manager_Scene.currentScene.lightList.begin(), Manager_Scene.currentScene.lightList.end(),
                        manager_Selection.currentObject));
                }
                else if (manager_Selection.currentObject->objectType == ObjectType::type_MovementPoint)
                {
                    Manager_Scene.currentScene.movementPointList.erase(std::find(
                        Manager_Scene.currentScene.movementPointList.begin(), Manager_Scene.currentScene.movementPointList.end(),
                        manager_Selection.currentObject));
                }
	        }
        }

        //display scene contents
       
        for ( std::shared_ptr<GameObject>& obj : Manager_Scene.currentScene.gameObjectList) 
        {
            
            std::string selectedName = "";
            if(manager_Selection.currentObject != nullptr)
	        	selectedName = manager_Selection.currentObject->name;
            //ImGui::Text(obj.name.c_str());
            if (ImGui::Selectable(obj->name.c_str(), selectedName == obj->name)) {
                selectedName = obj->name;
                //std::shared_ptr<GameObject> currentObject = std::make_shared<GameObject>(obj->name);
                manager_Selection.changeSelection(obj);
                std::cout<< selectedName; // Set the selected item index

            }
        }
        //save scene
        if(ImGui::Button("Save Scene"))
        {
            std::cout << "Scene saved\n";
            SaveScene(Manager_Scene.currentScene.gameObjectList);
        }

        //unload scene
	    if(ImGui::Button("Unload Scene"))
	    {
            std::cout << "Scene unloaded\n";
            Manager_Scene.sceneLoaded = false;
	    }
    }


    ImGui::End();
}

inline void Window_ObjectSelection()
{
    ImGui::Begin("Object Selection");

    if (manager_Selection.currentObject!=nullptr) {
        std::shared_ptr<GameObject>& selectedObject = manager_Selection.currentObject;

        
//        ImGui::Text(selectedObject->name.c_str());
        ImGui::Text("Object name: ");
        ImGui::SameLine();
        // Input field to change the object's name
        char objectName[256]; // Assuming a maximum name length of 255 characters
        memset(objectName, 0, sizeof(objectName)); // Clear the memory
        strcpy_s(objectName, selectedObject->name.c_str());
    	ImGui::InputText("##ObjectName", objectName, IM_ARRAYSIZE(objectName));

        // Set the object's name if it's changed
        if (strcmp(objectName, selectedObject->name.c_str()) != 0) {
            selectedObject->name = objectName;
        }

        //transform edit
        glm::vec3 position = selectedObject->transform.translation;
        glm::vec3 bposition = selectedObject->transform.translation;
        glm::vec3 rotation = selectedObject->transform.rotation;
        glm::vec3 brotation = selectedObject->transform.rotation;
        glm::vec3 scale = selectedObject->transform.scale;
        glm::vec3 bscale = selectedObject->transform.scale;

        ImGui::Text("Position");
        ImGui::SliderFloat("X Pos", &position.x, -50.0f, 50.0f);
        ImGui::SameLine();
        ImGui::InputFloat("##XPos", &position.x);
        ImGui::SliderFloat("Y Pos", &position.y, -50.0f, 50.0f);
        ImGui::SameLine();
        ImGui::InputFloat("##YPos", &position.y);
    	ImGui::SliderFloat("Z Pos", &position.z, -50.0f, 50.0f);
        ImGui::SameLine();
        ImGui::InputFloat("##ZPos", &position.z);

        ImGui::Text("Rotation");
        ImGui::SliderFloat("X Rot", &rotation.x, -180.0f, 180.0f);
        ImGui::SameLine();
        ImGui::InputFloat("##XRot", &rotation.x);
        ImGui::SliderFloat("Y Rot", &rotation.y, -180.0f, 180.0f);
        ImGui::SameLine();
        ImGui::InputFloat("##YRot", &rotation.y);
    	ImGui::SliderFloat("Z Rot", &rotation.z, -180.0f, 180.0f);
        ImGui::SameLine();
        ImGui::InputFloat("##ZRot", &rotation.z);

        ImGui::Text("Scale");
        ImGui::SliderFloat("X Scale", &scale.x, 0.0f, 10.0f);
        ImGui::SameLine();
        ImGui::InputFloat("##XSc", &scale.x);
        ImGui::SliderFloat("Y Scale", &scale.y, 0.0f, 10.0f);
        ImGui::SameLine();
        ImGui::InputFloat("##YSc", &scale.y);
        ImGui::SliderFloat("Z Scale", &scale.z, 0.0f, 10.0f);
        ImGui::SameLine();
        ImGui::InputFloat("##ZSc", &scale.z);
        
        //color edit
        if (selectedObject && selectedObject->renderData) {
            float newColor[4] = {
                selectedObject->renderData->material.color.x,
                selectedObject->renderData->material.color.y,
                selectedObject->renderData->material.color.z,
                selectedObject->renderData->material.color.w
            };
            ImGui::ColorEdit4("Color", newColor);
            if (newColor[0] != selectedObject->renderData->material.color.x ||
                newColor[1] != selectedObject->renderData->material.color.y ||
                newColor[2] != selectedObject->renderData->material.color.z ||
                newColor[3] != selectedObject->renderData->material.color.w) {
                selectedObject->renderData->material.color = glm::vec4(newColor[0], newColor[1], newColor[2], newColor[3]);
            }
        }
        if(selectedObject->objectType==ObjectType::type_Light)
        {
            std::shared_ptr<Light> lightObject = std::dynamic_pointer_cast<Light>(selectedObject);
            if (lightObject) 
            {
                float intensity = lightObject->getIntensity();
                ImGui::SliderFloat("Light Intensity", &intensity, 0.0f, 10.0f);
                if (intensity != lightObject->getIntensity())lightObject->setIntensity(intensity);
            }

        }

        // Apply changes if modified
        if (position != selectedObject->transform.translation ||
            rotation != selectedObject->transform.rotation ||
            scale != selectedObject->transform.scale) {
            selectedObject->transform.translation = position;
            selectedObject->transform.rotation = rotation;
            selectedObject->transform.scale = scale;
            selectedObject->transform.CalculateModel();
        }
    	



    }


    ImGui::End();
}

inline void Window_General()
{
    ImGui::Begin("General");

    if (ImGui::Button("Game Mode"))
    {
        manager_EditorState.setState(state_GameView);
    }
    if (ImGui::Button("Edit Mode"))
    {
        manager_EditorState.setState(state_EditorView);
    }

    // Assuming manager_EditorState.getState() returns a string
    if (manager_EditorState.getState() == state_EditorView)ImGui::Text("State: Editor");
    else if (manager_EditorState.getState() == state_GameView)ImGui::Text("State: Game");

    ImGui::End();
}

inline void Window_GameView()
{

    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    //texture for framebuffer
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);




    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    Shader screenTextureShader("screenShaderVert.vert", "textureFragShader.frag");
    std::shared_ptr<ScreenQuad> quad = std::make_shared<ScreenQuad>();
    Material m(glm::vec4(1, 1, 1, 1));
    quad->SetMaterial(m);
    quad->material.setShader(screenTextureShader);
    quad->material.shader.setInt("texture1", 0);






	glDeleteFramebuffers(1, &fbo);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);



	ImGui::Begin(" ");




	ImGui::End();


}






inline void Window_Debug()//window for debugging
{
	
}
