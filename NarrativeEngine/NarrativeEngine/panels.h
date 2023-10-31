#pragma once

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
    GameObject currentObject;

    Selector() { currentObject = GameObject(); }
    void changeSelection(GameObject obj)
    {
        currentObject = obj;
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
            const GameObject newGameObject("Example");
            Manager_Scene.currentScene.AddToScene(newGameObject);
        }
        //display scene contents
       
        for (const GameObject& obj : Manager_Scene.currentScene.gameObjectList) 
        {
            std::string selectedName="xample";
            //ImGui::Text(obj.name.c_str());
            if (ImGui::Selectable(obj.name.c_str(), selectedName == obj.name)) {
                selectedName = obj.name;
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
    
    GameObject selectedObject = manager_Selection.currentObject;

	ImGui::Text(selectedObject.name.c_str());
    

    ImGui::Text("Position");
    ImGui::InputFloat("X##Position", &selectedObject.transform.translation.x);
   // ImGui::SameLine();
    ImGui::InputFloat("Y##Position", &selectedObject.transform.translation.y);
  //  ImGui::SameLine();
    ImGui::InputFloat("Z##Position", &selectedObject.transform.translation.z);
    
    ImGui::Text("Rotation");
    ImGui::InputFloat("X##Rotation", &selectedObject.transform.rotation.x);
    //ImGui::SameLine();
    ImGui::InputFloat("Y##Rotation", &selectedObject.transform.rotation.y);
    //ImGui::SameLine();
    ImGui::InputFloat("Z##Rotation", &selectedObject.transform.rotation.z);

    ImGui::Text("Scale");
    ImGui::InputFloat("X##Scale", &selectedObject.transform.scale.x);
    //ImGui::SameLine();
    ImGui::InputFloat("Y##Scale", &selectedObject.transform.scale.y);
    //ImGui::SameLine();
    ImGui::InputFloat("Z##Scale", &selectedObject.transform.scale.z);
    
    ImGui::End();
}

inline void Window_Debug()//window for debugging
{
	
}

