#include "LoadingScreen.h"
#include "GameScreen.h"
#include "HUDOnLoad.h"
#include "OGLTexture.h"
#include "OGLAssetLoader.h"
#include <thread>
#include "GameManager.h"

using namespace NCL;
using namespace CSC8508;

NCL::CSC8508::LoadingScreen::LoadingScreen(ScreenManager* screenManager, SceneNode* sceneNode) : BaseScreen(screenManager, sceneNode) {
	NCL::Assets::OGLAssetLoader loader;
	ShaderBase* loadScreenShader = loader.LoadShader("hud.vert", "hud.frag");

	BaseScreen::sceneNode = new SceneNode(loader.LoadMesh(NCL::Assets::MeshType::Quad), loadScreenShader, TextureLoader::LoadAPITexture("Screens/bg1.jpg"));

	screenType = ScreenType::LoadingScreen;
	renderObj = this->sceneNode->GetRenderObject();
	transform = renderObj->GetTransform();

	hUDOnLoad = new HUDOnLoad(new Transform(), loader.LoadMesh(NCL::Assets::MeshType::Quad), loadScreenShader);
	hUDOnLoad->SetDefaultTexture(TextureLoader::LoadAPITexture("loadingSprites.png"));
	//loadThread = new std::thread(SpiningLoadScreen);
}

void NCL::CSC8508::LoadingScreen::OnAwake()
{
	
}

PushdownState::PushdownResult LoadingScreen::OnUpdate(float dt, PushdownState** newState)
{
	hUDOnLoad->Update(dt);
	return onStateChange(newState);
}

PushdownState::PushdownResult LoadingScreen::onStateChange(PushdownState** newState)
{
	if (!threadToWait || GameManager::GetLoadingFlag()) // if thread joinable
	{
		if (threadToWait)
		{
			threadToWait->join();
			delete threadToWait;
			threadToWait = nullptr;
			GameManager::FinishLoadingCallback();
		}

		*newState = screenManager->GetScreen(ScreenType::SplashScreen);

		return PushdownResult::Push;
	}
	else
	{
		return PushdownResult::NoChange;
	}
}