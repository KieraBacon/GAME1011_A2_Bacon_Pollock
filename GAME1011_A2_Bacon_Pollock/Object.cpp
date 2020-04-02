#include "Object.h"
#include "Game.h"
#include <iostream>

Object::Object(std::string name, Object* parent) : m_sName(name), m_pParent(parent), m_bLoaded(false), m_bCloser(false) {}

Object::~Object() {}

// Start of public methods
void Object::Enter()
{
	Load();
	for (std::list<Object*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); it++)
		(*it)->Load();
}

void Object::Load()
{
	if(!m_bLoaded)
		m_LoadFromFile();
}

/* When exiting an object by changing the focus to another object
the game needs to unload that object and all of its children except the new focus.*/ 
void Object::Exit()
{
	m_pParent->Clean();
	for (std::list<Object*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); it++)
	{
		(*it)->Clean();
	}
}

/* When cleaning an object, the game needs to delete its parent and all of its children
except if they are the inventory or they are in the inventory or they are the new focus. */
void Object::Clean()
{
	// Perhaps it would be more efficient to compile the instantiated list here and compare names all at once, instead of going throughthe entire list by string value each time?
	if (Game::Instance().CheckIfInstantiated(this->GetName()) == nullptr)
	{
		m_sFarScript.clear();
		m_sNearScript.clear();

		if (m_pParent != nullptr && Game::Instance().CheckIfInstantiated(m_pParent->GetName()) == nullptr)
		{
			delete m_pParent;
			m_pParent = nullptr;
		}
		while (!m_pChildren.empty())
		{
			if (Game::Instance().CheckIfInstantiated(m_pChildren.back()->GetName()) == nullptr)
			{
				delete m_pChildren.back();
				m_pChildren.back() = nullptr;
			}
			m_pChildren.pop_back();
		}
		m_bLoaded = false;
	}
}

void Object::Display()
{
	m_PrintDividingLine(GetName());
	if (!m_bCloser)
	{
		m_PrintFarScript();
	}
	else
	{
		m_PrintNearScript();
		m_PrintChildren();
	}
	m_PrintDividingLine();
}

// Start of input methods
void Object::ProcessInput()
{
	const unsigned int EXITSTRLEN = 9;
	const std::string EXITSTR[EXITSTRLEN] = {
		"exit the game",
		"exit game",
		"exit",
		"quit the game",
		"quit game",
		"quit",
		"leave the game",
		"leave game",
		"leave"	};
	const unsigned int INVSTRLEN = 6;
	const std::string INVSTR[INVSTRLEN] = {
		"look at inventory",
		"look at inv",
		"check inventory",
		"check inv",
		"inventory",
		"inv" };
	const unsigned int GETERRORSTRLEN = 4;
	const std::string GETERRORSTR[GETERRORSTRLEN] = {
		"Ah, unfortunately, it seems that may be attached.",
		"Yeah, that's not happening.",
		"It's kind of attached.",
		"It's... possible... but yeah, no."	};

	std::string input;
	bool valid = false;
	std::cout << "What will you do? ";
	while (!valid)
	{
		bool unable = false;

		// Obtain input, then make sure it's all lowercase
		std::getline(std::cin, input);
		for (unsigned int i = 0; i < input.size(); i++)
			input[i] = std::tolower(input[i]);

		// Check if input is even in the range of being valid
		if (input.length() > 0 && input.length() < 3)
			std::cout << "Sorry, I didn't understand. Could you try something a bit longer? " << std::endl;
		else if (input.length() > 30)
			std::cout << "Sorry, I didn't get all that! Could you try something a bit shorter? " << std::endl;
		
		else
		{
			// Check if input matches one of the pre-defined commands
			if (input == "look away")
			{
				m_pParent->Load();
				Game::Instance().ChangeFocus(m_pParent);
				valid = true;
			}
			else if (input == "look closer" && !m_bCloser)
			{
				m_bCloser = true;
				valid = true;
			}
			else
			{
				if (!valid)
				{
					for (unsigned int i = 0; i < EXITSTRLEN; i++)
					{
						if (input == EXITSTR[i])
						{
							Game::Instance().ChangeFocus(nullptr);
							valid = true;
							break;
						}
					}
				}
				if (!valid)
				{
					for (unsigned int i = 0; i < INVSTRLEN; i++)
					{
						if (input == INVSTR[i])
						{
							Object* inventory = Game::Instance().GetInventory();
							inventory->SetParent(this);
							Game::Instance().ChangeFocus(inventory);
							valid = true;
							break;
						}
					}
				}
				if (!valid)
				{
					for (std::list<Object*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); it++)
					{
						// Get the name, make sure it's lowercase, then append it to the two commands
						std::string namestr = (*it)->GetName();
						for (int i = 0; i < namestr.size(); i++)
							namestr[i] = std::tolower(namestr[i]);
						std::string lookstr = "look at " + namestr;
						std::string chekstr = "check " + namestr;
						std::string stepstr = "step closer to " + namestr;
						std::string pickstr = "pick up " + namestr;

						if (input == lookstr || input == chekstr || input == stepstr)
						{
							Game::Instance().ChangeFocus(*it);
							valid = true;
							break;
						}
						else if (input == pickstr)
						{
							if ((*it)->GetAttached() == false)
							{
								this->m_SendChild(it, Game::Instance().GetInventory());
								valid = true;
								break;
							}
							else
							{
								std::cout << GETERRORSTR[rand() % GETERRORSTRLEN] << std::endl;
								unable = true;
								break;
							}
						}
					}
				}
			}
			if (!valid && !unable)
				std::cout << "Sorry, I didn't understand." << std::endl;
		}
	}
	std::cout << std::endl;
}
// End of input methods

Object* Object::GetChildByName(std::string name)
{
	for (std::list<Object*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); it++)
		if ((*it)->GetName() == name)
			return *it;
	return nullptr;
}

void Object::AddChild(Object* child)
{
	m_pChildren.push_back(child);
	m_pChildren.back()->SetParent(this);
}
// End of public methods

// Start of loading methods
bool Object::m_CheckForTagHeader(std::fstream& file)
{
	bool found = false;
	if (!file.fail() && file.get() == '#')
	{
		if (!file.fail() && file.get() == '#')
			found = true;
		else
			file.seekg(-2, std::ios::cur);
	}
	else
		file.seekg(-1, std::ios::cur);
	return found;
}

bool Object::m_FindTag(std::fstream& file, char tag)
{
	bool found = false;
	while (!file.fail() && !found)
	{
		if (m_CheckForTagHeader(file))
		{
			if(file.get() == tag)
				found = true;
		}
		else
			file.seekg(1, std::ios::cur);
	}
	return found;
}

bool Object::m_ReadTag(std::fstream& file, char tag, std::string& container)
{
	bool found = m_FindTag(file, tag);
	if (found)
	{
		while (!file.fail() && !m_CheckForTagHeader(file))
			container += file.get();
		file.seekg(-2, std::ios::cur);
	}
	return found;
}

bool Object::m_ReadTag(std::fstream& file, char tag, bool& container)
{
	bool found = m_FindTag(file, tag);
	if (found)
	{
		container = (file.get() == '0' ? false : true);
		file.seekg(2, std::ios::cur);
	}
	return found;
}

bool Object::m_LoadFromFile()
{
	// Initialize the variables
	std::fstream file;
	std::string filename = "Objects/" + m_sName + ".object";
	std::string parentName;
	//std::vector<std::string> childrensNames;
	std::string childName;

	// Open the text file for the object
	file.open(filename, std::ios::in);
	if (!file)
	{
		std::cerr << "Error: Unable to open file " << filename << '.' << std::endl;
		return false;
	}

	m_sFarScript.clear();
	m_ReadTag(file, 'f', m_sFarScript); // Read the object's far script
	m_sNearScript.clear();
	m_ReadTag(file, 'n', m_sNearScript); // Read the object's near script
	m_ReadTag(file, 'p', parentName); // Read the name of the parent
	if (m_pParent == nullptr) // and instantiate it only if it didn't already exist
	{
		m_pParent = Game::Instance().CheckIfInstantiated(parentName);
		if (m_pParent == nullptr)
		{
			m_pParent = new Object(parentName);
		}
	}
	m_ReadTag(file, 'b', m_bAttached); // Read whether the object is attached
	while (m_ReadTag(file, 'c', childName)) // Read the names of the children
	{
		Object* temp = Game::Instance().CheckIfInstantiated(childName);
		if (temp != nullptr)
			m_pChildren.push_back(temp);
		else
			m_pChildren.push_back(new Object(childName, this));
		childName.clear();
	}

	// Close the file
	file.close();
	m_bLoaded = true;
	return m_bLoaded;
}
// End of loading methods

// Start of display methods
void Object::m_PrintFarScript()
{
	std::cout << m_sFarScript << std::endl;
}

void Object::m_PrintNearScript()
{
	std::cout << m_sNearScript << std::endl;
}

void Object::m_PrintChildren()
{
	std::cout << "Things of interest: ";
	if (m_pChildren.empty())
		std::cout << "Nothing.";
	else
	{
		std::cout << '\n';
		for (std::list<Object*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); it++)
			std::cout << "> " << (*it)->GetName() << '\n';
	}
	std::cout << std::endl;
}
void Object::m_PrintDividingLine(std::string focus)
{
	unsigned int focusSize = focus.size();
	if (focusSize % 2 == 1)
		focusSize++;
	unsigned int halfSize = static_cast<unsigned int>(static_cast<float>(focusSize) * 0.5);

	if(focusSize > 0)
		std::cout
		<< '*' << std::string(23 - halfSize, '-') << "* "
		<< (focus.size() == focusSize ? focus : focus + ' ')
		<< " *" << std::string(23 - halfSize, '-') << '*' << std::endl;
	else
		std::cout << '*' << std::string(50, '-') << '*' << std::endl;
}
// End of display methods

// Start of gameplay methods
void Object::m_SendChild(std::list<Object*>::iterator child, Object* destination)
{
	destination->AddChild(*child);
	m_pChildren.erase(child);
}
// End of gameplay methods