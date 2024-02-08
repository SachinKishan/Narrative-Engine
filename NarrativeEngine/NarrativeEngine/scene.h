#pragma once
#include <codecvt>
#include <vector>

#include <Windows.h>
#include <commdlg.h>
#include <fstream>
#include "gamehandler.h"
#include "gameobject.h"


class Scene
{
public:
	std::string sceneName;
	std::vector<std::shared_ptr<GameObject>> gameObjectList;
    std::vector<std::shared_ptr<Light>> lightList;
    std::vector<std::shared_ptr<MovementPoint>> movementPointList;
    std::shared_ptr<MovementPoint> currentMovementPoint;
    bool hasPlayer = false;

	void AddToScene(const std::shared_ptr<GameObject> &object)
	{
		gameObjectList.push_back(object);
	}
    void AddLight(const std::shared_ptr<Light> &light)
	{
        lightList.push_back(light);
	}
    void AddMovementPoint (const std::shared_ptr<MovementPoint>& point)
    {
		movementPointList.push_back(point);
    }
	void RemoveFromScene(const std::shared_ptr<GameObject>& object)
	{
        if (object->objectType == ObjectType::type_Player)hasPlayer = false;

		gameObjectList.erase(
			std::remove(
				gameObjectList.begin(), gameObjectList.end(), object),
			gameObjectList.end());
	}
    void SceneSetPlayer(const std::shared_ptr<Player>& newPlayer)
    {
        if (!hasPlayer)
        {
            manager_GameManager.SetPlayer(newPlayer);
            hasPlayer = true;
        }
    }
    
};
std::string convertWStringToString(const std::wstring& wstr)
{
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &strTo[0], size_needed, NULL, NULL);
    return strTo;
}
inline class SceneEditorManager
{
private:
	
public:
	
	bool sceneLoaded;
	Scene currentScene;
    std::wstring filepath;//file path of currently selected scene, is empty string if no scene is selected

    void resetCurrentSceneData()
    {
        currentScene.gameObjectList.clear();
        currentScene.lightList.clear();
        currentScene.sceneName.clear();
        currentScene.movementPointList.clear();
        currentScene.hasPlayer = false;
        manager_Inventory.clearData();
    }

    void setSceneName(std::wstring fileName)
    {
        if (&currentScene!=nullptr)
        {
            
            currentScene.sceneName = convertWStringToString(fileName);
        }
        else
        {
            std::cout << "No scene has been set!";
        }
    }

    void ReadSceneFromFile(std::wstring path)
    {
        resetCurrentSceneData();
        filepath = path;
        std::ifstream inFile(path);
        std::wstring fileName = path.substr(path.find_last_of(L"\\") + 1);
        int mode=0;//0 for reading objects, 1 for reading event data
        //wprintf(L"Selected File Name: %s\n", fileName.c_str());

        setSceneName(fileName);
        if (inFile.is_open()) {
            std::string line;
            while (std::getline(inFile, line)) {
                if (line._Equal("OBJECTS"))mode = 0;
                else if (line._Equal("EVENTS"))mode = 1;
                else if (line._Equal("CAMERA"))mode = 2;
                else if (line._Equal("INVENTORY"))mode = 3;
                else if (mode==0)
                {

                    std::istringstream iss(line);
                    std::string name;
                    glm::vec3 translation, rotation, scale;
                    glm::vec4 color;
                    int type;
                    iss >> type;
                    iss >> name
                        >> translation.x >> translation.y >> translation.z
                        >> rotation.x >> rotation.y >> rotation.z
                        >> scale.x >> scale.y >> scale.z
                        >> color.r >> color.g >> color.b;

                    // Create ObjectTransform based on parsed data
                    ObjectTransform transform(translation, rotation, scale);
                    if (type == ObjectType::type_Light)
                    {
                        float intensity;
                        iss >> intensity;
                        std::shared_ptr<Light> newGameObject = std::make_shared<Light>(name, transform, color);
                        newGameObject->setIntensity(intensity);
                        currentScene.AddToScene(newGameObject);
                        currentScene.AddLight(newGameObject);

                    }
                    else if (type == ObjectType::type_Platform)
                    {
                        std::shared_ptr<Platform> newGameObject = std::make_shared<Platform>(name, transform, color);
                        currentScene.AddToScene(newGameObject);
                    }
                    else if (type == ObjectType::type_Player)
                    {
                        std::shared_ptr<Player> newGameObject = std::make_shared<Player>(name, transform, color);
                        currentScene.AddToScene(newGameObject);
                        currentScene.SceneSetPlayer(newGameObject);
                    }
                    else if (type == ObjectType::type_MovementPoint)
                    {
                        std::shared_ptr<MovementPoint> newGameObject = std::make_shared<MovementPoint>(name, transform, color);
                        currentScene.AddToScene(newGameObject);
                        currentScene.AddMovementPoint(newGameObject);
                    }
                }
                else if(mode==1)
                {
                    std::istringstream iss(line);
                	int type;
                    int time;
                    std::string eventName;
                    std::string objName;
                    iss >> type;
                    iss >> objName;
                    iss >> eventName;
                	iss >> time;
                    
                	std::shared_ptr<MovementPoint> point;
                    for (const auto& p : currentScene.movementPointList)
                    {
                        if (objName == p->name){point = p; break;}
                    }

                    if (type == EventType::Print)
                    {
                        std::shared_ptr<PrintNum_Event> printEvent = std::make_shared<PrintNum_Event>(eventName, (EventType)type, (EventTime)time);
                        point->events.push_back(printEvent);
                    }
                    else if (type == EventType::TextBox)
                    {
                        std::string s;
                        iss >> s;
                        std::cout << s;
                        std::replace(s.begin(), s.end(), '_', ' ');
                        std::shared_ptr<Event_TextBox> printEvent = std::make_shared<Event_TextBox>(eventName, (EventType)type, (EventTime)time, s);
                        point->events.push_back(printEvent);
                    }
                    else if(type==EventType::Inventory)
                    {
                        std::string itemName;
                        int count;
                        iss >> itemName;
                        std::replace(itemName.begin(), itemName.end(), '_', ' ');
                        iss >> count;
                        std::shared_ptr<Event_Inventory> inventoryEvent = std::make_shared<Event_Inventory>(eventName, (EventTime)time, itemName, count);
                        point->events.push_back(inventoryEvent);

                    }

                    

                }
                else if(mode==2)
                {
                    std::istringstream iss(line);
                    glm::vec3 pos;
                    glm::vec3 up;
                    float pitch;
                    float yaw;

                    iss >> pos.x;
                    iss >> pos.y;
                    iss >> pos.z;
                    iss >> up.x;
                    iss >> up.y;
                    iss >> up.z;
                    iss >> pitch;
                    iss >> yaw;

                    gameViewCamera->setCamera(pos,up,yaw,pitch);
                    editViewCamera->copy(gameViewCamera);
                }
                else if(mode==3)
                {
	                std::istringstream iss(line);
                    std::string name;
                    iss >> name;
                    manager_Inventory.addItemToList(name);
                }
            }
            inFile.close();
        }
        else {
            std::cerr << "Failed to open the file for reading." << std::endl;
        }

    }


}Manager_Scene;




void LoadScene()//loads file into the scene manager
{
	
    // Initialize COM for Common Item Dialog
    CoInitialize(NULL);

    // Create a buffer to store the selected file's path
    wchar_t szFile[MAX_PATH] = L"";

    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = NULL; // The parent window; set to NULL for no parent.
    ofn.lpstrFile = szFile; // The buffer to store the selected file's path.
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"All Files (*.*)\0*.*\0"; // Filter for file types.
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Open the file dialog
    if (GetOpenFileName(&ofn)) {
        // The selected file's path is now in szFile.
        // You can use this path for further processing.

        // For example, print the selected file's path
        wprintf(L"Selected File: %s\n", szFile);
        Manager_Scene.filepath = szFile;
        Manager_Scene.sceneLoaded = true;

    	//std::wcout<<Manager_Scene.filepath;
    }
    else {
        // The user canceled the file dialog.
        wprintf(L"File selection canceled.\n");
    }

    // Release COM resources
    CoUninitialize();

    Manager_Scene.ReadSceneFromFile(szFile);


}



inline void SaveScene(std::vector<std::shared_ptr<GameObject>> gameObjects)
{
	//file saving
    std::ofstream outFile(Manager_Scene.filepath , std::ios::out | std::ios::trunc);

    if (outFile.is_open()) {
        outFile << "OBJECTS\n";
        for (const std::shared_ptr<GameObject> &gameObject : gameObjects) {
            outFile << gameObject->objectType << " ";
            outFile << gameObject->name<<" "; // Write each string on a new line
            outFile << gameObject->transform.translation.x << " ";
            outFile << gameObject->transform.translation.y << " ";
            outFile << gameObject->transform.translation.z << " ";
            outFile << gameObject->transform.rotation.x << " ";
            outFile << gameObject->transform.rotation.y << " ";
            outFile << gameObject->transform.rotation.z << " ";
            outFile << gameObject->transform.scale.x << " ";
            outFile << gameObject->transform.scale.y << " ";
            outFile << gameObject->transform.scale.z<< " ";
            outFile << gameObject->renderData->material.color.r<< " ";
            outFile << gameObject->renderData->material.color.g<< " ";
            outFile << gameObject->renderData->material.color.b<< " ";
            if(gameObject->objectType==type_Light)
            {
                //TODO
                //CONVERT OBJECT TO LIGHT OBJECT, GET THE LIGHT INTENSITY, SEND IT TO THE FILE
                const auto light = static_cast<Light*>(gameObject.get());
                outFile << light->getIntensity();
            }
            outFile << "\n";
        }

        //write events to the file
        outFile << "EVENTS\n";
        for (const auto& point : Manager_Scene.currentScene.movementPointList)
        {
	        for (const auto& event : point->events)
	        {
                outFile <<event->getType() <<" ";
                outFile << point->name<<" ";
                std::string eventname = event->getName();
                if (eventname.empty())
                {
                    eventname = "_";
                }
                else std::replace(eventname.begin(), eventname.end(), ' ', '_');
	        	outFile << eventname <<" ";
                outFile <<event->getTime() <<" ";

                if (event->getType() == EventType::Print)
                {
	                //outFile<<event
                    const auto numEvent = static_cast<PrintNum_Event*>(event.get());
                }
                else if (event->getType() == EventType::TextBox)
                {

                    const auto printEvent = static_cast<Event_TextBox*>(event.get());
                    std::string s = printEvent->getString();
                    if (s.empty())
                    {
                        s = "_";
                    }
                    else std::replace(s.begin(), s.end(), ' ', '_');
                    outFile << s<<" ";
                }
                else if(event->getType()==EventType::Inventory)
                {
                    const auto inventoryEvent = static_cast<Event_Inventory*>(event.get());
                    std::string s = inventoryEvent->getItem().name;
                    if (s.empty())
                    {
                        s = "_";
                    }
                    else std::replace(s.begin(), s.end(), ' ', '_');
                    outFile << s << " ";

                    int count= inventoryEvent->getItem().count;
                    outFile << count;
                }


                outFile << "\n";

	        }
        }

        //write camera data to the file
    	outFile << "CAMERA\n";
        outFile << gameViewCamera->Position.x<<" ";
        outFile << gameViewCamera->Position.y<<" ";
        outFile << gameViewCamera->Position.z<<" ";
        outFile << gameViewCamera->Up.x<<" ";
        outFile << gameViewCamera->Up.y<<" ";
        outFile << gameViewCamera->Up.z<<" ";
        outFile << gameViewCamera->Pitch<<" ";
        outFile << gameViewCamera->Yaw<<" ";

        outFile << "\n";

        //inventory
        outFile << "INVENTORY\n";

        for (auto name : manager_Inventory.getItemNames())
            outFile << name <<" ";


        outFile.close();
        std::wcout << "File '" << Manager_Scene.filepath << "' created and data written successfully." << std::endl;
    }
    else {
        std::cerr << "Failed to open the file for writing." << std::endl;
    }
}

void CreateNewScene()
{
    CoInitialize(NULL);

    // Initialize the OPENFILENAME structure

    OPENFILENAME ofn;
    wchar_t szFile[MAX_PATH] = L"";
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = NULL; // The parent window; set to NULL for no parent.
    ofn.lpstrFile = szFile; // Buffer to store the selected file's path.
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"All Files (*.*)\0*.*\0"; // Filter for file types.
    ofn.nFilterIndex = 1;
    ofn.lpstrDefExt = L"plip"; // Default file extension.
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

    // Open the Save File dialog
    if (GetSaveFileName(&ofn)) {
        // The user selected or created a file. The file path is in szFile.
        std::wstring selectedFilePath(szFile);
        wprintf(L"Selected File: %s\n", selectedFilePath.c_str());
    }
    else {
        // The user canceled the dialog.
        wprintf(L"File selection canceled.\n");
    }
    // Release COM resources
    CoUninitialize();

    // Create the file
    std::ofstream outFile(szFile);

    // Check if the file is opened successfully
    if (outFile.is_open()) {
        // Write data to the file if needed
        outFile.close();

        wprintf(L"File '%s' with extension '%s' created successfully.\n", szFile, ".plip");
        Manager_Scene.sceneLoaded = true;

    }
    else {
        wprintf(L"Failed to create the file.\n");
    }


    Manager_Scene.resetCurrentSceneData();
    Manager_Scene.filepath = szFile;
    // Example file path
    std::wstring filePath = szFile;
    std::wstring fileName = filePath.substr(filePath.find_last_of(L"\\") + 1);
    wprintf(L"Selected File Name: %s\n", fileName.c_str());

    Manager_Scene.setSceneName(fileName);
}