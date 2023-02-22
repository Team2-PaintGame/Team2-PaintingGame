#include "LanScreen.h"
#include "PauseScreen.h"
#include "MenuHandler.h"

#include <imgui_impl_win32.h>
#include <imgui_impl_opengl3.h>
#include <Win32Window.h>

namespace NCL {
	namespace CSC8508 {
		LanScreen::LanScreen(Window* window, GameTechRenderer* rend, GameWorld* gameWorld, reactphysics3d::PhysicsCommon* physicsCommon, MenuHandler* menu)
		{
			this->window = window;
			isPlayingGame = true;
			menuHandler = menu;
			this->renderer = rend;
			this->gameWorld = gameWorld;
			this->physicsCommon = physicsCommon;
			menuHandler->SetGameState(GameState::LAN);

			this->paintingGame = new NetworkedGame(window,renderer,gameWorld,physicsCommon, menuHandler);
			paintingGame->GetGameTechRenderer()->SetRenderMode(GameTechRenderer::RenderMode::SingleViewport);
		}
		LanScreen::~LanScreen()
		{
			delete paintingGame;
		}
		PushdownState::PushdownResult LanScreen::OnUpdate(float dt, PushdownState** newState)
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
			case GameState::LAN: {
				return PushdownResult::NoChange;
			}	break;

			case GameState::MainMenu: {
				return PushdownResult::Pop;
			}break;

			case GameState::PauseMenu: {
				*newState = new PauseScreen(paintingGame, menuHandler);
				return PushdownResult::Push;
			}break;
			}

		}
		void LanScreen::OnAwake()
		{

		}
	}
}

