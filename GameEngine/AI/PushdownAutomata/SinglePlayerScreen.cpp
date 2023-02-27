#include "SinglePlayerScreen.h"
#include "PauseScreen.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include "MenuHandler.h"

#include <Win32Window.h>

namespace NCL {
	namespace CSC8508 {

		SinglePlayerScreen::SinglePlayerScreen(Window* window, GameTechRenderer* rend, GameWorld* gameWorld, reactphysics3d::PhysicsCommon* physicsCommon, MenuHandler* menu)
		{
			this->window = window;
			this->menuHandler = menu;
			this->renderer = rend;
			this->gameWorld = gameWorld;
			this->physicsCommon = physicsCommon;		

			reactphysics3d::PhysicsWorld* physicsWorld = physicsCommon->createPhysicsWorld();

			paintingGame = new PaintingGame(renderer, gameWorld, physicsCommon, menuHandler, false);
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

			

			GameState gameState = menuHandler->GetGameState();
			switch (gameState) {
				case GameState::SinglePlayer: {
					// return PushdownResult::NoChange;
				}	break;

				case GameState::MainMenu: {
					return PushdownResult::Pop;
				}break;

				case GameState::PauseMenu: {
					*newState = new PauseScreen(paintingGame, menuHandler);
					return PushdownResult::Push;
				}break;

			}

			paintingGame->UpdateGame(dt);
			return PushdownResult::NoChange;
		}
		void SinglePlayerScreen::OnAwake()
		{
			if (menuHandler->GetGameState() == GameState::ExitPauseMenu) // Resume game
			{
				menuHandler->SetGameState(GameState::SinglePlayer);
			}
		}
		
		void SinglePlayerScreen::OnSleep()
		{
			
		}

	}
}
