#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include <vector>
#include <string>

class ModuleEngine : public Module
{
public:
	ModuleEngine(Application* app, bool start_enabled = true);
	~ModuleEngine();

	bool Start();
	bool Init() override;
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

    void MainMenuBar();
    void Config();
    void ConsoleLog(std::string log);
    void Console();

	void RenderImgui();

public:

	SDL_version sdlVersion;

public:
	bool dockingWindow = true;

public:

    int fps = 60;
    bool vsync = false;

    std::vector<std::string> console_log;


    bool showConfig = true;
    bool showConsole = true;
    bool showHierarchy = true;
    bool showInspector = true;

    int window_width = 500;
    int window_height = 500;

    bool fullscreen = false;
    bool borderless = false;
    bool full_desktop = false;
    bool debug;
};

