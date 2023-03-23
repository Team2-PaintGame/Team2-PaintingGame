#include "GameOverScreen.h"
#include "Debug.h"

using namespace NCL;
using namespace CSC8508;

void GameOverScreen::OnAwake()
{
	switch (winner)
	{
	case 1: {
		victory_string = "Well Done! Blue Team won!";
	}; break;
	case 2: {
		victory_string = "Well Done! Red Team Won!";
	}; break;
	default: {
		victory_string = "Well Done! It's a Draw!";
	};
	}
}

void GameOverScreen::MenuFrame() {
#ifdef _WIN32
	ImGui::Begin("Game Over!");
	ImGui::Text(victory_string.c_str());
	if (ImGui::Button("Exit")) {
		command = ScreenCommand::Exit;
	}
	ImGui::End();
#endif
}

PushdownState::PushdownResult GameOverScreen::OnUpdate(float dt, PushdownState** newState)
{
	return BaseScreen::OnUpdate(dt, newState);
}

PushdownState::PushdownResult GameOverScreen::onStateChange(PushdownState** newState) {
	switch (command)
	{
		case ScreenCommand::Exit: 
			return PushdownResult::Reset2;
		default: 
			return PushdownResult::NoChange;
	}
}
