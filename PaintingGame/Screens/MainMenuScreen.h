#pragma once
#include "ScreenManager.h"

namespace NCL {
	namespace CSC8508 {
		enum GameMode {
			None,
			SinglePlayer,
			SplitScreen,
			Networked,
		};
		class MainMenuScreen : public BaseScreen
		{
		public:
			MainMenuScreen(ScreenManager* screenManager, SceneNode* sceneNode) : BaseScreen(screenManager, sceneNode) {}
			//virtual ~MainMenuScreen();
			void OnAwake() override;
		protected:
			virtual void MenuFrame();
			virtual PushdownResult onStateChange(PushdownState** newState);

			GameMode gameModeSelected = GameMode::None;
		};

	}
}
