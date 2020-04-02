#pragma once
#ifndef _OBJECT_
#define _OBJECT_
#include <string>
#include <list>
#include <fstream>

struct Child
{
	Child(std::string name, std::string present, std::string missing, bool attached)
		: filename(filename), present(present), missing(missing), attached(attached) {}
	std::string filename;
	std::string present;
	std::string missing;
	bool attached;
};

class Object
{
private: // Private member variables
	std::string m_sName;
	std::string m_sFarScript;
	std::string m_sNearScript;
	bool m_bAttached;
	bool m_bLoaded;
	bool m_bCloser;
	Object* m_pParent;
	std::list<Object*> m_pChildren;
public: // Public member functions
	// System methods
	Object(std::string name, Object* parent = nullptr);
	~Object();

	// Gameplay methods
	void Enter();
	void Load();
	void Exit();
	void Clean();
	void Display();
	void ProcessInput();

	// Accessor methods
	std::string GetName() { return m_sName; }
	bool GetAttached() { return m_bAttached; }
	bool GetLoaded() { return m_bLoaded; }
	Object* GetParent() { return m_pParent; }
	Object* GetChildByName(std::string name);

	// Mutator methods
	void SetParent(Object* parent) { m_pParent = parent; }
	void AddChild(Object* child);
	void ForceCloser() { m_bCloser = true; }

private: // Private member functions
	// Loading methods
	bool m_CheckForTagHeader(std::fstream& file); // Checks for the presence of the tag header within the file. If found, returns true and advances the position of the stream extractor past the tag.
	bool m_FindTag(std::fstream& file, char tag); // Locates a specific tag within the file, going forward from the stream extractor's current position.
	bool m_ReadTag(std::fstream& file, char tag, std::string& container); // Reads each character found between the start of the specified tag and the start of the next tag header, exclusive, and adds them to the referenced string.
	bool m_ReadTag(std::fstream& file, char tag, bool& container); // Reads the character associated with the specified tag and then advances the stream extractor to the start of the next one.
	bool m_LoadFromFile(); // Loads the script of the object and the names of all children, then instiantiates objects for each child.
	
	// Display methods
	void m_PrintFarScript(); // Prints the description for the object to the console.
	void m_PrintNearScript(); // Prints the script for the object to the console.
	void m_PrintChildren(); // Checks the inventory for the presence of each child, then prints each one not found to the console.
	void m_PrintDividingLine(std::string focus = "");

	// Gameplay methods
	void m_SendChild(std::list<Object*>::iterator child, Object* destination);
};

#endif // !_OBJECT_