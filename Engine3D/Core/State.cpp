#include "State.h"
#include "GameObject.h"





State::State(GameObject* gameobject, Component* component, bool value, char* keyword_) : go(gameobject), comp(component), value_bool(value), keyword(keyword_) {
	this->type = BOOL;
}
State::State(GameObject* gameobject, Component* component, int value, char* keyword_) : go(gameobject), comp(component), value_int(value), keyword(keyword_) {
	this->type = INT;
}
State::State(GameObject* gameobject, Component* component, float value, char* keyword_) : go(gameobject), comp(component), value_float(value), keyword(keyword_) {
	this->type = FLOAT;
}
State::State(GameObject* gameobject, Component* component, std::string value, char* keyword_) : go(gameobject), comp(component), value_string(value), keyword(keyword_) {
	this->type = STRING;
}
State::~State() {

}