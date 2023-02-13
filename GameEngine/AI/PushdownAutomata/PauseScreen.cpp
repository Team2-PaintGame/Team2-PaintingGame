#include "PauseScreen.h"
#include <iostream>
#include "MenuHandler.h"

namespace NCL {
	namespace CSC8508 {

		PauseScreen::PauseScreen( PaintingGame* paintingGame)
		{
			selectExitOption = false;
			this->paintingGame = paintingGame;
		}
		PauseScreen::~PauseScreen()
		{

		}
		PushdownState::PushdownResult PauseScreen::OnUpdate(float dt, PushdownState** newState)
		{
			paintingGame->GetGameTechRenderer()->Render();
			GameState gameState = menuHandler->GetGameState();

			switch (gameState) {

			case GameState::PauseMenu: {
				return PushdownResult::NoChange;
			}break;
	
			case GameState::SplitScreen: {
				return PushdownResult::Pop;
			}break;

			case GameState::SinglePlayer: {
				return PushdownResult::Pop;
			}break;

			case GameState::LAN: {
				return PushdownResult::Pop;
			}break;

			case GameState::MainMenu: {
				return PushdownResult::Pop;
			}break;

			}
			
		}
		void PauseScreen::OnAwake()
		{

		}

	}
}
