#include "LoadingScreen.h"
#include "GameScreen.h"
#include "HUDOnLoad.h"
#include "OGLTexture.h"
#include "OGLAssetLoader.h"
#include <thread>

using namespace NCL;
using namespace CSC8508;

void NCL::CSC8508::LoadingScreen::OnAwake()
{
	NCL::Assets::OGLAssetLoader loader;
	hUDOnLoad = new HUDOnLoad(new Transform(), loader.LoadMesh(NCL::Assets::MeshType::Quad), loader.LoadShader("hud.vert", "hud.frag"));
	hUDOnLoad->SetDefaultTexture(TextureLoader::LoadAPITexture("loadingSprites.png"));

	//TextureBase* t = TextureLoader::LoadAPITexture("loadingSprites.png");
	GameScreen* gs;
	gs = (GameScreen*)screenManager->GetScreen(ScreenType::GameScreen);
	//create thread 
	gs->SetCommand(command);
//	GameScreen::OnLoad(gs, screenManager->GetGameAssets());
	threadToWait = new std::thread(GameScreen::OnLoad, gs, screenManager->GetGameAssets());
}

PushdownState::PushdownResult LoadingScreen::OnUpdate(float dt, PushdownState** newState)
{
	hUDOnLoad->Update(dt);

	return onStateChange(newState);
}

PushdownState::PushdownResult LoadingScreen::onStateChange(PushdownState** newState)
{
	if (threadToWait->joinable()) // if thread joinable
	{
		*newState = screenManager->GetScreen(ScreenType::SplashScreen);
		threadToWait->join();
		delete threadToWait;
		threadToWait = nullptr;
		return PushdownResult::Push;
	}
	else
	{
		return PushdownResult::NoChange;
	}
}
