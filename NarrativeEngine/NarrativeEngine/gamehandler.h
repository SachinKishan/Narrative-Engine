#pragma once
#include <iostream>
#include "event.h"

#include "gameobject.h"



inline class GameManager
{
private:
	glm::vec3 playerStartPoint;
	std::shared_ptr<MovementPoint> currentMovementPoint;
	std::shared_ptr<Player> player = nullptr;
	std::string currentText;

public:

	void ResetGame()
	{
		if(player)
			player->changePosition(playerStartPoint);
		currentMovementPoint = nullptr;
		manager_UI.ClearDialogue();
	}

	void SetPlayer(const std::shared_ptr<Player>& newPlayer)
	{
		player = newPlayer;
		playerStartPoint = player->transform.translation;
	}

	void MovePlayer(std::shared_ptr<MovementPoint> newPos)
	{
		currentMovementPoint = newPos;
		player->changePosition(newPos->transform.translation);
		RunEntryEvents(newPos);


		if(currentMovementPoint!=nullptr)
			RunExitEvents(currentMovementPoint);


		
	}
	void RunEntryEvents(const std::shared_ptr<MovementPoint>& point)
	{
		for (const auto& event : point->events)
		{
			if(event->getTime()==EventTime::Enter)
				event->runEvent();
		}
	}
	void RunExitEvents(const std::shared_ptr<MovementPoint>& point)
	{
		for (const auto& event : point->events)
		{
			if (event->getTime() == EventTime::Exit)
				event->runEvent();
		}
	}
	
}manager_GameManager;


class GameBuilder
{
	std::vector<std::string> sceneList;
	int startingscene=0;
public:
	GameBuilder()
	{
		
	}
	void addSceneToGame(std::string sceneName)
	{
		sceneList.push_back(sceneName);
	}
	void removeSceneFromGame(std::string sceneName)
	{
		for(int i=0;i<sceneList.size();i++)
		{
			if (sceneList[i]._Equal(sceneName))
			{
				sceneList[i].swap(sceneList.back());
				sceneName.pop_back();
			}
		}
	}
	int getStartScene()
	{
		return startingscene;
	}
	void setStartScene(int newstartScene)
	{
		startingscene = newstartScene;
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
			configFile << "Starting Scene: " << startingscene << "\n";
			configFile.close(); // Close the file
			std::cout << "Scene configurator file created successfully.\n";
		}
		else
		{
			std::cerr << "Unable to create scene configurator file.\n";
		}
	}
}manager_GameBuilder;