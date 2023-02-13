#include "MenuHandler.h"

#include <imgui_impl_win32.h>
#include <imgui_impl_opengl3.h>

namespace NCL {

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
		if (gameState == MainMenu) {
			ShowMainMenuWindow();
		}
		if (gameState == PauseMenu) {
			ShowPauseMenuWindow();
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
			//SetGameState(LAN);
		}
		if (ImGui::Button("Exit"))
		{
			SetGameState(ExitGame);
		}
		ImGui::End();
	}

	void MenuHandler::ShowPauseMenuWindow()
	{
		bool isMainMenu = (gameState == MainMenu);
		ImGui::Begin("Pause Menu", &isMainMenu);
		if (ImGui::Button("Resume"))
		{
			SetGameState(previousGameState);
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
}