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
        currentObject=(obj);
    }
}manager_Selection;

inline void Window_Basic()
{
    ImGui::Begin("Hello 222");
    ImGui::Text("Hello world!");
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
            Manager_Scene.sceneLoaded = true;
        }
        else if(ImGui::Button("Load Scene"))
        {
            std::cout << "Load scene pressed\n";
            LoadScene();
        	Manager_Scene.sceneLoaded = true;
        }
    }
    else
    {

        //scene name
        ImGui::Text(Manager_Scene.currentScene.sceneName.c_str());

        //add to scene contents
        if (ImGui::Button("Add Object"))
        {
        	const std::shared_ptr<GameObject> newGameObject= std::make_shared<GameObject>("example" + std::to_string(Manager_Scene.currentScene.gameObjectList.size()));
            Manager_Scene.currentScene.AddToScene(newGameObject);
        }
        //display scene contents
       
        for ( std::shared_ptr<GameObject>& obj : Manager_Scene.currentScene.gameObjectList) 
        {
            std::string selectedName = "";
            //ImGui::Text(obj.name.c_str());
            if (ImGui::Selectable(obj->name.c_str(), selectedName == obj->name)) {
                selectedName = obj->name;
                std::shared_ptr<GameObject> currentObject = std::make_shared<GameObject>(obj->name);
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

    if (manager_Selection.currentObject) {
        std::shared_ptr<GameObject>& selectedObject = manager_Selection.currentObject;

        ImGui::Text(selectedObject->name.c_str());

        // Create temporary variables to track changes
        glm::vec3 position = selectedObject->transform.translation;
        glm::vec3 rotation = selectedObject->transform.rotation;
        glm::vec3 scale = selectedObject->transform.scale;

        ImGui::Text("Position");
        ImGui::SliderFloat("X Pos", &position.x, -50.0f, 50.0f);
        ImGui::SliderFloat("Y Pos", &position.y, -50.0f, 50.0f);
        ImGui::SliderFloat("Z Pos", &position.z, -50.0f, 50.0f);


        ImGui::Text("Rotation");
        //ImGui::InputFloat("X##Rotation", &rotation.x);
        ImGui::SliderFloat("X Rot", &rotation.x, -180.0f, 180.0f);
        ImGui::SliderFloat("Y Rot", &rotation.y, -180.0f, 180.0f);
        ImGui::SliderFloat("Z Rot", &rotation.z, -180.0f, 180.0f);
        //ImGui::InputFloat("Z##Rotation", &rotation.z);

        ImGui::Text("Scale");
        //ImGui::InputFloat("X##Scale", &scale.x);
        ImGui::SliderFloat("X Scale", &scale.x, 0.0f, 1.0f);
        ImGui::SliderFloat("Y Scale", &scale.y, 0.0f, 1.0f);
        ImGui::SliderFloat("Z Scale", &scale.z, 0.0f, 1.0f);

        //ImGui::InputFloat("Y##Scale", &scale.y);
        //ImGui::InputFloat("Z##Scale", &scale.z);

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

inline void Window_Debug()//window for debugging
{
	
}

