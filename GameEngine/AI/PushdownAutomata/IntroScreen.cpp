#include "IntroScreen.h"
#include "SinglePlayerScreen.h"
#include "SplitScreen.h"
#include "LanScreen.h"
#include "GameTechRenderer.h"
#include <iostream>
#include "MenuHandler.h"


namespace NCL {
	namespace CSC8508 {
		IntroScreen::IntroScreen(Window* window, PaintingGame* paintingGame)
		{
			isLanScreen = false;
			this->window = window;
			this->paintingGame = paintingGame;
		}
		IntroScreen::IntroScreen(Window* window)
		{
			isLanScreen = false;
			this->window = window;
			PaintingGame g(false);
			paintingGame = &g;
		}
		IntroScreen::~IntroScreen()
		{

		}
		PushdownState::PushdownResult IntroScreen::OnUpdate(float dt, PushdownState** newState)
		{
			paintingGame->UpdateGame(dt);
			NCL::GameState gameState = menuHandler->GetGameState();
			switch (gameState) {

				case GameState::SinglePlayer: {
					*newState = new SinglePlayerScreen(window, paintingGame);
					return PushdownResult::Push;
				}	break;

				case GameState::SplitScreen: {
					*newState = new SplitScreen(window, paintingGame);
					return PushdownResult::Push;
				}break;

				case GameState::LAN: {
					*newState = new LanScreen(window, paintingGame);
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

