#pragma once
#include <iostream>
#include <string>
#include <vector>

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
