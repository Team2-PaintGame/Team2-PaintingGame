#pragma once

namespace NCL {
	enum GameState {
		MainMenu,
		SinglePlayer,
		SplitScreen,
		LAN,
		PauseMenu,
		ExitGame
	};

	class MenuHandler
	{
	public:
		MenuHandler();

		void Update(float dt);

		GameState GetGameState();
		void SetGameState(GameState gameState);
	private:
		void ShowMainMenuWindow();
		void ShowPauseMenuWindow();

	private:
		//Pushdown Automata Game States
		GameState gameState = MainMenu;
		GameState previousGameState = MainMenu;
	};
}