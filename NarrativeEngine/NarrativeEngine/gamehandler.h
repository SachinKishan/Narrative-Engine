#pragma once
#include <iostream>
#include "gameobject.h"
//keep track of player movement
//allow user to add events here
//events are added when a certain thing happens from a movement point
//events for now will just be printing

//we can just keep track of movement point using the current scene list- use manager


///
///create an array of event objects
///these are all pointers, these are all inheritables
///they all have basic parent, but deviate in their virtual function that does a thing
///when a player chooses to add an event to a movement point, they add an event object
///when the call happens, the idea is that we find what movement point the player is at, find the vector of events its associated to
///parse through each of the event objects and make their function calls
///

class Event_Print:public Event
{
private:
	std::string stringtoprint;
public:
	Event_Print() { setEventType(EventType::Print); }
	Event_Print(std::string ename, EventType etype, EventTime etime, std::string s)
	{
		setEventType(EventType::Print);
		setEventName(ename);
		setEventTime(etime);
		stringtoprint = s;
	}
	
	std::string getString() { return stringtoprint; }
	void setString(std::string s) { stringtoprint = s; }
	void doThing() override
	{
		std::cout << stringtoprint;
	}
};

class PrintNum_Event :public Event
{
private:
	
public:
	PrintNum_Event() = default;
	PrintNum_Event(std::string ename, EventType etype, EventTime etime)
	{
		setEventType(etype);
		setEventName(ename);
		setEventTime(etime);
		setEventType(EventType::TextBox);
	}
	void doThing() override
	{
		std::cout << 10;
	}
};

inline class GameManager
{
private:
	std::shared_ptr<MovementPoint> currentMovementPoint;
	std::shared_ptr<Player> player = nullptr;


public:
	void SetPlayer(const std::shared_ptr<Player>& newPlayer)
	{
		player = newPlayer;
	}

	void MovePlayer(std::shared_ptr<MovementPoint> newPos)
	{
			currentMovementPoint = newPos;
			player->changePosition(newPos->transform.translation);
		
	}

	void RunEvents(const std::shared_ptr<MovementPoint>& point)
	{
		for (const auto& event : point->events)
		{
			event->doThing();
		}
	}
}manager_GameManager;