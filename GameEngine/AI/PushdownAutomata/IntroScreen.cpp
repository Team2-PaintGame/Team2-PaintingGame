#include "IntroScreen.h"
#include "SinglePlayerScreen.h"
#include "SplitScreen.h"
#include "LanScreen.h"
#include "GameTechRenderer.h"
#include <iostream>
#include "MenuHandler.h"


namespace NCL {
	namespace CSC8508 {
		IntroScreen::IntroScreen(Window* window)
		{
			this->window = window;

			menuHandler = new MenuHandler();
			menuHandler->SetGameState(GameState::MainMenu);

			paintingGame = new PaintingGame(menuHandler, false);
			paintingGame->GetGameTechRenderer()->SetRenderMode(GameTechRenderer::RenderMode::MainMenu);
		}
		IntroScreen::~IntroScreen()
		{
			delete paintingGame;
			delete menuHandler;
		}
		PushdownState::PushdownResult IntroScreen::OnUpdate(float dt, PushdownState** newState)
		{
			paintingGame->UpdateGame(dt);

			NCL::GameState gameState = menuHandler->GetGameState();
			switch (gameState) {

				case GameState::SinglePlayer: {
					*newState = new SinglePlayerScreen(window, menuHandler);
					return PushdownResult::Push;
				}	break;

				case GameState::SplitScreen: {
					*newState = new SplitScreen(window, menuHandler);
					return PushdownResult::Push;
				}break;

				case GameState::LAN: {
					*newState = new LanScreen(window, menuHandler);
					return PushdownResult::Push; // Add in when we have LAN game created
				}break;

				case GameState::ExitGame: {
					return PushdownResult::Pop;
				}break;

				case GameState::MainMenu: {
					return PushdownResult::NoChange;
				}break;

			}
		}
		void IntroScreen::OnAwake()
		{

		}
	}
}

