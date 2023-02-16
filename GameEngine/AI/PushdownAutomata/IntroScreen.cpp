#include "IntroScreen.h"
#include "SinglePlayerScreen.h"
#include "SplitScreen.h"
#include "LanScreen.h"
#include "GameTechRenderer.h"
#include <iostream>
#include "MenuHandler.h"

//#include "imgui.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_opengl3.h>
#include <Win32Window.h>


namespace NCL {
	namespace CSC8508 {
		IntroScreen::IntroScreen(Window* window, PaintingGame* g)
		{
			this->window = window;

			menuHandler = new MenuHandler();
			menuHandler->SetGameState(GameState::MainMenu);

			paintingGame = new PaintingGame(menuHandler, false);
			paintingGame->GetGameTechRenderer()->SetRenderMode(GameTechRenderer::RenderMode::MainMenu);

			
		}
		IntroScreen::~IntroScreen()
		{
			delete paintingGame;
			delete menuHandler;
		}
		PushdownState::PushdownResult IntroScreen::OnUpdate(float dt, PushdownState** newState)
		{
			paintingGame->UpdateGame(dt);

			NCL::GameState gameState = menuHandler->GetGameState();
			switch (gameState) {

				case GameState::SinglePlayer: {
					*newState = new SinglePlayerScreen(window, menuHandler);
					return PushdownResult::Push;
				}	break;

				case GameState::SplitScreen: {
					*newState = new SplitScreen(window, menuHandler);
					return PushdownResult::Push;
				}break;

				case GameState::LAN: {
					*newState = new LanScreen(window, menuHandler);
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
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			//Init Win32
			ImGui_ImplWin32_Init(dynamic_cast<NCL::Win32Code::Win32Window*>(window)->GetHandle());
			//Init OpenGL Imgui Implementation
			ImGui_ImplOpenGL3_Init();
			// Setup style
			ImGui::StyleColorsClassic();
		}

		void IntroScreen::OnSleep()
		{
			// Cleanup
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();
		}
	}
}

