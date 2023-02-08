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
			if (paintingGame->GetGameTechRenderer()->GetIsExitPauseMenu()) // Exit to main Menu
			{
				//paintingGame->GetGameTechRenderer()->ToggleIsExitPauseMenu();
				std::cout << "isExitPauseMenu: " << paintingGame->GetGameTechRenderer()->GetIsExitPauseMenu() << "\n";
				return PushdownResult::Pop;
			}
			if (!paintingGame->GetGameTechRenderer()->GetIsPauseMenu()) { // Resume Game
				return PushdownResult::Pop;
			}
			return PushdownResult::NoChange;
		}
		void PauseScreen::OnAwake()
		{
			std::cout << "Pause Menu\n"
				<< "isSinglePlayer: " << paintingGame->GetGameTechRenderer()->GetIsSinglePlayer() << "\n"
				<< "isMainMenu: " << paintingGame->GetGameTechRenderer()->GetIsMainMenu() << "\n"
				<< "isPauseMenu: " << paintingGame->GetGameTechRenderer()->GetIsPauseMenu() << "\n"
				<< "isExitPauseMenu: " << paintingGame->GetGameTechRenderer()->GetIsExitPauseMenu() << "\n"
				<< "isExitPaintGame: " << paintingGame->GetGameTechRenderer()->GetIsExitPaintGame() << "\n"
				<< "isSplitScreen: " << paintingGame->GetGameTechRenderer()->GetIsSplitScreen() << "\n\n";
		}

	}
}
