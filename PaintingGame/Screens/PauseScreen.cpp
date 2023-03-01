#include "PauseScreen.h"
#include <iostream>
#include "MenuHandler.h"

namespace NCL {
	namespace CSC8508 {

		PauseScreen::PauseScreen( PaintingGame* paintingGame, MenuHandler* menu)
		{
			selectExitOption = false;
			this->paintingGame = paintingGame;
			menuHandler = menu;
			menuHandler->SetGameState(GameState::PauseMenu);


		}
		PauseScreen::~PauseScreen()
		{

		}
		PushdownState::PushdownResult PauseScreen::OnUpdate(float dt, PushdownState** newState)
		{
			paintingGame->GetGameTechRenderer()->Render();
			menuHandler->Update(dt);
			GameState gameState = menuHandler->GetGameState();

			switch (gameState) {

			case GameState::PauseMenu: {
				return PushdownResult::NoChange;
			}break;
	
			case GameState::ExitPauseMenu: {
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
