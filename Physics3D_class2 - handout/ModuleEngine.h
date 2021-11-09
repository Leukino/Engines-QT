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
    void Hierarchy();
    void Inspector();
    void About();

	void RenderImgui();

    const char* GetSystemCaps();

    int GetBudget();
    int GetUsage();
    int GetAvailable();
    int GetReserved();

public:

	SDL_version sdlVersion;

public:
	bool dockingWindow = true;

public:

    int fps = 60;
    bool vsync = false;

    std::vector<std::string> console_log;
    std::string Caps;


    bool showConfig = true;
    bool showConsole = true;
    bool showHierarchy = true;
    bool showInspector = true;
    bool showAbout = false;

    int window_width = 500;
    int window_height = 500;
    float brightness = 0;

    bool fullscreen = false;
    bool borderless = false;
    bool full_desktop = false;
    bool debug;
    bool closeEngine = false;

    char title[25];
    char label[32];
};

