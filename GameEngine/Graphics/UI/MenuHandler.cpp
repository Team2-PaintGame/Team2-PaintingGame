#include "MenuHandler.h"

#include <imgui_impl_win32.h>
#include <imgui_impl_opengl3.h>

namespace NCL {

	MenuHandler::MenuHandler() {
	}

	GameState MenuHandler::GetGameState()
	{
		return gameState;
	}

	void MenuHandler::Update(float dt)
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		switch (gameState)
		{
		case MainMenu:
			ShowMainMenuWindow();
			break;
		case PauseMenu:
			ShowPauseMenuWindow();
			break;
		case LAN:
			ShowLanMenuWindow();
			break;
		default:
			//ShowMainMenuWindow();
			break;
		}
		ImGui::EndFrame();
	}

	void MenuHandler::SetGameState(GameState gameState)
	{
		previousGameState = this->gameState;
		this->gameState = gameState;
	}

	void MenuHandler::ShowMainMenuWindow()
	{
		bool isMainMenu = (gameState == MainMenu);

		ImGui::Begin("Splat Main Menu", &isMainMenu);
		ImGui::Text("This is going to be the splat main menu!");

		if (ImGui::Button("Single Player"))
		{
			SetGameState(SinglePlayer);
		}
		if (ImGui::Button("Split Screen"))
		{
			SetGameState(SplitScreen);
		}
		if (ImGui::Button("LAN"))
		{
			SetGameState(LAN);
		}

		if (ImGui::Button("Exit"))
		{
			SetGameState(ExitGame);
		}
		ImGui::End();
	}

	void MenuHandler::ShowPauseMenuWindow()
	{
		bool isPauseMenu = (gameState == PauseMenu);
		
		ImGui::Begin("Pause Menu", &isPauseMenu);
		if (ImGui::Button("Resume"))
		{
			SetGameState(ExitPauseMenu);
		}
		if (ImGui::Button("Toggle Debug Info"))
		{
			//ToggleDebugInfo();
		}
		if (ImGui::Button("Exit to Main Menu"))
		{
			SetGameState(MainMenu);
		}
		ImGui::End();
	}
	void MenuHandler::ShowLanMenuWindow()
	{
		bool isLANMainMenu = (gameState == LAN);

		ImGui::Begin("Pause Menu", &isLANMainMenu);
		if (ImGui::Button("Server"))
		{
			SetGameState(Server);
		}
		if (ImGui::Button("Client"))
		{
			SetGameState(Client);
		}
		if (ImGui::Button("Exit to Main Menu"))
		{
			SetGameState(MainMenu);
		}
		ImGui::End();
	}
}