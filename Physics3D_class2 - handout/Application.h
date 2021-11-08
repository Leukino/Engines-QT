#ifndef __Application__
#define __Application__

#include <vector>
#include <string>

#include "p2List.h"
#include "Globals.h"
#include "Timer.h"

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleRenderer3D;
class ModuleEngine;
class ModuleCamera3D;

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	//ModuleAudio* audio;
	ModuleEngine* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	//ModulePhysics3D* physics;

private:

	uint					frame_count = 0;
	Timer					startup_time;
	Timer					frame_time;
	Timer					last_sec_frame_time;
	uint					last_sec_frame_count = 0;
	uint					prev_last_sec_frame_count = 0;
	uint					framerate_cap = 0;
	int						capped_ms = -1;
	int						framerateCap = 144; // Max amount of FPS
	int						totalBars = 100; // Number of bars that appear in the histogram

	std::vector<float> fpsVec;
	std::vector<float> msVec;

	Timer	ms_timer;
	float	dt;
	p2List<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

#endif __Application__