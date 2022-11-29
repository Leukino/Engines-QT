#pragma once

// -- Tools
#include <vector>
#include <string>
#include "GameObject.h"
#include "State.h"
class State;
class Component {

public:


	Component(GameObject* parent) : owner(parent)
	{
		if (parent)
			parent->AddComponent(this);
	};
	virtual ~Component() {};

	virtual bool Update(float dt) {
		return true;
	}

	virtual bool Enable() { return active = true; };
	virtual bool Disable() { return active = false; };
	virtual void OnGui() { }
	virtual bool StoreAction() { return false; }
	virtual bool UndoAction(State* state) { return false; }
	//virtual bool OnLoad(JSONReader& reader) { return false; }
	//virtual bool OnSave(JSONWriter& writer) { return false; }

public:
	char* type = "unknown_type";
	GameObject* owner = nullptr;
	bool active = true;
};