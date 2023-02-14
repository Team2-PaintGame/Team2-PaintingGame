#include "SplitScreen.h"
#include "PauseScreen.h"
#include "MenuHandler.h"


namespace NCL {
	namespace CSC8508 {

		SplitScreen::SplitScreen(Window* window, MenuHandler* menu)
		{
			this->window = window;
			this->menuHandler = menu;
			menuHandler->SetGameState(GameState::SplitScreen);


			this->paintingGame = new PaintingGame(menu);
			paintingGame->GetGameTechRenderer()->SetRenderMode(GameTechRenderer::RenderMode::SplitScreen);
		}
		SplitScreen::~SplitScreen()
		{
			delete paintingGame;
		}
		PushdownState::PushdownResult SplitScreen::OnUpdate(float dt, PushdownState** newState)
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
			if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::ESCAPE)) {
				menuHandler->SetGameState(GameState::PauseMenu);
			}
			window->SetTitle("Gametech frame time:" + std::to_string(1000.0f * dt));
			paintingGame->UpdateGame(dt);

			GameState gameState = menuHandler->GetGameState();
			switch (gameState) {
			case GameState::SplitScreen: {
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
		void SplitScreen::OnAwake()
		{
			paintingGame->InitSecondPlayer();
			paintingGame->InitSecondCamera();
		}

		void SplitScreen::OnSleep() {

		}

	}
}