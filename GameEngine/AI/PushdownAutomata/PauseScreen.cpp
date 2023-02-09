#include "PauseScreen.h"
#include <iostream>

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
			GameTechRenderer::GameState gameState = paintingGame->GetGameTechRenderer()->GetGameState();

			switch (gameState) {

			case GameTechRenderer::GameState::PauseMenu: {
				return PushdownResult::NoChange;
			}break;
	
			case GameTechRenderer::GameState::SplitScreen: {
				return PushdownResult::Pop;
			}break;

			case GameTechRenderer::GameState::SinglePlayer: {
				return PushdownResult::Pop;
			}break;

			case GameTechRenderer::GameState::LAN: {
				return PushdownResult::Pop;
			}break;

			case GameTechRenderer::GameState::MainMenu: {
				return PushdownResult::Pop;
			}break;

			}
			
		}
		void PauseScreen::OnAwake()
		{

		}

	}
}
