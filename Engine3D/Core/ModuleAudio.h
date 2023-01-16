#pragma once
#include "Module.h"
#include "Globals.h"
#include "Math.h"
#include "Wwise/AK/SoundEngine/Common/AkTypes.h"
#include <vector>

class ModuleAudio : public Module
{
public:

	ModuleAudio(Application* app, bool start_enabled = true);

	bool Init() override;
	update_status Update(float dt) override;
	bool CleanUp() override;
};