#include "ModuleAudio.h"
#include <string>

//#include "AK/Comm/AkCommunication.h"

#include "wwise/AK/SoundEngine/Common/AkSoundEngine.h"
//#include "AK/SoundEngine/Common/AkMemoryMgr.h"
#include "wwise/AK/SoundEngine/Common/AkModule.h"

//#include "AK/SoundEngine/Common/IAkStreamMgr.h";

//#include "AK/SoundEngine/Common/AkStreamMgrModule.h"
#include "wwise/AK/Tools/Common/AkPlatformFuncs.h"
//#include "samples/SoundEngine/Win32/AkFilePackageLowLevelIOBlocking.h";

#include "wwise/AK/SoundEngine/Common/AkSoundEngine.h"
#include "wwise/AK/MusicEngine/Common/AkMusicEngine.h"
#include "wwise/AK/SpatialAudio/Common/AkSpatialAudio.h"

//#pragma comment(lib, "AkStreamMgr.lib")

//#ifndef AK_OPTIMIZED
//#include <AK/Comm/AkCommunication.h>
//#endif // AK_OPTIMIZED

//CAkFilePackageLowLevelIOBlocking cak;

ModuleAudio::ModuleAudio(Application* app, bool start_enabled) : Module(app, start_enabled) {}

ModuleAudio::~ModuleAudio()
{

}

bool ModuleAudio::Init()
{
	return false;
}

bool ModuleAudio::Start()
{
	return false;
}

update_status ModuleAudio::Update(float dt)
{
	return update_status();
}

bool ModuleAudio::CleanUp()
{
	return false;
}

bool ModuleAudio::InitSoundEngine()
{
	LOG("Initializing sound engine");
	AkMemSettings memSettings;
	AK::MemoryMgr::GetDefaultSettings(memSettings);
	if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
	{
		LOG("Could not initialize memory manager");
		return false;
	}

	/*AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings(stmSettings);
	if (!AK::StreamMgr::Create(stmSettings))
	{
		LOG("Could not initialize Streaming Manager");
		return false;
	}*/

	/*AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);
	if (cak.Init(deviceSettings) != AK_Success)
	{
		LOG("Could not create Low-Level I/O system");
		return false;
	}*/

	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

	if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
	{
		LOG("Could not initialize sound engine");
		return false;
	}

	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);

	if (AK::MusicEngine::Init(&musicInit) != AK_Success)
	{
		LOG("Could not initialize the Music Engine.");
		return false;
	}

	AkSpatialAudioInitSettings spatSettings; // The constructor fills AkSpatialAudioInitSettings with the recommended default settings. 

	if (AK::SpatialAudio::Init(spatSettings) != AK_Success)
	{
		LOG("Could not initialize the Spatial Audio.");
		return false;
	}
	
//#ifndef AK_OPTIMIZED
//
//
//	AkCommSettings commSettings;
//	AK::Comm::GetDefaultInitSettings(commSettings);
//	if (AK::Comm::Init(commSettings) != AK_Success)
//	{
//		LOG("Could not initialize communication.");
//		return false;
//	}
//#endif // AK_OPTIMIZED

	return true;
}


