#include "LanScreen.h"
#include "PauseScreen.h"

namespace NCL {
	namespace CSC8508 {
		LanScreen::LanScreen(Window* window, PaintingGame* paintingGame)
		{
			this->window = window;
			this->paintingGame = paintingGame;
			isPlayingGame = true;
		}
		LanScreen::~LanScreen()
		{

		}
		PushdownState::PushdownResult LanScreen::OnUpdate(float dt, PushdownState** newState)
		{
			while (isPlayingGame)
			{
				if (dt > 0.5f) {
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
				if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::ESCAPE)) {
					paintingGame->GetGameTechRenderer()->SetGameState(GameTechRenderer::GameState::PauseMenu);
				}
				window->SetTitle("Gametech frame time:" + std::to_string(1000.0f * dt));
				paintingGame->UpdateGame(dt);

				GameTechRenderer::GameState gameState = paintingGame->GetGameTechRenderer()->GetGameState();
				switch (gameState) {
				case GameTechRenderer::GameState::LAN: {
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
		void LanScreen::OnAwake()
		{

		}
	}
}
