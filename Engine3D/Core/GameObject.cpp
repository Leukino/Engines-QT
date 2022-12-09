#include "GameObject.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleFileSystem.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ImGui/imgui.h"
#include "ModuleEditor.h"
#include "uuid_generator.h"

GameObject::GameObject() {

	name = name + ("GameObject");
	parent = nullptr;
	transform = CreateComponent<ComponentTransform>();
	uuid = uuidgen::generate_uuid();
	
	active = true;
}

GameObject::GameObject(const std::string name) : name(name) 
{
	transform = CreateComponent<ComponentTransform>();
	uuid = uuidgen::generate_uuid();
	active = true;
}


GameObject::~GameObject() {

	for (size_t i = 0; i < components.size(); i++) {
		RELEASE(components[i]);
	}

	components.clear();

	for (GameObject* go : children)
	{
		RELEASE(go);
	}

	parent = nullptr;
}

void GameObject::Update(float dt) 
{
	for (Component* component : components)
	{
		component->Update(dt);
	}
}

void GameObject::OnGui()
{
	if (App->scene->root != this)
	{
		ImGui::Text("%s", name.c_str());
		ImGui::Separator();
		
		for (Component* component : components)
		{
			component->OnGui();
		}

		ImGui::Separator();
		const char* items[] = { "Transform", "Mesh", "Material" };
		static int item_current_idx = 0;                                     // Here our selection data is an index.
		const char* combo_label = items[item_current_idx];  // Label to preview before opening the combo (technically it could be anything)
		if (ImGui::BeginCombo("combo 1", combo_label))
		{
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				const bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable(items[n], is_selected))
					item_current_idx = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		if (ImGui::Button("Create Component"))
		{
			const char* cas = items[item_current_idx];
			//Component* comp = nullptr;

			if (strcmp(cas, "Mesh") == 0) {
				CreateComponent<ComponentMesh>();
			}
			else if (strcmp(cas, "Material") == 0) { CreateComponent<ComponentMaterial>();}
		}

		/*if (ImGui::Button("Undo Action")) // Manual click of ctrl + z
		{
			if (App->editor->actions.size() > 0)
			{
				for (Component* component : components)
				{
					if (strcmp(component->type, App->editor->actions.back().comp->type) == 0)
					{
						bool ret = component->UndoAction(&App->editor->actions.back());
						if (ret)
						{
							App->editor->actions.pop_back();
							LOG("Successfully undone action")
								break;
						}
						else
							LOG("Error undoing action");
					}
				}
			}
			else
				LOG("No actions have been done! (don't spam ctrl z pls he gets stressed)");
		}
		*/

		ImGui::Separator();
		ImGui::Text("%s", uuid.c_str());
	}
}

void GameObject::DeleteComponent(Component* component) {

	auto componentIt = std::find(components.begin(), components.end(), component);
	if (componentIt != components.end())
	{
		components.erase(componentIt);
		components.shrink_to_fit();
	}
}

void GameObject::AddComponent(Component* component)
{
	components.push_back(component);
}

void GameObject::AttachChild(GameObject* child)
{
	child->parent = this;
	children.push_back(child);
	child->transform->NewAttachment();
	child->PropagateTransform();
}

void GameObject::RemoveChild(GameObject* child)
{
	auto it = std::find(children.begin(), children.end(), child);
	if (it != children.end())
	{
		children.erase(it);
	}
}

void GameObject::PropagateTransform()
{
	for (GameObject* go : children)
	{
		go->transform->OnParentMoved();
	}
}

void GameObject::OnSave(JSONWriter& writer)
{
	writer.String(name.c_str());
	const char* uuid_ = uuid.c_str();
	const char* parentUuid_ = parent->uuid.c_str();


	writer.StartObject();
	SAVE_JSON_STRING(uuid_);
	if (parent->name.compare("Root") != 0)
	SAVE_JSON_STRING(parentUuid_);

	for (auto& component : components)
	{
		writer.String(component->type);
		writer.StartObject();
		component->OnSave(writer);
		writer.EndObject();
	}


		if (children.empty())
			writer.EndObject();
		for (uint c = 0; c < children.size(); c++)
		{
			const char* childrenUuid_ = children[c]->uuid.c_str();

			SAVE_JSON_STRING(childrenUuid_);
			writer.EndObject();
			children[c]->OnSave(writer);
		}

	//writer.EndObject();

	/*for (size_t j = 0; j < objects[i]->components.size(); j++)
	{
		objects[i]->components[j]->OnSave(writer);
	}*/

	return;
}

void GameObject::OnLoad(JSONReader& reader)
{
	auto& config = reader;
	LOAD_JSON_STRING(name);
	uuid = reader.HasMember("uuid_") ? reader["uuid_"].GetString() : "Unknown_UUID";
	//bool meshFound = false;
	for (rapidjson::Value::MemberIterator itr = reader.MemberBegin(); itr != reader.MemberEnd(); ++itr)
	{
		if (itr->name == "Transform")
		{
			transform->OnLoad(itr->value);
		}
		if (itr->name == "Mesh" /*&& !meshFound*/)
		{
			ComponentMesh* comp = CreateComponent<ComponentMesh>();
			comp->OnLoad(itr->value);
			//meshFound = true;
		}
		if (itr->name == "Material")
		{
			if (!GetComponent<ComponentMaterial>())
				CreateComponent<ComponentMaterial>()->OnLoad(itr->value);
			else
			{
				GetComponent<ComponentMaterial>()->OnLoad(itr->value);
			}
		}
	}

	/*if (reader.HasMember("Transform"))
	{


	}*/



	//for (rapidjson::Value::MemberIterator itr = reader.MemberBegin(); itr != reader.MemberEnd(); ++itr)
	//{
	//	// create gameobjects
	//	std::string name = itr->name.GetString();
	//	//if (itr->value.HasMember(""))

	//}

	//LOG("")

	//components
	/*if (reader.HasMember("camera"))
	{
		const auto& config = reader["camera"];
		LOAD_JSON_FLOAT(verticalFOV);
		LOAD_JSON_FLOAT(nearPlaneDistance);
	}*/
}
