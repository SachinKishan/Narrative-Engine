#pragma once
#include <string>

#include "GUIManagement.h"
#include "inventory.h"

enum EventTime
{
    Enter,
    Exit
};
enum EventType
{
    TextBox,
    Print,
    Inventory,
    SceneChange,
	CloseApplication,
	ObjectDisappear
};

class ConditionalEventData
{
private:
    Item item;
public:
    ConditionalEventData()
    {
        item.name = "";
        item.count = 0;
    }
    ConditionalEventData(std::string name, int count)
    {
        setItem(name, count);
    }
    void setItem(std::string itemName, int n)
    {
        item.name = itemName;
        item.count = n;
    }
    void setCount(int n) { item.count = n; }
    Item getItem() { return item; }
};


class Event
{
private:
    EventTime eventTime = Enter;
    std::string eventName;
    EventType eventType;

    //allow user to create conditions if they want
    bool isConditional = false;

public:
    ConditionalEventData conditionalEventData;

    Event() {}
    Event(std::string ename, EventType etype, EventTime etime)
    {
        setEventType(etype);
        setEventName(ename);
        setEventTime(etime);
    }
    void setEventTime(EventTime type)
    {
        eventTime = type;
    }
    void setEventName(std::string name)
    {
        eventName = name;
    }
    void setEventType(EventType type)
    {
        eventType = type;
    }
    virtual void doThing() {}
    void runEvent()
    {
        if (isConditional)
        {
            conditionalDoThing();
        }
        else
            doThing();
    }

	void conditionalDoThing()
	{
		if (manager_Inventory.checkForItem(conditionalEventData.getItem().name, conditionalEventData.getItem().count))
		{
			doThing();
		}
	}
    std::string getName() { return eventName; }
    EventTime getTime() { return eventTime; }
    EventType getType() { return eventType; }
    void setConditional(bool con) { isConditional = con; }
    bool getIsConditional() { return isConditional; }
};



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

bool shouldRun = true;
class Event_CloseApplication :public Event
{
public:
	Event_CloseApplication()
	{
		setEventType(EventType::CloseApplication);
	}
	void doThing() override
	{
		shouldRun = false;
	}
};

class Event_ObjectDisappear:public Event
{
private:
	// currentObject;
public:
	Event_ObjectDisappear()
	{
		setEventType(EventType::ObjectDisappear);
	}
	void doThing() override;

};