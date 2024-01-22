#pragma once
#include <iostream>
#include "gameobject.h"
//keep track of player movement
//allow user to add events here
//events are added when a certain thing happens from a movement point
//events for now will just be printing

//we can just keep track of movement point using the current scene list- use manager

/*
class Event
{
	
};

class PrintEvent: public Event
{
	
};

class OtherEvent:public Event
{
	
};

typedef void (*MovementEvent)(Event);

class GameManager
{
public:
	std::vector<std::pair<std::string, std::vector<MovementEvent>>> events;

	void CheckForEvent(const std::shared_ptr<MovementPoint> point)
	{
		std::cout << "Check called";
		for(auto event:events)
		{
			if (point->name == event.first)
			{
				for(auto func:event.second)
				{
					func();
				}
			}
		}
		
	}
	void addEvent()
	{
		
	}
}Manager_game;

///Different types of events
///
///Text box
///Change scene
///Move player elsewhere
///
///Exit game
///
///
///
///
/*
void CreateEventPrint(std::string message)
{
	Event a;

	// Create a lambda function that matches the signature of MovementEvent
	MovementEvent multiply = [a](Event capturedA) -> void {
		// Use 'a' within the lambda
		std::cout << "Captured Event instance in the lambda" << std::endl;
	};

}
*/

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
	Event_Print(std::string s)
	{
		stringtoprint = s;
	}
	void doThing() override
	{
		std::cout << stringtoprint;
	}
};



class PrintNum_Event :public Event
{
private:
	
public:
	PrintNum_Event()
	{
		
	}
	void doThing() override
	{
		std::cout << 10;
	}
};

class GameManager
{
public:
	std::vector<std::pair<std::shared_ptr<MovementPoint>, Event>> events;

	void AddEvent(std::shared_ptr<MovementPoint> point, Event event)
	{
		
	}

};