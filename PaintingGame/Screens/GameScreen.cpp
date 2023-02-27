#include "SplashScreen.h"

void NCL::CSC8508::SplashScreen::MenuFrame() {
	ImGui::Begin("Painting Game");
	if (ImGui::Button("Play Game"))
	{
		//transition to main menu
	}
	ImGui::End();
}
