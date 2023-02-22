#pragma once

namespace NCL {
	enum GameState {
		MainMenu,
		SinglePlayer,
		SplitScreen,
		LANMenu,
		LAN,
		Server,
		Client,
		PauseMenu,
		ExitPauseMenu,
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
		void ShowLanMenuWindow();

	private:
		//Pushdown Automata Game States
		GameState gameState = MainMenu;
		GameState previousGameState = MainMenu;
	};
}