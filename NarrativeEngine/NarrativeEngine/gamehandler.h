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

