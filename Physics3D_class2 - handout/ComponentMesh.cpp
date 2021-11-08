#pragma once
#include "ComponentMesh.h"
#include "GameObject.h"

#include "ModuleImport.h" //TODO: NEEDED ONLY BECAUSE OF MESH CLASS-> Turn Mesh Class into stand alone file.
#include "ImGUI/imgui.h"

ComponentMesh::ComponentMesh(GameObject* parent) : Component(parent)
{
	//parent->AddComponent(this);
	mesh = nullptr; 
	path = "No path!";
	type = ComponentType::Mesh;
};

void ComponentMesh::Enable() {

}

void ComponentMesh::Disable() {

}

void ComponentMesh::Update() {

}

void ComponentMesh::DrawInspector() {
	
	if(ImGui::CollapsingHeader("Component Mesh"))
	{
		ImGui::Text("Path: %s", path);
		ImGui::Text("Index: %d", mesh->sizeOf[0]);
		ImGui::Text("Vertices: %d", mesh->sizeOf[1]);
		ImGui::Text("Normals: %d", mesh->sizeOf[2]);
		ImGui::Text("texture: %d", mesh->sizeOf[3]);
	}
}

void ComponentMesh::SetMesh(Mesh* mesh)
{
	this->mesh = mesh;
}

void ComponentMesh::SetPath(std::string path)
{
	this->path = path;
}