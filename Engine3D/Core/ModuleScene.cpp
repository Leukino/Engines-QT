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

	return true;
}

update_status ModuleScene::Update(float dt)
{
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

GameObject* ModuleScene::CreateGameObject(GameObject* parent) {

	GameObject* temp = new GameObject();
	if (parent)
		parent->AttachChild(temp);
	else
		root->AttachChild(temp);
	return temp;
}
GameObject* ModuleScene::CreateGameObject(const std::string name, GameObject* parent)
{
	GameObject* temp = new GameObject(name);
	if (parent)
		parent->AttachChild(temp);
	else
		root->AttachChild(temp);
	return temp;
}

bool ModuleScene::SaveScene(std::string path)
{
	/*rapidjson::StringBuffer sb;
	JSONWriter writer(sb);

	std::vector<GameObject*> objects = root->children;

	writer.StartObject();
	for (size_t i = 0; i < objects.size(); i++)
	{

		writer.String("urmom");
		writer.StartObject();
		SAVE_JSON_FLOAT(1.0f)
		SAVE_JSON_FLOAT(2.0f)
		SAVE_JSON_FLOAT(3.0f)
		SAVE_JSON_FLOAT(4.0f)
		SAVE_JSON_FLOAT(5.0f)
		writer.EndObject();

		for (size_t j = 0; j < objects[i]->components.size(); j++)
		{
			objects[i]->components[j]->OnSave(writer)
		}
		
	}
	writer.EndObject();

	if (App->fileSystem->Save(path.c_str(), sb.GetString(), strlen(sb.GetString()), false))
	{
		LOG("Scene saved.");
	}
	else
	{
		LOG("Scene not saved.");
	}*/
	return false;
}

bool ModuleScene::LoadScene(std::string path)
{
	/*char* buffer = nullptr;
	uint bytesFile = App->fileSystem->Load(path.c_str(), &buffer);

	if (bytesFile)
	{
		rapidjson::Document document;
		if (document.Parse<rapidjson::kParseStopWhenDoneFlag>(buffer).HasParseError())
		{
			LOG("Error loading engine config");
		}
		else
		{
			const rapidjson::Value config = document.GetObjectJSON();

			for (size_t i = 0; i < modules.size(); i++)
			{
				modules[i]->OnLoad(config);
			}

			LOG("Engine config loaded");
		}
	}
	RELEASE_ARRAY(buffer);*/
	return false;
}

