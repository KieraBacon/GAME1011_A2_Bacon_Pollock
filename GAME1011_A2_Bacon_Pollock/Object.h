#pragma once
#ifndef _OBJECT_
#define _OBJECT_
#include <string>
#include <vector>

class Object
{
private: // Private member variables
	std::string m_sName;
	std::string m_sScript;
	bool m_bAttached;
	Object* m_pParent;
	std::vector<Object*> m_pChildren;
	Object* m_pInventory;
public: // Public member functions
	// System methods
	Object() {}
	~Object() {}

	// Gameplay methods
	void StepInto(); // Changes the focal object for the game to this one by loading its data and printing that data to the console.
	void StepBack();
	void RemoveChildToInventory();
	void AddChildFromInventory();
	bool CheckInventory();

	// Accessor methods
	std::string getName() { return m_sName; }
	std::string getScript() { return m_sScript; }
	bool getAttached() { return m_bAttached; }
	Object* getParent() { return m_pParent; }
	std::vector<Object*>& getChildren() { return m_pChildren; }
	bool hasChild(std::string name);

	// Mutator methods
	void setAttached(bool attached) { m_bAttached = attached; }

private: // Private member functions
	void m_load(); // Loads the script of the object and the names of all children, then instiantiates objects for each child.
	void m_printScript(); // Prints the script for the object to the console.
	void m_printChildren(); // Checks the inventory for the presence of each child, then prints each one not found to the console.
};

#endif // !_OBJECT_