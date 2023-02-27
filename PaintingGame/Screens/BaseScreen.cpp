#include "BaseScreen.h"

using namespace NCL;
using namespace CSC8508;

BaseScreen::BaseScreen(SceneNode* sceneNode) {
	this->sceneNode = sceneNode;
}

BaseScreen::~BaseScreen() {
}

PushdownState::PushdownResult BaseScreen::OnUpdate(float dt, PushdownState** newState) {
	if (!isMenuDisplayed) {
		sceneNode->Update(dt);
	}
	return onStateChange(newState);
}

//void BaseScreen::RenderMenu() {
//	// Start the Dear ImGui frame
//	ImGui_ImplOpenGL3_NewFrame();
//	ImGui_ImplWin32_NewFrame();
//	ImGui::NewFrame();
//
//	if (isMenuDisplayed) {
//		MenuFrame();
//	}
//
//	ImGui::EndFrame();
//}
