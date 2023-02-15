#include "SplitScreen.h"
#include "PauseScreen.h"
#include "MenuHandler.h"

#include <imgui_impl_win32.h>
#include <imgui_impl_opengl3.h>
#include <Win32Window.h>

namespace NCL {
	namespace CSC8508 {

		SplitScreen::SplitScreen(Window* window, MenuHandler* menu)
		{
			this->window = window;
			this->menuHandler = menu;
			menuHandler->SetGameState(GameState::SplitScreen);


			this->paintingGame = new PaintingGame(menu);
			paintingGame->GetGameTechRenderer()->SetRenderMode(GameTechRenderer::RenderMode::SplitScreen);

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			//Init Win32
			//ImGui_ImplWin32_Init(dynamic_cast<NCL::Win32Code::Win32Window*>(window)->GetHandle());
			//Init OpenGL Imgui Implementation
			ImGui_ImplOpenGL3_Init();
			// Setup style
			//ImGui::StyleColorsClassic();
		}
		SplitScreen::~SplitScreen()
		{
			delete paintingGame;

			// Cleanup
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();
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
			paintingGame->InitSecondPlayer();
			paintingGame->InitSecondCamera();
		}

		void SplitScreen::OnSleep() {

		}

	}
}