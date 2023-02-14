#include "SinglePlayerScreen.h"
#include "PauseScreen.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include "MenuHandler.h"

namespace NCL {
	namespace CSC8508 {

		SinglePlayerScreen::SinglePlayerScreen(Window* window, MenuHandler* menu)
		{
			isPlayingGame = true;
			this->window = window;
			this->menuHandler = menu;
			menuHandler->SetGameState(GameState::SinglePlayer);

			this->paintingGame = new PaintingGame(menuHandler, false);
			paintingGame->GetGameTechRenderer()->SetRenderMode(GameTechRenderer::RenderMode::SingleViewport);
		}
		SinglePlayerScreen::~SinglePlayerScreen()
		{
			delete paintingGame;
		}
		PushdownState::PushdownResult SinglePlayerScreen::OnUpdate(float dt, PushdownState** newState)
		{
			if (dt > 0.1f) {
				std::cout << "Skipping large time delta" << std::endl;
				return PushdownResult::NoChange; //must have hit a breakpoint or something to have a 1 second frame time!
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
			if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::ESCAPE)){
				menuHandler->SetGameState(GameState::PauseMenu);
			}
			window->SetTitle("Gametech frame time:" + std::to_string(1000.0f * dt));
			paintingGame->UpdateGame(dt);
			GameState gameState = menuHandler->GetGameState();
			switch (gameState) {
				case GameState::SinglePlayer: {
					return PushdownResult::NoChange;
				}	break;

				case GameState::MainMenu: {
					return PushdownResult::Pop;
				}break;

				case GameState::PauseMenu: {
					*newState = new PauseScreen(paintingGame);
					return PushdownResult::Push;
				}break;

			}
		}
		void SinglePlayerScreen::OnAwake()
		{

		}

	}
}
