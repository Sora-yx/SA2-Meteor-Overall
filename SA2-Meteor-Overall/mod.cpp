#include "pch.h"
#include "time.h"

HelperFunctions HelperFunctionsGlobal;
const char* error = "[SA2 Meteor]: WARNING: Your version of the Mod Loader is old, the mod won't work properly.\nPlease update your Mod Loader for the best experience.";
time_t t;

extern "C" {

	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		Sleep(10);

		HelperFunctionsGlobal = helperFunctions;

		if (HelperFunctionsGlobal.Version < 8) {
			PrintDebug(error);
			MessageBoxA(MainWindowHandle, error, "SA2 Meteor", MB_ICONWARNING);
		}

		srand((unsigned)time(&t));
		ReadConfig(path); //get mod settings by the user
	
		init_MeteorHack();
		init_CartHack();

	}

	__declspec(dllexport) void __cdecl OnFrame() {

		Load_MeteorObject();

	}


	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}