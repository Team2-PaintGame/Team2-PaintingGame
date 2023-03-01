#include "GameScreen.h"

using namespace NCL;
using namespace CSC8508;

void GameScreen::OnAwake() {
	isMenuDisplayed = false;
}

void GameScreen::MenuFrame() {
	ImGui::Begin("Painting Game");
	if (ImGui::Button("Pause")) {
		isMenuDisplayed = true;
	}
	if (ImGui::Button("Quit Game")) {
		command = ScreenCommand::Exit;
	}
	ImGui::End();
}

PushdownState::PushdownResult GameScreen::onStateChange(PushdownState** newState) {
	switch (command) {
		case ScreenCommand::Exit: {
			return PushdownResult::Pop;
		}
		default: {
			return PushdownResult::NoChange;
		}
	}
}
