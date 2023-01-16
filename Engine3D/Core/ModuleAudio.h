#pragma once
#include "Module.h"
#include "Globals.h"
#include "Math.h"
#include "Wwise/AK/SoundEngine/Common/AkTypes.h"
#include "MathGeoLib/include/Math/float3.h"
#include <vector>
#include <map>


class ModuleAudio : public Module
{
public:

	ModuleAudio(Application* app, bool start_enabled = true);
	~ModuleAudio();


	bool Init() override;
	bool Start() override;
	update_status Update(float dt) override;
	bool CleanUp() override;

	bool InitSoundEngine();
	
};