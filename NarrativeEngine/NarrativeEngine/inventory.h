#pragma once
#include <string>

struct Item
{
public:
    std::string name;
    int count = 1;
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
	void clearPlayerInventory()
	{
		itemsHeldCurrently.clear();
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

		for (auto& item : itemsHeldCurrently)
		{
			if (item.name == i.name)
			{
				item.count += i.count;

				if (item.count < 0)
				{
					item.count = 0;
				}
				return;
			}
		}
		itemsHeldCurrently.push_back(i);
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
	bool checkForItem(std::string itemname, int count)
	{
		for (auto& item : itemsHeldCurrently)
		{
			if (item.name == itemname && item.count >= count)
			{
				return true;
			}
		}
		return false;
	}

} manager_Inventory;