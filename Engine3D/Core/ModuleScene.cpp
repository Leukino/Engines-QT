#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "glew.h"
#include "ImGui/imgui.h"
#include "ModuleImport.h"
#include "ModuleTextures.h"
#include "ModuleCamera3D.h"
#include "ModuleFileSystem.h"
#include "Component.h"
#include <stack>
#include <queue>
#include <string>

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

bool ModuleScene::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	
	root = new GameObject("Root");

	//Loading house and textures since beginning
	App->import->LoadGeometry("Assets/Models/BakerHouse.fbx");
	
	return ret;
}

bool ModuleScene::CleanUp()
{
	std::stack<GameObject*> S;
	for (GameObject* child : root->children)	
	{
		S.push(child);
	}
	root->children.clear();

	while (!S.empty())
	{
		GameObject* go = S.top();
		S.pop();
		for (GameObject* child : go->children)
		{
			S.push(child);
		}
		go->children.clear();
		delete go;
	}

	delete root;
	root = nullptr;

	return true;
}

update_status ModuleScene::Update(float dt)
{
	if (root == nullptr)
	{
		root = new GameObject("Root");
		return UPDATE_CONTINUE;
	}
		std::queue<GameObject*> S;
	for (GameObject* child : root->children)
	{
		S.push(child);
	}

	while (!S.empty())
	{
		GameObject* go = S.front();
		go->Update(dt);
		S.pop();
		for (GameObject* child : go->children)
		{
			S.push(child);
		}
	}

	return UPDATE_CONTINUE;
}

GameObject* ModuleScene::CreateGameObject(GameObject* parent) 
{
	if (root == nullptr)
		root = new GameObject("Root");
	GameObject* temp = new GameObject();
	if (parent)
		parent->AttachChild(temp);
	else
		
		root->AttachChild(temp);
	return temp;
}
GameObject* ModuleScene::CreateGameObject(const std::string name, GameObject* parent)
{
	if (root == nullptr)
		root = new GameObject("Root"); 
	GameObject* temp = new GameObject(name);
	if (parent)
		parent->AttachChild(temp);
	else
		
		root->AttachChild(temp);
	return temp;
}

GameObject* ModuleScene::FindGameObject(std::string name)
{
	std::vector<GameObject*> objects = root->children;
	GameObject* ret = nullptr;
	for (size_t i = 0; i < objects.size(); i++)
	{
		std::string n = objects[i]->name;
		if (n == name)
			ret = objects[i];
	}
	return ret;
}

GameObject* ModuleScene::FindGameObjectByUuid(std::string uuid)
{
	std::vector<GameObject*> objects = root->children;
	GameObject* ret = nullptr;
	for (size_t i = 0; i < objects.size(); i++)
	{
		std::string n = objects[i]->name;
		if (n == uuid)
			ret = objects[i];
	}
	return ret;
}

bool ModuleScene::SaveScene(std::string path)
{
	rapidjson::StringBuffer sb;
	JSONWriter writer(sb);

	std::vector<GameObject*> objects = root->children;
	//writer.StartArray();
	writer.StartObject();
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->OnSave(writer);
	}
	writer.EndObject();
	//writer.EndArray();
	if (App->fileSystem->Save(path.c_str(), sb.GetString(), strlen(sb.GetString()), false))
	{
		LOG("Scene saved.");
	}
	else
	{
		LOG("Scene not saved.");
	}
	return false;
}

bool ModuleScene::LoadScene(std::string path)
{
	char* buffer = nullptr;
	uint bytesFile = App->fileSystem->Load(path.c_str(), &buffer);

	//std::vector<GameObject*> objects = root->children;

	if (bytesFile)
	{
		rapidjson::Document document;
		if (document.Parse<rapidjson::kParseStopWhenDoneFlag>(buffer).HasParseError())
		{
			LOG("Error loading engine config");
		}
		else
		{
			if (root == nullptr)
				root = new GameObject("Root");
			rapidjson::Value config = document.GetObjectJSON();
			//const auto& begin = config.MemberBegin();
			/*for (JSONReader::ConstValueIterator i = config.Begin(); i != config.End(); ++i)
			{
			}*/
			//assert(config.IsArray());
			for (rapidjson::Value::MemberIterator itr = config.MemberBegin(); itr != config.MemberEnd(); ++itr)
			{
				LOG("Found object poggers! %s", itr);
				// create gameobjects
				std::string name = itr->name.GetString();
				CreateGameObject(name, root);
				//if (itr->value.HasMember(""))

			}
				//objects[i]->OnLoad();

				
			LOG("Engine config loaded");
		}
	}
	RELEASE_ARRAY(buffer);
	return false;
}

