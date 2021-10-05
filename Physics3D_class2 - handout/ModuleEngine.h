#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"

class ModuleEngine : public Module
{
public:
	ModuleEngine(Application* app, bool start_enabled = true);
	~ModuleEngine();

	bool Start();
	bool Init() override;
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();

	void RenderImgui();

public:

	SDL_version sdlVersion;
};
