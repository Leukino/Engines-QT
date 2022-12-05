#pragma once

#include "Component.h"
#include "Math/float3.h"
#include "Math/float4x4.h"
#include "Math/Quat.h"
#include "State.h"
class State;

class ComponentTransform : public Component {

public:

	ComponentTransform(GameObject* parent);

	/*virtual std::string GetType() {
		Component* c = nullptr;
		if (dynamic_cast<ComponentTransform*>(c) == nullptr)
			return std::string("Transform");
	}*/

	/*std::string GetType() {
		Component* c = nullptr;
		if (typeid(*c) == typeid(ComponentTransform))
			return std::string("Transform");
	}*/

	std::string GetType() {
		Component* c = nullptr;
		auto* trans = dynamic_cast<ComponentTransform*>(c);
		if (trans)
			return std::string("Transform");
		return std::string("unknown_type");
	}
	

	bool Update(float dt) override;
	void OnGui() override;

	void SetPosition(const float3& newPosition);
	void SetRotation(const float3& newRotation);
	void SetScale(const float3& newScale);

	inline float3 GetPosition() const { return position; };
	inline float3 GetRotation() const { return rotationEuler; };
	inline float3 GetScale() const { return scale; };
	

	void NewAttachment();
	void OnParentMoved();

	void RecomputeGlobalMatrix();

	bool OnLoad(JSONReader& reader);
	bool OnSave(JSONWriter& writer);

	//State StoreAction();
	bool UndoAction(State* state);
	
	float4x4 transformMatrix;
	float4x4 transformMatrixLocal;
	bool isDirty = false;
	bool usingManual = false;

private:
	

	float3 position;
	Quat rotation;
	float3 rotationEuler;
	float3 scale;

};