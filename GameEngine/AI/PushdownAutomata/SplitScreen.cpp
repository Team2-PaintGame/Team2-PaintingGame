#include "SplitScreen.h"
#include "PauseScreen.h"
#include "MenuHandler.h"

#include <imgui_impl_win32.h>
#include <imgui_impl_opengl3.h>
#include <Win32Window.h>

namespace NCL {
	namespace CSC8508 {

		SplitScreen::SplitScreen(Window* window, GameTechRenderer* rend, GameWorld* gameWorld, reactphysics3d::PhysicsCommon* physicsCommon, MenuHandler* menu)
		{
			this->window = window;
			this->menuHandler = menu;
			this->renderer = rend;
			this->gameWorld = gameWorld;
			this->physicsCommon = physicsCommon;

			reactphysics3d::PhysicsWorld* physicsWorld = physicsCommon->createPhysicsWorld();

			menuHandler->SetGameState(GameState::SplitScreen);


			this->paintingGame = new PaintingGame(renderer,gameWorld,physicsCommon, menuHandler,false);
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
				*newState = new PauseScreen(paintingGame, menuHandler);
				return PushdownResult::Push;
			}break;

			}
		}
		void SplitScreen::OnAwake()
		{
			if (menuHandler->GetGameState() == GameState::ExitPauseMenu) // Resume game
			{
				menuHandler->SetGameState(GameState::SplitScreen);
			}
			else if(menuHandler->GetGameState() == GameState::SplitScreen) {
				paintingGame->InitSecondPlayer();
				paintingGame->InitSecondCamera();
			}
			
		}

		void SplitScreen::OnSleep() {

		}

	}
}