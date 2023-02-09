#include "IntroScreen.h"
#include "SinglePlayerScreen.h"
#include "SplitScreen.h"
#include "LanScreen.h"
#include "GameTechRenderer.h"
#include <iostream>

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
			GameTechRenderer::GameState gameState = paintingGame->GetGameTechRenderer()->GetGameState();
			switch (gameState) {

				case GameTechRenderer::GameState::SinglePlayer: {
					*newState = new SinglePlayerScreen(window, paintingGame);
					return PushdownResult::Push;
				}	break;

				case GameTechRenderer::GameState::SplitScreen: {
					*newState = new SplitScreen(window, paintingGame);
					return PushdownResult::Push;
				}break;

				case GameTechRenderer::GameState::LAN: {
					*newState = new LanScreen(window, paintingGame);
					return PushdownResult::Push; // Add in when we have LAN game created
				}break;

				case GameTechRenderer::GameState::ExitGame: {
					return PushdownResult::Pop;
				}break;

				case GameTechRenderer::GameState::MainMenu: {
					return PushdownResult::NoChange;
				}break;

			}
		}
		void IntroScreen::OnAwake()
		{


		}
	}
}

