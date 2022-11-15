#pragma once

#include <vector>
#include <string>
#include "Math/float3.h"

class GameObject;
class Component;
class ComponentTransform;


class State {
public:
	enum value_type{
		BOOL, INT, FLOAT, STRING, VEC3
	};

	State(GameObject* gameobject, Component* component, bool value, char* keyword_ = "Unknown");
	State(GameObject* gameobject, Component* component, int value, char* keyword_ = "Unknown");
	State(GameObject* gameobject, Component* component, float value, char* keyword_ = "Unknown");
	State(GameObject* gameobject, Component* component, std::string value, char* keyword_ = "Unknown");
	State(GameObject* gameobject, Component* component, math::float3 value, char* keyword_ = "Unknown");
	~State();


	GameObject* go;
	Component* comp;
	char* keyword = "Unknown";
	value_type type;
	bool value_bool = false;
	int value_int = 0;
	float value_float = 0.0f;
	std::string value_string;
	math::float3 value_vec3;

};