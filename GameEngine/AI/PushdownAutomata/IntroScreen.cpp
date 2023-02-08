#include "IntroScreen.h"
#include "SinglePlayerScreen.h"
#include "SplitScreen.h"
#include "LanScreen.h"
#include <iostream>

namespace NCL {
	namespace CSC8508 {
		IntroScreen::IntroScreen(Window* window, PaintingGame* paintingGame)
		{
			isLanScreen = false;
			this->window = window;
			this->paintingGame = paintingGame;
		}
		IntroScreen::~IntroScreen()
		{

		}
		PushdownState::PushdownResult IntroScreen::OnUpdate(float dt, PushdownState** newState)
		{
			paintingGame->UpdateGame(dt);
		//	std::cout << "isMainMenu: " << paintingGame->GetGameTechRenderer()->GetIsMainMenu() << "\n";
			if (paintingGame->GetGameTechRenderer()->GetIsSinglePlayer() )
			{
				*newState = new SinglePlayerScreen(window, paintingGame);
				paintingGame->GetGameTechRenderer()->ToggleIsMainMenu();
				
				return PushdownResult::Push;
			}
			if (paintingGame->GetGameTechRenderer()->GetIsSplitScreen())
			{
				*newState = new SplitScreen(window, paintingGame);
				paintingGame->GetGameTechRenderer()->ToggleIsMainMenu();
				return PushdownResult::Push;
			}
			if (isLanScreen)
			{
				*newState = new LanScreen();
				paintingGame->GetGameTechRenderer()->ToggleIsMainMenu();
				return PushdownResult::Push;
			}
			if (paintingGame->GetGameTechRenderer()->GetIsExitPaintGame())
			{
				return PushdownResult::Pop;
			}
			return PushdownResult::NoChange;
		}
		void IntroScreen::OnAwake()
		{
			// ImGui Main Menu

			std::cout << "IntroScreen:\n"
				<< "isSinglePlayer: " << paintingGame->GetGameTechRenderer()->GetIsSinglePlayer() << "\n"
				<< "isMainMenu: " << paintingGame->GetGameTechRenderer()->GetIsMainMenu() << "\n"
				<< "isPauseMenu: " << paintingGame->GetGameTechRenderer()->GetIsPauseMenu() << "\n"
				<< "isExitPauseMenu: " << paintingGame->GetGameTechRenderer()->GetIsExitPauseMenu() << "\n"
				<< "isExitPaintGame: " << paintingGame->GetGameTechRenderer()->GetIsExitPaintGame() << "\n"
				<< "isSplitScreen: " << paintingGame->GetGameTechRenderer()->GetIsSplitScreen() << "\n\n";
				

		}


	}
}