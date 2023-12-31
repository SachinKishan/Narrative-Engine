#pragma once
#include <codecvt>
#include <vector>

#include <Windows.h>
#include <commdlg.h>
#include <fstream>

#include "gameobject.h"


class Scene
{
public:
	std::string sceneName;
	std::vector<std::shared_ptr<GameObject>> gameObjectList;
    std::vector<std::shared_ptr<Light>> lightList;
    std::vector<std::shared_ptr<MovementPoint>> movementPointList;

    bool hasPlayer = false;
    std::shared_ptr<Player> player=nullptr;

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
    void SetPlayer(const std::shared_ptr<Player>& newPlayer)
    {
        player = newPlayer;
        hasPlayer = true;
    }
    void MovePlayer(glm::vec3 newPos)
	{
		if(hasPlayer)
		{
            player->changePosition(newPos);
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
        wprintf(L"Selected File Name: %s\n", fileName.c_str());

        setSceneName(fileName);
        if (inFile.is_open()) {
            std::string line;
            while (std::getline(inFile, line)) {
                

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
                    std::shared_ptr<Light> newGameObject= std::make_shared<Light>(name, transform, color);
                    currentScene.AddToScene(newGameObject);
                    currentScene.AddLight(newGameObject);

                }
                else if(type==ObjectType::type_Platform)
                {
                    std::shared_ptr<Platform> newGameObject= std::make_shared<Platform>(name, transform, color);
                    currentScene.AddToScene(newGameObject);
                }
                else if (type == ObjectType::type_Player)
                {
                    std::shared_ptr<Player> newGameObject = std::make_shared<Player>(name, transform, color);
                    currentScene.AddToScene(newGameObject);
                    currentScene.SetPlayer(newGameObject);
                }
                else if (type == ObjectType::type_MovementPoint)
                {
                    std::shared_ptr<MovementPoint> newGameObject = std::make_shared<MovementPoint>(name, transform, color);
                    currentScene.AddToScene(newGameObject);
                    currentScene.AddMovementPoint(newGameObject);
                }

                // Create a GameObject using the read data
                //newGameObject->transform = transform; // Assign the ObjectTransform

                std::cout << "Scene data has been read and stored\n";
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

            outFile << "\n";
        }
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
        //outFile << "This is your file content." << std::endl;
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