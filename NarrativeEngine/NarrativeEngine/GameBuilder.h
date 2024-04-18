#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "scene.h"

class GameBuilder
{
	std::vector<std::string> sceneList;
	std::vector<std::wstring> filePaths;

	int startingScene = 0;
	std::string gameName;

public:
	GameBuilder()
	{

	}
	void addSceneToGame(std::wstring filePath)
	{
		std::filesystem::path fsPath(filePath);
		std::string fileName = fsPath.stem().string();

		sceneList.push_back(fileName);
		filePaths.push_back((filePath));

	}
	void removeSceneFromGame(std::string sceneName)
	{
		for (int i = 0; i < sceneList.size(); i++)
		{
			if (sceneList[i]._Equal(sceneName))
			{
				sceneList[i].swap(sceneList.back());
				sceneList.pop_back();
				filePaths[i].swap(filePaths.back());
				filePaths.pop_back();
			}
		}
	}
	std::vector<std::string>& getSceneList()
	{
		return sceneList;
	}
	int getStartScene()
	{
		return startingScene;
	}
	void setStartScene(int newstartScene)
	{
		startingScene = newstartScene;
	}

	void createSceneConfigurator()
	{
		std::ofstream configFile("scene_config.txt");
		if (configFile.is_open())
		{
			// Write each scene name to the file
			for (const std::string& scene : sceneList)
			{
				configFile << scene << "\n";
			}
			configFile << "STARTING SCENE \n" << sceneList[startingScene] << "\n";
			configFile.close(); // Close the file
			std::cout << "Scene configurator file created successfully.\n";
		}
		else
		{
			std::cerr << "Unable to create scene configurator file.\n";
		}
		std::wstring gname = std::wstring(gameName.begin(), gameName.end());;
		copyFilesToDirectory(filePaths, L"BUILD"+gname, L"scene_config.txt");
	}
	void setGameName(const std::string& gamename)
	{
		gameName = gamename;
	}
	std::string getGameName() { return gameName; }
}manager_GameBuilder;