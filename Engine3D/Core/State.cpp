#include "State.h"
#include "GameObject.h"





State::State(GameObject* gameobject, Component* component, bool value) : go(gameobject), comp(component), value_bool(value) {
	this->type = BOOL;
}
State::State(GameObject* gameobject, Component* component, int value) : go(gameobject), comp(component), value_int(value) {
	this->type = INT;
}
State::State(GameObject* gameobject, Component* component, float value) : go(gameobject), comp(component), value_float(value) {
	this->type = FLOAT;
}
State::State(GameObject* gameobject, Component* component, std::string value) : go(gameobject), comp(component), value_string(value) {
	this->type = STRING;
}
State::~State() {

}