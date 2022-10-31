#pragma once

#include <vector>
#include <string>

class GameObject;
class Component;
class ComponentTransform;


class State {
public:
	enum value_type{
		BOOL, INT, FLOAT, STRING
	};

	State(GameObject* gameobject, Component* component, bool value);
	State(GameObject* gameobject, Component* component, int value);
	State(GameObject* gameobject, Component* component, float value);
	State(GameObject* gameobject, Component* component, std::string value);
	~State();


	GameObject* go;
	Component* comp;
	char* keyword = "Unknown";
	value_type type;
	bool value_bool = false;
	int value_int = 0;
	float value_float = 0.0f;
	std::string value_string;

};