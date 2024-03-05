#pragma once
#include <codecvt>
#include <vector>

#include <Windows.h>
#include <commdlg.h>
#include <fstream>
#include "gamehandler.h"
#include "gameobject.h"


std::string clean_string_for_file(std::string uncleanString)//used to ensure strings are safe to store in file
{

    if (uncleanString.empty())
    {
        uncleanString = "_";
    }
    else std::replace(uncleanString.begin(), uncleanString.end(), ' ', '_');

    return uncleanString;
}

std::string clean_string_for_display(std::string uncleanString)
{
    if (uncleanString.empty())
    {
        uncleanString = " ";
    }
    else std::replace(uncleanString.begin(), uncleanString.end(), '_', ' ');
    return uncleanString;
}

std::string convertWStringToString(const std::wstring& wstr)
{
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

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
        sceneLoaded = true;
        filepath = path;
        std::ifstream inFile(path);
        std::wstring fileName = path.substr(path.find_last_of(L"\\") + 1);
        int mode=0;//0 for reading objects, 1 for reading event data
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
                    bool conditional;
                    int c;
                    iss >> type;
                    iss >> objName;
                    iss >> eventName;
                	iss >> time;
                    iss >> c;
                    std::cout << std::endl << c;
                    conditional = c == 1;

                	std::shared_ptr<MovementPoint> point;
                    for (const auto& p : currentScene.movementPointList)
                    {
                        if (objName == p->name){point = p; break;}
                    }

                    if (type == EventType::Print)
                    {
                        std::shared_ptr<PrintNum_Event> printEvent = std::make_shared<PrintNum_Event>(eventName, (EventType)type, (EventTime)time);
                        printEvent->setConditional(conditional);
                        if (conditional)
                        {
                            std::string itemName;
                            int count;
                            iss >> itemName;
                            iss >> count;
                            printEvent->conditionalEventData.setItem(itemName,count);
                            printEvent->conditionalEventData.setCount(count);
                        }
                    	point->events.push_back(printEvent);
                    }
                    else if (type == EventType::TextBox)
                    {
                        std::string itemName;
                        int count;
                       
                        if (conditional)
                        {

                            iss >> itemName;
                            iss >> count;
                           
                        }
                        std::string s;
                        iss >> s;
                        //std::cout << s;
                        std::replace(s.begin(), s.end(), '_', ' ');
                        std::shared_ptr<Event_TextBox> printEvent = std::make_shared<Event_TextBox>(eventName, (EventType)type, (EventTime)time, s);
                        printEvent->setConditional(conditional);
                    	if (conditional)
                        {
                            printEvent->conditionalEventData.setItem(itemName,count);
                            printEvent->conditionalEventData.setCount(count);
                        }
                    	point->events.push_back(printEvent);
                    }
                    else if(type==EventType::Inventory)
                    {

                        std::string citemName;
                        std::string itemName;
                        int ccount;
                        int count;

                        if (conditional)
                        {

                            iss >> citemName;
                            iss >> ccount;

                        }
                        iss >> itemName;
                        std::replace(itemName.begin(), itemName.end(), '_', ' ');
                        iss >> count;
                        std::shared_ptr<Event_Inventory> inventoryEvent = std::make_shared<Event_Inventory>(eventName, (EventTime)time, itemName, count);
                        inventoryEvent->setConditional(conditional);
                    	if (conditional)
                        {
                            inventoryEvent->conditionalEventData.setItem(citemName,count);
                            inventoryEvent->conditionalEventData.setCount(ccount);
                        }

                    	point->events.push_back(inventoryEvent);
                    }

                    else if(type==EventType::SceneChange)
                    {
                        std::string itemName;
                        int count;

                        if (conditional)
                        {

                            iss >> itemName;
                            iss >> count;

                        }
                        std::string s;
                        iss >> s;
                        //std::cout << s;
                        std::replace(s.begin(), s.end(), '_', ' ');
                        std::shared_ptr<Event_SceneChange> printEvent = std::make_shared<Event_SceneChange>(eventName, (EventTime)time, s);
                        printEvent->setConditional(conditional);
                        if (conditional)
                        {
                            printEvent->conditionalEventData.setItem(itemName, count);
                            printEvent->conditionalEventData.setCount(count);
                        }
                        point->events.push_back(printEvent);
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

    void findPlipFile(std::string s)//find a scene based on its name, load it
	{
        std::filesystem::path currentPath = std::filesystem::current_path();
    	currentPath = currentPath.parent_path();
    	currentPath /= "Scenes";
    	for (const auto& entry : std::filesystem::directory_iterator(currentPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".plip") {
                if (s._Equal( entry.path().filename().string()))
                {
                    std::cout << entry.path();
                    ReadSceneFromFile(entry.path());
                    return;
                }
            }
        }
        std::cout << "file not found";

    }

    void changeScene(std::string sceneToChange)
    {
        findPlipFile(sceneToChange);
    }
}Manager_Scene;

void Event_SceneChange::doThing()
{
    std::cout << "Scene change called";
    Manager_Scene.changeScene(sceneFileName);
}

std::wstring select_file(const wchar_t* filter, const wchar_t* defaultExtension, DWORD flags)
{
    CoInitialize(NULL);

    wchar_t szFile[MAX_PATH] = L"";

    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filter;
    ofn.lpstrDefExt = defaultExtension;
    ofn.nFilterIndex = 1;
    ofn.Flags = flags;

    if (GetOpenFileName(&ofn)) {
        wprintf(L"Selected File: %s\n", szFile);
        CoUninitialize();
        return szFile;
    }
    else {
        wprintf(L"File selection canceled.\n");
        CoUninitialize();
        return L"";
    }
}

void LoadScene()
{
    std::wstring filePath = select_file(L"All Files (*.*)\0*.*\0", NULL, OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST);
    if (!filePath.empty()) {
        Manager_Scene.filepath = filePath;
        Manager_Scene.sceneLoaded = true;
    }
    Manager_Scene.ReadSceneFromFile(filePath.c_str());
}

std::string select_scene()
{
    std::wstring filePath = select_file(L"All Files (*.*)\0*.*\0", NULL, OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST);
    if (!filePath.empty()) {
        std::filesystem::path fsPath(filePath);
        std::string fileName = fsPath.stem().string();
        return fileName;
    }
    return "";
}

void CreateNewScene()
{
    std::wstring filePath = select_file(L"All Files (*.*)\0*.*\0", L"plip", OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT);
    if (!filePath.empty()) {
        Manager_Scene.sceneLoaded = true;
        Manager_Scene.resetCurrentSceneData();
        Manager_Scene.filepath = filePath;
        std::wstring fileName = filePath.substr(filePath.find_last_of(L"\\") + 1);
        Manager_Scene.setSceneName(fileName);
    }
    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        wprintf(L"Failed to create the file.\n");
    }
}


inline void SaveScene(std::vector<std::shared_ptr<GameObject>> gameObjects)
{
    std::ofstream outFile(Manager_Scene.filepath , std::ios::out | std::ios::trunc);

    if (outFile.is_open()) {
        outFile << "OBJECTS\n";
        for (const std::shared_ptr<GameObject> &gameObject : gameObjects) {
            outFile << gameObject->objectType << " ";
            outFile << gameObject->name<<" "; 
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
                std::string eventname =clean_string_for_file(event->getName());
	        	outFile << eventname <<" ";
                outFile <<event->getTime() <<" ";
                outFile << (event->getIsConditional() ? 1:0) << " ";
                if (event->getIsConditional())
                {
                    outFile << event->conditionalEventData.getItem().name << " ";
                    outFile << event->conditionalEventData.getItem().count << " ";
                }

                /*use this to test new event feature changes
                if (event->getType() == EventType::Print)
                {
	                //outFile<<event
                    const auto numEvent = static_cast<PrintNum_Event*>(event.get());
                }
                */

	        	if (event->getType() == EventType::TextBox)
                {

                    const auto printEvent = static_cast<Event_TextBox*>(event.get());
                    std::string s =clean_string_for_file(printEvent->getString());
                    outFile << s<<" ";
                }
                else if(event->getType()==EventType::Inventory)
                {
                    const auto inventoryEvent = static_cast<Event_Inventory*>(event.get());
                    std::string s =clean_string_for_file(inventoryEvent->getItem().name);
                    int count= inventoryEvent->getItem().count;
                    outFile << count;
                }
                else if (event->getType() == EventType::SceneChange)
                {
                    const auto inventoryEvent = static_cast<Event_SceneChange*>(event.get());
                    std::string s =clean_string_for_file(inventoryEvent->getSceneName());
                    outFile << s << " ";
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
            outFile << name <<"\n";


        outFile.close();
        std::wcout << "File '" << Manager_Scene.filepath << "' created and data written successfully." << std::endl;
    }
    else {
        std::cerr << "Failed to open the file for writing." << std::endl;
    }
}

