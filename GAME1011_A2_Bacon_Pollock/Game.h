#pragma once
#ifndef _GAME_
#define _GAME_
#include "Object.h"
#include <deque>

class Game
{
private:
	Object* m_pInventory;
	std::deque<Object*> m_pFocus;
public:
	static Game& Instance();
	int Run();
	Object* GetInventory() { return m_pInventory; }
	Object* CheckIfInstantiated(std::string name);
	void ChangeFocus(Object* newFocus);
};

#endif // !_GAME_