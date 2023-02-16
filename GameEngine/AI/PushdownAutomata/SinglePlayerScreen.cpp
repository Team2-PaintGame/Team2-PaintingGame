#include "SinglePlayerScreen.h"
#include "PauseScreen.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include "Debug.h"

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
				/*Debug::DrawLine(Vector3(0.0,0.0,0.0), Vector3(0.0,0.0,100.0));
				Debug::DrawLine(Vector3(0.0, 0.0, 0.0), Vector3(70.0, 0.0, 65.0));*/
				Debug::DrawLine(Vector3(-75.0, 0.0, 65.0), Vector3(-75.0, 0.0, -75.0), Debug::RED);
				Debug::DrawLine(Vector3(0.0, 0.0, 0.0), Vector3(-70, 5, -60));
				
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
				if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::ESCAPE)){
					paintingGame->GetGameTechRenderer()->SetGameState(GameTechRenderer::GameState::PauseMenu);
				}
				window->SetTitle("Gametech frame time:" + std::to_string(1000.0f * dt));
				paintingGame->UpdateGame(dt);
				
				GameTechRenderer::GameState gameState = paintingGame->GetGameTechRenderer()->GetGameState();
				switch (gameState) {
					case GameTechRenderer::GameState::SinglePlayer: {
						return PushdownResult::NoChange;
					}	break;

					case GameTechRenderer::GameState::MainMenu: {
						return PushdownResult::Pop;
					}break;

					case GameTechRenderer::GameState::PauseMenu: {
						*newState = new PauseScreen(paintingGame);
						return PushdownResult::Push;
					}break;

				}
			}
		}
		void SinglePlayerScreen::OnAwake()
		{

		}

	}
}
