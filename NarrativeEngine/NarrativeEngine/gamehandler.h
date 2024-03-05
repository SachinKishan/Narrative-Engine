#pragma once
#include <iostream>
#include "gameobject.h"


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


class Event_SceneChange : public Event {
	// Scene to change
	std::string sceneFileName = "_____";

public:
	Event_SceneChange() { setEventType(EventType::SceneChange); }

	Event_SceneChange(std::string ename, EventTime etime, std::string s)
	{
		setEventType(EventType::SceneChange);
		setEventName(ename);
		setEventTime(etime);
		setSceneName(s);
	}
	void setSceneName(std::string newSceneName) {
		sceneFileName = newSceneName;
	}

	std::string getSceneName() {
		return sceneFileName;
	}

	void doThing() override; // Declaration of the overridden virtual function
};



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