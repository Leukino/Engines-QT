#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	bool Init() override;
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();

public:

	SDL_version sdlVersion;
};
