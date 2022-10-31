#include "ComponentTransform.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleScene.h"
#include "glew.h"
#include "ImGui/imgui.h"
#include "ModuleEditor.h"
#include "ModuleRenderer3D.h"

ComponentTransform::ComponentTransform(GameObject* parent) : Component(parent) {
	
	position = float3::zero;
	rotation = Quat::identity;
	scale = float3::one;

	transformMatrix.SetIdentity();
	transformMatrixLocal.SetIdentity();
	this->type = "Transform";
}


bool ComponentTransform::Update(float dt) {
	if (isDirty)
	{
		transformMatrixLocal = float4x4::FromTRS(position, rotation, scale);
		RecomputeGlobalMatrix();
		owner->PropagateTransform();
		isDirty = false;
	}
	if (App->renderer3D->drawGizmo)
		DrawGizmo();
	return true;
}

void ComponentTransform::OnGui()
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::Text("Elements in stack: %d", App->editor->actions.size());
		if (ImGui::Button("Store position"))
		{
			App->editor->actions.push_back(State(owner, this, position.x));
		}
		if (ImGui::Button("Get position"))
		{
			if (App->editor->actions.size() > 0) {
				SetPosition(math::float3(App->editor->actions.back().value_float, position.y, position.z));
				App->editor->actions.pop_back();
			}
		}


		float3 newPosition = position;
		if (ImGui::DragFloat3("Location", &newPosition[0]))
		{
			SetPosition(newPosition);
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
		}
		float3 newScale = scale;
		if (ImGui::DragFloat3("Scale", &(newScale[0])))
		{
			SetScale(newScale);
		}
		
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

//State ComponentTransform::StoreAction() 
//{
//	
//}

bool ComponentTransform::UndoAction() { return true; }

void ComponentTransform::DrawGizmo()
{
	float ORG[3] = { position.x,position.y,position.z };

	float XP[3] = { position.x+1,position.y,position.z }, XN[3] = { -1+position.x,position.y,position.z },
		YP[3] = { position.x,1+position.y,position.z }, YN[3] = { position.x,-1+position.y,position.z },
		ZP[3] = { position.x,position.y,1+position.z }, ZN[3] = { position.x,position.y,-1+position.z};
	glBegin(GL_LINE_LOOP);
	glColor3f(1, 0, 0);   // X axis is red.
	glVertex3fv(ORG);
	glVertex3fv(XP);
	glColor3f(0, 1, 0);   // Y axis is green.
	glVertex3fv(ORG);
	glVertex3fv(YP);
	glColor3f(0, 0, 1);   // z axis is blue.
	glVertex3fv(ORG);
	glVertex3fv(ZP);
	glEnd();
}