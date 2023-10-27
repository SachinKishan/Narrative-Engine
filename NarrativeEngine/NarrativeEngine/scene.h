#pragma once
#include <codecvt>
#include <string>
#include <vector>

#include <Windows.h>
#include <commdlg.h>
#include <fstream>

class GameObject
{
public:
	std::string name;
    
    GameObject(const std::string& name) : name(name) {}

    bool operator==(const GameObject& other) const {
        return name == other.name;
    }
};

class Scene
{
public:
	std::string sceneName;
	std::vector<GameObject> gameObjectList;


public:

	void AddToScene(const GameObject &object)
	{
		gameObjectList.push_back(object);
	}

	void RemoveFromScene(const GameObject& object)
	{
		gameObjectList.erase(
			std::remove(
				gameObjectList.begin(), gameObjectList.end(), object),
			gameObjectList.end());
	}
};

class SceneEditorManager
{
private:
	

public:
	
	bool sceneLoaded;
	Scene currentScene;
    std::wstring filepath;//file path of currently selected scene, is empty string if no scene is selected

    void resetCurrentSceneData()
    {
        currentScene.gameObjectList.clear();
        currentScene.sceneName.clear();
    }

    void setSceneName(std::wstring fileName)
    {
        if (&currentScene!=nullptr)
        {
            currentScene.sceneName = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(fileName);
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
                currentScene.AddToScene(GameObject(line));
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



void SaveScene(std::vector<GameObject> gameObjects)
{
	//file saving
    std::ofstream outFile(Manager_Scene.filepath , std::ios::out | std::ios::trunc);

    if (outFile.is_open()) {
        for (const GameObject &gameObject : gameObjects) {
            outFile << gameObject.name << '\n'; // Write each string on a new line
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