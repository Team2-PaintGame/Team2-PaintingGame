#include "SinglePlayerScreen.h"
#include "PauseScreen.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

namespace NCL {
	namespace CSC8508 {

		SinglePlayerScreen::SinglePlayerScreen(Window* window, PaintingGame* paintingGame)
		{
			isPlayingGame = true;
			this->window = window;
			this->paintingGame = paintingGame;
		}
		SinglePlayerScreen::~SinglePlayerScreen()
		{

		}
		PushdownState::PushdownResult SinglePlayerScreen::OnUpdate(float dt, PushdownState** newState)
		{
			while (isPlayingGame)
			{
				if (dt > 0.1f) {
					std::cout << "Skipping large time delta" << std::endl;
					continue; //must have hit a breakpoint or something to have a 1 second frame time!
				}
				if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::PRIOR)) {
					window->ShowConsole(true);
				}
				if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NEXT)) {
					window->ShowConsole(false);
				}

				if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::T)) {
					window->SetWindowPosition(0, 0);
				}
				window->SetTitle("Gametech frame time:" + std::to_string(1000.0f * dt));
				paintingGame->UpdateGame(dt);
				if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::ESCAPE))
				{
					paintingGame->GetGameTechRenderer()->ToggleIsPauseMenu();
					*newState = new PauseScreen(paintingGame);
					return PushdownResult::Push;
				}
				if (paintingGame->GetGameTechRenderer()->GetIsExitPauseMenu() )
				{
					paintingGame->GetGameTechRenderer()->ToggleIsMainMenu();
					paintingGame->GetGameTechRenderer()->ToggleIsExitPauseMenu();
					paintingGame->GetGameTechRenderer()->ToggleIsSinglePlayer();
					return PushdownResult::Pop;
				}
				return PushdownResult::NoChange;
			}
		}
		void SinglePlayerScreen::OnAwake()
		{
			std::cout << "Single Player Mode:\n"
				<< "isSinglePlayer: " << paintingGame->GetGameTechRenderer()->GetIsSinglePlayer() << "\n"
				<< "isMainMenu: " << paintingGame->GetGameTechRenderer()->GetIsMainMenu() << "\n"
				<< "isPauseMenu: " << paintingGame->GetGameTechRenderer()->GetIsPauseMenu() << "\n"
				<< "isExitPauseMenu: " << paintingGame->GetGameTechRenderer()->GetIsExitPauseMenu() << "\n"
				<< "isExitPaintGame: " << paintingGame->GetGameTechRenderer()->GetIsExitPaintGame() << "\n"
				<< "isSplitScreen: " << paintingGame->GetGameTechRenderer()->GetIsSplitScreen() << "\n\n";
		}

	}
}
