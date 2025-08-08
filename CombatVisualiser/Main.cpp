#include <SDL.h>
#include <filesystem>

#ifdef WIN32
#include <windows.h>
#endif

//#ifndef NDEBUG
//// ReSharper disable once CppUnusedIncludeDirective
//#if __has_include(<vld.h>)
//#include <vld.h>
//#endif
//#endif

#include "MkUltra.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "DefaultSoundSystem.h"
#include "ServiceLocator.h"
#include "Levels/Levels.h"

namespace fs = std::filesystem;
using namespace mk;

void load()
{
	//ServiceLocator::GetSoundSystem().Play("MainMenuMusic.mp3", 0.7f);
	SceneManager::GetInstance().LoadScene("Tron", &LoadMainGame);
}


int main(int, char*[]) {

#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "./Data/";
#endif
	MkUltra engine(data_location, 480, 480);
	ServiceLocator::SetDefaultDataPath(data_location.string());
	ServiceLocator::RegisterSoundSystem<DefaultSoundSystem>();
	engine.Run(load);

	return 0;
}
