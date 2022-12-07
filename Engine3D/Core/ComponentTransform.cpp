#include "ComponentTransform.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleScene.h"
#include "glew.h"
#include "ImGui/imgui.h"
#include "ModuleEditor.h"
#include "ModuleRenderer3D.h"
#include "ImGui/ImGuizmo.h"
#include "ModuleCamera3D.h"


ComponentTransform::ComponentTransform(GameObject* parent) : Component(parent) {
	
	position = float3::zero;
	rotation = Quat::identity;
	scale = float3::one;

	transformMatrix.SetIdentity();
	transformMatrixLocal.SetIdentity();
	this->type = "Transform";
}


bool ComponentTransform::Update(float dt) {
	if (App->pause)
		return true;

	if (isDirty)
	{
		transformMatrixLocal = float4x4::FromTRS(position, rotation, scale); 
		RecomputeGlobalMatrix();
		owner->PropagateTransform();
		isDirty = false;
	}
	return true;
}

void ComponentTransform::OnGui()
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		usingManual = false;
		if (ImGui::Button("Store info"))
		{
			App->editor->actions.push_back(State(owner, this, position, "Pos"));
			App->editor->actions.push_back(State(owner, this, rotationEuler, "Rot"));
			App->editor->actions.push_back(State(owner, this, scale, "Sca"));
		}
		/*if (ImGui::Button("Get position"))
		{
			
		}*/

		/*if (ImGuizmo::IsUsing())
			return;*/
		float3 newPosition = position;
		if (ImGui::DragFloat3("Location", &newPosition[0]))
		{
			SetPosition(newPosition);
			usingManual = true;
		}
		float3 newRotationEuler;
		newRotationEuler.x = RADTODEG * rotationEuler.x;
		newRotationEuler.y = RADTODEG * rotationEuler.y;
		newRotationEuler.z = RADTODEG * rotationEuler.z;
		if (ImGui::DragFloat3("Rotation", &(newRotationEuler[0])))
		{
			newRotationEuler.x = DEGTORAD * newRotationEuler.x;
			newRotationEuler.y = DEGTORAD * newRotationEuler.y;
			newRotationEuler.z = DEGTORAD * newRotationEuler.z;
			SetRotation(newRotationEuler);
			usingManual = true;
		}
		float3 newScale = scale;
		if (ImGui::DragFloat3("Scale", &(newScale[0])))
		{
			SetScale(newScale);
			usingManual = true;
		}
		ImGui::Text("Elements in stack: %d", App->editor->actions.size());
		
		
	}
}

void ComponentTransform::SetPosition(const float3& newPosition)
{
	position = newPosition;
	isDirty = true;
}

void ComponentTransform::SetRotation(const float3& newRotation)
{
	rotation = Quat::FromEulerXYZ(newRotation.x, newRotation.y, newRotation.z);
	rotationEuler = newRotation;
	isDirty = true;
}

void ComponentTransform::SetScale(const float3& newScale)
{
	scale = newScale;
	isDirty = true;
}

void ComponentTransform::NewAttachment()
{
	if (owner->parent != App->scene->root)
		transformMatrixLocal = owner->parent->transform->transformMatrix.Inverted().Mul(transformMatrix);

	float3x3 rot;
	transformMatrixLocal.Decompose(position, rot, scale);
	rotationEuler = rot.ToEulerXYZ();
}

void ComponentTransform::OnParentMoved()
{
	RecomputeGlobalMatrix();
}

void ComponentTransform::RecomputeGlobalMatrix()
{
	if (owner->parent != nullptr)
	{
		transformMatrix = owner->parent->transform->transformMatrix.Mul(transformMatrixLocal);
	}
	else
	{
		transformMatrix = transformMatrixLocal;
	}
}

bool ComponentTransform::OnSave(JSONWriter& writer)
{
	writer.String("Position"); 
	writer.StartArray();
	writer.Double(position.x); writer.Double(position.y); writer.Double(position.z);
	writer.EndArray();
	writer.String("Rotation");
	writer.StartArray();
	writer.Double(rotationEuler.x); writer.Double(rotationEuler.y); writer.Double(rotationEuler.z);
	writer.EndArray();
	writer.String("Scale");
	writer.StartArray();
	writer.Double(scale.x); writer.Double(scale.y); writer.Double(scale.z);
	writer.EndArray();
	return true;
}

bool ComponentTransform::OnLoad(JSONReader& reader)
{
	if (reader.HasMember("Position"))
	{
		auto& arr = reader["Position"].GetArray();
		LOG("Position found %f %f %f", arr[0].GetDouble(), arr[1].GetDouble(), arr[2].GetDouble());
		SetPosition(float3(arr[0].GetDouble(), arr[1].GetDouble(), arr[2].GetDouble()));
	}
	if (reader.HasMember("Rotation"))
	{
		auto& arr = reader["Rotation"].GetArray();
		LOG("Rotation found %f %f %f", arr[0].GetDouble(), arr[1].GetDouble(), arr[2].GetDouble());
		SetRotation(float3(arr[0].GetDouble(), arr[1].GetDouble(), arr[2].GetDouble()));
	}
	if (reader.HasMember("Scale"))
	{
		auto& arr = reader["Scale"].GetArray();
		LOG("Scale found %f %f %f", arr[0].GetDouble(), arr[1].GetDouble(), arr[2].GetDouble());
		SetScale(float3(arr[0].GetDouble(), arr[1].GetDouble(), arr[2].GetDouble()));
	}

	return true;
}

//State ComponentTransform::StoreAction() 
//{
//	
//}

bool ComponentTransform::UndoAction(State* state) { 
	if (state->go == this->owner)
	{
		if (strcmp(state->keyword, "Pos") == 0)
			SetPosition(App->editor->actions.back().value_vec3);
		if (strcmp(state->keyword, "Rot") == 0)
			SetRotation(App->editor->actions.back().value_vec3);
		if (strcmp(state->keyword, "Sca") == 0)
			SetScale(App->editor->actions.back().value_vec3);
		isDirty = true;
		return true;
	}
	return false;
}