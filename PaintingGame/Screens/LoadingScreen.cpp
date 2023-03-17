#include "LoadingScreen.h"
#include "GameScreen.h"
#include <thread>

using namespace NCL;
using namespace CSC8508;

void NCL::CSC8508::LoadingScreen::OnAwake()
{
	GameScreen* gs;
	gs = (GameScreen*)screenManager->GetScreen(ScreenType::GameScreen);
	//create thread 
	gs->SetCommand(command);
//	GameScreen::OnLoad(gs, screenManager->GetGameAssets());
	gameScreenThread = new std::thread(GameScreen::OnLoad, gs, screenManager->GetGameAssets());
}

PushdownState::PushdownResult LoadingScreen::OnUpdate(float dt, PushdownState** newState)
{
	return onStateChange(newState);
}

PushdownState::PushdownResult LoadingScreen::onStateChange(PushdownState** newState)
{
	if (gameScreenThread->joinable()) // if thread joinable
	{
		*newState = screenManager->GetScreen(ScreenType::GameScreen);
		gameScreenThread->join();
		delete gameScreenThread;
		gameScreenThread = nullptr;
		return PushdownResult::Push;
	}
	else
	{
		return PushdownResult::NoChange;
	}
}
