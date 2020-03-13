#include "Object.h"
#include <iostream>
#include <fstream>

void Object::StepInto()
{
	std::cout << m_sScript << std::endl;
}

void Object::StepBack()
{
}

void Object::RemoveChildToInventory()
{
}

void Object::AddChildFromInventory()
{
}

bool Object::CheckInventory()
{
	return false;
}

bool Object::hasChild(std::string name)
{
	return false;
}

void Object::m_load()
{
	// Initialize the variables
	std::fstream file;
	std::vector<std::string> children;

	// Open the text file for the object
	// Read the script and save it to the appropriate variable
	// Read the names for each child and instantiate objects for each one
	// Close the file

	// Check the inventory for the presence of each child
	for (unsigned int childIndex = 0; childIndex < children.size(); childIndex++)
	{
		// Instantiate objects for each one not found
		if (!m_pInventory->hasChild("ChildName"))
		{

		}
	}
}

void Object::m_printScript()
{
}

void Object::m_printChildren()
{
	for (Object* child : m_pChildren)
	{
		std::cout << child->getName();
	}
}
