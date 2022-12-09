#include "Application.h"
#include "ModuleTextures.h"
#include "ImGui/imgui.h"
#include "ComponentMaterial.h"
#include "GameObject.h"
#include "Component.h"
#include "ModuleFileSystem.h"
ComponentMaterial::ComponentMaterial(GameObject* parent) : Component(parent) {
	this->type = "Material";
}

void ComponentMaterial::SetTexture(const TextureObject& texture)
{
	textureName = texture.name;
	textureId = texture.id;
	width = texture.width;
	height = texture.height;
}

void ComponentMaterial::OnGui()
{
	if (ImGui::CollapsingHeader("Material"))
	{
		if (textureId != 0)
		{
			ImGui::Text("Name: %s", textureName.c_str());
			ImGui::Image((ImTextureID)textureId, ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::Text("Size: %d x %d", width, height);
		}
	}
}

bool ComponentMaterial::OnSave(JSONWriter& writer)
{
	std::string file;
	std::string ext;
	App->fileSystem->SplitFilePath(textureName.c_str(), nullptr, &file, &ext);
	file += "." + ext;
	writer.String("TexturePath"); writer.String(file.c_str());
	return false;
}

bool ComponentMaterial::OnLoad(JSONReader& reader)
{
	if (reader.HasMember("TexturePath"))
	{
		textureName = reader["TexturePath"].GetString();
		const TextureObject& textureObject = App->textures->Load("Assets/Textures/" + textureName);
		SetTexture(textureObject);

	}
	return false;
}



