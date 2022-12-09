#pragma once

#include "Component.h"
#include "Module.h"
struct TextureObject;

class ComponentMaterial : public Component {

public:

	ComponentMaterial(GameObject* parent);

	void SetTexture(const TextureObject& texture);
	void OnGui() override;
	inline uint GetTextureId() const { return textureId; }
	virtual std::string GetType() {
		Component* c;
		if (dynamic_cast<ComponentMaterial*>(c) == nullptr)
			return std::string("Material");
	}

	std::string textureName;

	bool OnLoad(JSONReader& reader);
	bool OnSave(JSONWriter& writer);

private:

	uint textureId = 0, width = 0, height = 0;
};