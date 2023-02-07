#include "IntroScreen.h"
#include "SinglePlayerScreen.h"
#include "SplitScreen.h"
#include "LanScreen.h"
#include <iostream>

namespace NCL {
	namespace CSC8508 {
		IntroScreen::IntroScreen() 
		{
			isSinglePlayer, isSplitScreen, isLanScreen, isExit = false;
		}
		IntroScreen::~IntroScreen()
		{

		}
		PushdownState::PushdownResult IntroScreen::OnUpdate(float dt, PushdownState** newState)  
		{
			if (isSinglePlayer)
			{
				*newState = new SinglePlayerScreen();
				isSinglePlayer = !isSinglePlayer;
				return PushdownResult::Push;
			}
			if (isSplitScreen)
			{
				*newState = new SplitScreen();
				isSplitScreen = !isSplitScreen;
				return PushdownResult::Push;
			}
			if (isLanScreen)
			{
				*newState = new LanScreen();
				isLanScreen = !isLanScreen;
				return PushdownResult::Push;
			}
			if (isExit)
			{
				isExit = !isExit;
				return PushdownResult::Pop;
			}
			return PushdownResult::NoChange;
		}
		void IntroScreen::OnAwake() 
		{
			// ImGui Main Menu
			std::cout << "Welcome to Splat at the Museum\n";
		}


	}
}