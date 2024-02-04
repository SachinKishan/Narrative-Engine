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



inline class GameManager
{
private:
	std::shared_ptr<MovementPoint> currentMovementPoint;
	std::shared_ptr<Player> player = nullptr;
	std::string currentText;

public:
	void SetPlayer(const std::shared_ptr<Player>& newPlayer)
	{
		player = newPlayer;
	}

	void MovePlayer(std::shared_ptr<MovementPoint> newPos)
	{
		RunEntryEvents(newPos);

		if(currentMovementPoint!=nullptr)
			RunExitEvents(currentMovementPoint);

		currentMovementPoint = newPos;
		player->changePosition(newPos->transform.translation);

		
	}
	void RunEntryEvents(const std::shared_ptr<MovementPoint>& point)
	{
		for (const auto& event : point->events)
		{
			if(event->getTime()==EventTime::Enter)
				event->doThing();
		}
	}
	void RunExitEvents(const std::shared_ptr<MovementPoint>& point)
	{
		for (const auto& event : point->events)
		{
			if (event->getTime() == EventTime::Exit)
				event->doThing();
		}
	}
	
}manager_GameManager;

class UI_Manager
{

private:
	//dialogue boxes
	int numberOfDialogueBoxes;
	std::vector<std::string> dialogueText;
	std::vector<bool> displayTextVector;


	std::string currentText;
	bool displayText=false;
public:
	UI_Manager()
	{
		
	}
	void addTextBox(const std::string newText)
	{
		dialogueText.push_back(newText);
		std::cout << dialogueText.size();
		displayTextVector.push_back(true);
	}
	void setText(const std::string newText)
	{
		currentText = newText;

	}
	std::string getText() { return currentText; }
	bool shouldDisplayText() { return displayText; }
	void setDisplayCondition(bool condition)
	{
		displayText = condition;
	}
	void DisplayNextBox()
	{
		if (dialogueText.empty()) { setDisplayCondition(false); std::cout << "empty"; }
		else
		{
			DisplayDialogue(dialogueText.back());
			std::cout << "\n\n" << dialogueText.size() << "\n\n";
			//displayTextVector.pop_back();
			
		}
	}
	void DisplayDialogue(const std::string newText)
	{
		setText(newText);
		setDisplayCondition(true);
	}
	void movetonextdialogue(){ dialogueText.pop_back(); }

}manager_UI;

class Event_TextBox :public Event
{
private:
	std::string stringtoprint;
public:
	Event_TextBox() { setEventType(EventType::Print); }
	Event_TextBox(std::string ename, EventType etype, EventTime etime, std::string s)
	{
		setEventType(EventType::Print);
		setEventName(ename);
		setEventTime(etime);
		 setString(s);
	}

	std::string getString() { return stringtoprint; }
	void setString(std::string s) { stringtoprint = s; }
	void doThing() override
	{

		manager_UI.addTextBox(stringtoprint);
		manager_UI.DisplayNextBox();

		//manager_UI.DisplayDialogue(stringtoprint);

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