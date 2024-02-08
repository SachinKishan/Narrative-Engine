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

struct Item
{
public:
	std::string name;
	int count=1;
};

class Inventory
{
private:
	std::vector<std::string> itemNames;
	std::vector<Item> itemsHeldCurrently;
public:
	std::vector<std::string>& getItemNames() { return itemNames; }
	std::vector<Item>& getCurrentInventory() { return itemsHeldCurrently; }
	void clearData()
	{
		itemsHeldCurrently.clear();
		itemNames.clear();
	}
	void addItemToList(const std::string i)
	{
		itemNames.push_back(i);
	}
	void removeLastItemFromList()
	{
		if (!itemNames.empty())
		{
			itemNames.pop_back();
		}
	}
	void addItemToInventory(Item i)
	{
		for (auto it = itemsHeldCurrently.begin(); it != itemsHeldCurrently.end(); ++it)
		{
			if (it->name == i.name)
			{
				it->count += i.count;
				if (it->count == 0)
				{
					itemsHeldCurrently.erase(it);
				}
				break; 
			}
		}
	}
	void removeItemFromInventory(Item i)
	{
		for (auto it = itemsHeldCurrently.begin(); it != itemsHeldCurrently.end(); ++it)
		{
			if (it->name == i.name)
			{
				it->count -= i.count;
				if (it->count <= 0)
				{
					itemsHeldCurrently.erase(it);
				}
				break;
			}
		}
	}
}manager_Inventory;


class UI_Manager
{

private:
	//dialogue boxes
	int numberOfDialogueBoxes;
	std::vector<std::string> dialogueText;
	std::vector<bool> displayTextVector;


	std::string currentText;
	bool displayText = false;
public:
	UI_Manager()
	{

	}
	void addTextBox(const std::string newText)
	{
		dialogueText.push_back(newText);
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
		}
	}
	void DisplayDialogue(const std::string newText)
	{
		setText(newText);
		setDisplayCondition(true);
	}
	void movetonextdialogue() { dialogueText.pop_back(); }
	void ClearDialogue()
	{
		dialogueText.clear();
		setDisplayCondition(false);
	}

}manager_UI;

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



class Event_TextBox :public Event
{
private:
	std::string stringtoprint;
public:
	Event_TextBox() { setEventType(EventType::TextBox); }
	Event_TextBox(std::string ename, EventType etype, EventTime etime, std::string s)
	{
		setEventType(EventType::TextBox);
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
		setEventType(EventType::Print);
	}
	void doThing() override
	{
		std::cout << 10;
	}
};

class Event_Inventory :public Event
{
private:
	Item item;

public:
	Event_Inventory()
	{
		item.name = "";
		item.count = 0;
		setEventType(EventType::Inventory);
	}
	Event_Inventory(std::string ename, EventTime etime, std::string name, int count)
	{
		setEventName(ename);
		setEventTime(etime);
		item.name = name;
		item.count = count;
		setEventType(EventType::Inventory);
	}
	void setItem(std::string itemName, int n)
	{
		item.name = itemName;
		item.count = n;
	}
	void setCount(int n) { item.count = n; }
	Item getItem() { return item; }
	void doThing() override
	{
		manager_Inventory.addItemToInventory(item);
	}
};