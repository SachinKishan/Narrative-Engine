#pragma once

#include <functional>
#include "scene.h"
#include "GameBuilder.h"

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
        ImGui::Text(clean_string_for_display(Manager_Scene.currentScene.sceneName).c_str());

        
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
            if (ImGui::Button("Add Player") && Manager_Scene.currentScene.hasPlayer==false)
            {
                const std::shared_ptr<Player> newGameObject = std::make_shared<Player>("Player");
                Manager_Scene.currentScene.AddToScene(newGameObject);
                Manager_Scene.currentScene.SceneSetPlayer(newGameObject);
            }
            if (ImGui::Button("Add Movement Position"))
            {
                const std::shared_ptr<MovementPoint> newGameObject = std::make_shared<MovementPoint>("move" + std::to_string(Manager_Scene.currentScene.movementPointList.size()));
                Manager_Scene.currentScene.AddToScene(newGameObject);
                Manager_Scene.currentScene.AddMovementPoint(newGameObject);
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

       
        for (std::shared_ptr<GameObject>& obj : Manager_Scene.currentScene.gameObjectList) 
        {
            std::string selectedName = "";
            if(manager_Selection.currentObject != nullptr)
	        	selectedName = manager_Selection.currentObject->name;
            if (ImGui::Selectable(clean_string_for_display(obj->name).c_str(), selectedName == obj->name)) {
                selectedName = obj->name;
                manager_Selection.changeSelection(obj);
                //std::cout<< selectedName; 
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

void InventorySelectionDisplay(std::shared_ptr<Event> event,int i)
{
            std::shared_ptr<Event_Inventory> ep = std::static_pointer_cast<Event_Inventory>(event);
            std::string popupID = "Select Inventory Item#" + std::to_string(i);
            if (ImGui::BeginMenu(popupID.c_str()))
            {
                for (const auto& itemName : manager_Inventory.getItemNames()) {
                    if (ImGui::MenuItem(itemName.c_str()))
                    {
                        printf("Selected item: %s\n",clean_string_for_display(itemName).c_str());

                        ep->setItem(itemName, 0);
                    }
                }
                ImGui::EndMenu();

            }
}

inline void EventSelection(std::shared_ptr<MovementPoint>& point)
{
    if (ImGui::BeginMenu("Add New Event"))
    {
        if (ImGui::Button("Add text box event"))
        {
            std::shared_ptr<Event_TextBox> e = std::make_shared<Event_TextBox>();
            point->events.push_back(e);
            std::cout << "Event added";
        }
        /*if (ImGui::Button("Add print number event"))
                {
                    std::shared_ptr<PrintNum_Event> e = std::make_shared<PrintNum_Event>();
                    point->events.push_back(e);
                    std::cout << "Event added";
                }*/
        if (ImGui::Button("Add inventory event"))
        {
            std::shared_ptr<Event_Inventory> e = std::make_shared<Event_Inventory>();
            point->events.push_back(e);
            std::cout << "Event added";
        }
        if(ImGui::Button("Add scene change event"))
        {
            std::shared_ptr<Event_SceneChange> e = std::make_shared<Event_SceneChange>();
            point->events.push_back(e);
            std::cout << "Event added";
        }
        if (ImGui::Button("Delete last event"))
        {
            point->events.pop_back();
        }

        ImGui::EndMenu();
    }
}

void EventEditor(std::shared_ptr<MovementPoint> &point)
{
    for (size_t i = 0; i < point->events.size(); ++i) 
    {
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();

        auto event = point->events[i];

        switch (event->getType())
        {
            //case EventType::Print: ImGui::Text("Print a number"); break;
        case EventType::TextBox: ImGui::Text("Display Text Event"); break;
        case EventType::Inventory: ImGui::Text("Inventory event"); break;
        default: break;
        }



        char eventName[256]; // Assuming a maximum name length of 255 characters
        memset(eventName, 0, sizeof(eventName)); // Clear the memory
        strcpy_s(eventName, event->getName().c_str());

        // Concatenate the index to the label to make it unique
        std::string label = "Event Name##" + std::to_string(i);

        ImGui::InputText(label.c_str(), eventName, IM_ARRAYSIZE(eventName));

        // Set the object's name if it's changed
        if (strcmp(eventName, event->getName().c_str()) != 0) {
            event->setEventName(eventName);
        }

        // Display a combo box for selecting the event type
        const char* eventTypes[] = { "Enter", "Exit" };
        int currentEventType = static_cast<int>(event->getTime());
    	label = "Event Type##" + std::to_string(i);

        if (ImGui::Combo(label.c_str(), &currentEventType, eventTypes, IM_ARRAYSIZE(eventTypes)))
        {
            // Update the event type if changed
            event->setEventTime(static_cast<EventTime>(currentEventType));
        }

        if (event->getType() == EventType::TextBox)
        {
            std::shared_ptr<Event_TextBox> ep = std::dynamic_pointer_cast<Event_TextBox>(event);

        	char eventText[256]; 
            memset(eventText, 0, sizeof(eventText));

            strcpy_s(eventText, ep->getString().c_str());
        	std::string label = "Event text##" + std::to_string(i);

            ImGui::InputText(label.c_str(), eventText, IM_ARRAYSIZE(eventText));

            // Set the object's name if it's changed
            if (strcmp(eventText, ep->getString().c_str()) != 0) {
                ep->setString(eventText);
            }


        }

        else if (event->getType() == EventType::Inventory)
        {
            std::shared_ptr<Event_Inventory> ep = std::static_pointer_cast<Event_Inventory>(event);


            ImGui::Text("Item name: ");
            ImGui::SameLine();
            ImGui::Text(clean_string_for_display(ep->getItem().name).c_str());
            int n = ep->getItem().count;
            std::string label = "Value change#" + std::to_string(i);
            ImGui::InputInt(label.c_str(), &n);
            ep->setCount(n);
            InventorySelectionDisplay(event, i);
        }
        else if(event->getType()==EventType::SceneChange)
        {
            std::shared_ptr<Event_SceneChange> ep = std::dynamic_pointer_cast<Event_SceneChange>(event);

            // Concatenate the index to the label to make it unique
            std::string sceneName=ep->getSceneName();
            if(ImGui::Button("Select scene file"))
            {
                sceneName = select_scene();
                sceneName = sceneName + ".plip";
                ep->setSceneName(sceneName);
            }
            ImGui::Text("Scene: ");
            ImGui::Text(clean_string_for_display(sceneName).c_str());

        }

		//handle conditionals on the event
        label = "Is event conditional?#" + std::to_string(i);
        bool b=event->getIsConditional();
        ImGui::Checkbox(label.c_str(), &b);
        event->setConditional(b);
        if(event->getIsConditional())//let user set condition for event
        {
            ImGui::Text("Item name: ");
            ImGui::SameLine();
            ImGui::Text( clean_string_for_display(event->conditionalEventData.getItem().name).c_str());

            int n = event->conditionalEventData.getItem().count;
            std::string label = "Value must be more than or equal to#" + std::to_string(i);
            ImGui::InputInt(label.c_str(), &n);
            event->conditionalEventData.setCount(n);
            std::string popupID = "Conditional Inventory Item#" + std::to_string(i);
            if (ImGui::BeginMenu(popupID.c_str()))
            {
                for (const auto& itemName : manager_Inventory.getItemNames()) {
                    if (ImGui::MenuItem(itemName.c_str()))
                    {
                        //printf("Selected item: %s\n", itemName.c_str());

                        event->conditionalEventData.setItem(itemName, 0);
                    }
                }
                ImGui::EndMenu();
            }
        }

        if(ImGui::Button("Delete event"))
        {
            point->events[i].swap(point->events.back());
            point->events.pop_back();
        }
    }

}

inline void Window_ObjectSelection()
{
    ImGui::Begin("Object Selection");

    if (manager_Selection.currentObject!=nullptr) {
        std::shared_ptr<GameObject>& selectedObject = manager_Selection.currentObject;
    	ImGui::Text("Object name: ");
        ImGui::SameLine();
        // Input field to change the object's name
        char objectName[256]; // Assuming a maximum name length of 255 characters
        memset(objectName, 0, sizeof(objectName)); // Clear the memory
        strcpy_s(objectName, clean_string_for_display(selectedObject->name).c_str());
    	ImGui::InputText("##ObjectName", objectName, IM_ARRAYSIZE(objectName));

        // Set the object's name if it's changed
        if (strcmp(objectName, selectedObject->name.c_str()) != 0) 
        {
            selectedObject->name = clean_string_for_file(std::string(objectName));
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

        // Apply changes if modified
        if (position != selectedObject->transform.translation ||
            rotation != selectedObject->transform.rotation ||
            scale != selectedObject->transform.scale) {
            selectedObject->transform.translation = position;
            selectedObject->transform.rotation = rotation;
            selectedObject->transform.scale = scale;
            selectedObject->transform.CalculateModel();
        }


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

        if (selectedObject->objectType == ObjectType::type_MovementPoint)
        {
            std::shared_ptr<MovementPoint> point
                = std::dynamic_pointer_cast<MovementPoint>(selectedObject);

            //make all of these into functions, section them off to make easier to modularise in other functions
            EventEditor(point);

            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Spacing();
        	ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Spacing();
        	ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Spacing();
            EventSelection(point);
            

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
        setCamera(gameViewCamera);
    }
    if (ImGui::Button("Edit Mode"))
    {

        manager_EditorState.setState(state_EditorView);
        setCamera(editViewCamera);
        manager_GameManager.ResetGame();
        manager_Inventory.clearPlayerInventory();
    }
    // Assuming manager_EditorState.getState() returns a string
    if (manager_EditorState.getState() == state_EditorView)ImGui::Text("State: Editor");
    else if (manager_EditorState.getState() == state_GameView)ImGui::Text("State: Game");

    if(ImGui::Button("Edit from game view"))
    {
        editViewCamera->copy(gameViewCamera);
    }


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

inline void Window_Inventory()
{
    static char inputBuffer[256] = "";

    ImGui::Begin("Inventory");

    if (ImGui::InputText("Enter new item name: ", inputBuffer, sizeof(inputBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
    {
        std::string itemName = inputBuffer;
        itemName = clean_string_for_file(itemName);
        manager_Inventory.addItemToList(itemName);
        memset(inputBuffer, 0, sizeof(inputBuffer));
    }
    if(ImGui::Button("Add New Item"))
    {
        std::string itemName = inputBuffer;
        itemName = clean_string_for_file(itemName);
        manager_Inventory.addItemToList(itemName);
        memset(inputBuffer, 0, sizeof(inputBuffer));
    }
    std::vector<std::string> itemNames = manager_Inventory.getItemNames();
    for (auto name : itemNames)
    {
        name = clean_string_for_display(name);
        ImGui::Text(name.c_str());
    }
    if(ImGui::Button("Delete last item"))
    {
        manager_Inventory.removeLastItemFromList();
    }



    //demo button for adding to inventory
    if (ImGui::Selectable("Add Inventory Item", false, 0, ImVec2(ImGui::GetWindowContentRegionWidth(), 0))) {
        // Open the dropdown when the item is clicked
        ImGui::OpenPopup("InventoryItemsPopup");
    }

    // Dropdown for inventory items
    if (ImGui::BeginPopup("InventoryItemsPopup")) {
        for (const auto& item : manager_Inventory.getItemNames()) {
            // Selectable items in the dropdown
            if (ImGui::Selectable(item.c_str())) 
            {
                printf("Selected item: %s\n", item.c_str());
                //when a user selects, that event is generated and the item is selected along with a number box next to it
            }
        }
        ImGui::EndPopup();
    }


    ImGui::End();
}

inline void Window_Dialogue()
{
        ImGui::SetNextWindowPos(ImVec2(0, SCR_HEIGHT - 150));
        ImGui::SetNextWindowSize(ImVec2(SCR_WIDTH, 150));
        ImGui::Begin("Dialogue", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

        ImGui::Text(manager_UI.getText().c_str());
        if (ImGui::Button("Ok"))
        {
            manager_UI.movetonextdialogue();
            manager_UI.DisplayNextBox();
        }
        ImGui::End();
}

inline void Window_Debug()//window for debugging
{
    ImGui::Begin("Debug");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
        1000.0f / ImGui::GetIO().Framerate,
        ImGui::GetIO().Framerate);



    if(ImGui::Button("find files"))
    {
        //manager_UI.addTextBox("dummy tex");
        //manager_UI.DisplayNextBox();
        Manager_Scene.findPlipFile("example.plip");
    }

//#ifdef ENGINE
    if (ImGui::Button("find files"))
    {
        //create directory
    }
//#endif

	ImGui::End();
}

inline void Window_PlayerInventory()
{
    ImGui::Begin("Inventory");

    for(auto a:manager_Inventory.getCurrentInventory())
    {
        ImGui::Text(a.name.c_str());
        ImGui::SameLine();
        ImGui::Text("%d",a.count);
    }

    ImGui::End();
}

inline void Window_GameBuilder()
{
    ImGui::Begin("Game Builder");


    //game name
    ImGui::Text("Game name: ");
    ImGui::SameLine();
    // Input field to change the object's name
    char objectName[256]; // Assuming a maximum name length of 255 characters
    memset(objectName, 0, sizeof(objectName)); // Clear the memory
    strcpy_s(objectName, manager_GameBuilder.getGameName().c_str());
    ImGui::InputText("#GameName", objectName, IM_ARRAYSIZE(objectName));

    // Set the object's name if it's changed
    if (strcmp(objectName, manager_GameBuilder.getGameName().c_str()) != 0)
    {
        manager_GameBuilder.setGameName(std::string(objectName));
    }


    //game scenes
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

	ImGui::Text("Scene List");
    static std::string selectedName = "";
    ImGui::Text("Starting Scene: ");
    for (auto scene : manager_GameBuilder.getSceneList())
    {
        /*
        if (ImGui::Selectable(clean_string_for_display(scene).c_str(), selectedName == scene)) {
            selectedName = scene;
            //std::cout<< selectedName; 
        }*/
        ImGui::Text(scene.c_str());
    }

    //  add a scene button
    if(ImGui::Button("Add scene to game build"))//rewrite this
    {

        //manager_GameBuilder.addSceneToGame(select_scene());
        manager_GameBuilder.addSceneToGame(select_SceneFilePath());


        if(manager_GameBuilder.getSceneList().size()==1)
        {
            manager_GameBuilder.setStartScene(0);
        }
    }
    //  remove a scene
    if (ImGui::Button("Remove scene from game build"))
    {
        //manager_GameBuilder.removeSceneFromGame(select_scene());
        manager_GameBuilder.getSceneList().pop_back();
    }
	//starting scene mark



    //create game button
    if(ImGui::Button("Build Game"))
    {
        manager_GameBuilder.createSceneConfigurator();
    }


    ImGui::End();

	//bundle it all up in a single folder

}

void Window_LoadGame()
{
    ImGui::Begin("Start Menu");

    ImGui::Text("Load a configuration file with file ending .config");
    if(ImGui::Button("Load Game"))
    {
        std::wstring path = select_SceneFilePath();
        std::filesystem::path gamePath = std::filesystem::path(path);
        std::wstring a= read_config_file(path);
        Manager_Scene.ReadSceneFromFile(a+L".plip");

    }

    ImGui::End();
}