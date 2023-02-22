#include "IntroScreen.h"
#include "SinglePlayerScreen.h"
#include "SplitScreen.h"
#include "LanScreen.h"
#include "GameTechRenderer.h"
#include <iostream>
#include "MenuHandler.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_opengl3.h>
#include <Win32Window.h>



namespace NCL {
	namespace CSC8508 {
		IntroScreen::IntroScreen(Window* window)
		{
			this->window = window;

			gameWorld = new GameWorld();
			physicsCommon = new reactphysics3d::PhysicsCommon();

			reactphysics3d::PhysicsWorld* physicsWorld = physicsCommon->createPhysicsWorld();

			menuHandler = new MenuHandler();
			renderer = new GameTechRenderer(*(this->gameWorld), physicsWorld);

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			//Init Win32
			ImGui_ImplWin32_Init(dynamic_cast<NCL::Win32Code::Win32Window*>(window)->GetHandle());
			//Init OpenGL Imgui Implementation
			ImGui_ImplOpenGL3_Init();
			// Setup style
			ImGui::StyleColorsClassic();

			paintingGame = new PaintingGame(renderer, gameWorld, physicsWorld, physicsCommon, menuHandler, false);
		}
		IntroScreen::~IntroScreen()
		{
			delete paintingGame;
			delete menuHandler;
			delete renderer;
			delete gameWorld;
		}
		PushdownState::PushdownResult IntroScreen::OnUpdate(float dt, PushdownState** newState)
		{
			paintingGame->UpdateGame(dt);

			NCL::GameState gameState = menuHandler->GetGameState();
			switch (gameState) {

				case GameState::SinglePlayer: {
					*newState = new SinglePlayerScreen(window, renderer, gameWorld, physicsCommon ,menuHandler);
					return PushdownResult::Push;
				}	break;

				case GameState::SplitScreen: {
					*newState = new SplitScreen(window, renderer, gameWorld, physicsCommon, menuHandler);
					return PushdownResult::Push;
				}break;

				case GameState::LAN: {
					*newState = new LanScreen(window,renderer,gameWorld,physicsCommon, menuHandler);
					return PushdownResult::Push; // Add in when we have LAN game created
				}break;

				case GameState::ExitGame: {
					return PushdownResult::Pop;
				}break;

				case GameState::MainMenu: {
					return PushdownResult::NoChange;
				}break;

			}
		}
		void IntroScreen::OnAwake()
		{
			renderer->SetRenderMode(GameTechRenderer::RenderMode::MainMenu);
			menuHandler->SetGameState(GameState::MainMenu);
		}

		void IntroScreen::OnSleep()
		{
		}
	}
}

