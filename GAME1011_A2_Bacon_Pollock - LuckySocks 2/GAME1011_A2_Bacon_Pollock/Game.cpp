#include "Game.h"
#include <iostream>
#include <ctime>

Game& Game::Instance()
{
	static Game instance;
	return instance;
}

int Game::Run()
{
	srand(static_cast<unsigned int>(time(NULL)));
	m_pInventory = new Object("Inventory");
	m_pInventory->ForceCloser();
	m_pFocus.push_back(new Object("You"));
	m_pFocus.back()->Enter();
	while (!m_pFocus.empty() && m_pFocus.back() != nullptr && m_pFocus.back()->GetLoaded() != false)
	{
		m_pFocus.back()->Display();
		m_pFocus.back()->ProcessInput();
	}
	std::cout << "Thank you for playing!" << std::endl;
	return 0;
}

Object* Game::CheckIfInstantiated(std::string name)
{
	if (name == "Inventory")
		return m_pInventory;

	Object* newObject = nullptr;

	// Check if is among the focal objects, either as previous, next, or in some other position
	// Check if it is in the inventory, either currently or as an emplaced child in another object
	newObject = m_pInventory->GetChildByName(name); // returns nullptr if not found
	
	for (unsigned int i = 0; i < m_pFocus.size(); i++)
	{
		if (m_pFocus[i] != nullptr && m_pFocus[i]->GetName() == name)
			newObject = m_pFocus[i];
	}

	return newObject;
}

void Game::ChangeFocus(Object* newFocus)
{
	m_pFocus.push_back(newFocus);
	m_pFocus.front()->Exit();
	m_pFocus.pop_front();
	if (newFocus != nullptr)
		m_pFocus.back()->Enter();
}
