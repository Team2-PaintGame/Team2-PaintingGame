#include <Window.h>
#include "Debug.h"

#include "PaintingGame.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_opengl3.h>
#include <Win32Window.h>
#include "NetworkedGame.h"

#include "PushdownMachine.h"
#include "IntroScreen.h"
#include "MenuHandler.h"
#include "GameManager.h"

#define NETWORKING_ENABLED	(0)	// (0) - off, (1) - on

using namespace NCL;
using namespace CSC8508;

void PushdownAutomata(Window* window) {
	//PushdownMachine machine(new IntroScreen(window, paintingGame));
	PushdownMachine machine(new IntroScreen(window));
	while (window->UpdateWindow()) {
		float dt = window->GetTimer()->GetTimeDeltaSeconds();
		if (!machine.Update(dt)) {
			return;
		}
	}
}

int main() {
	Window* w = Window::CreateGameWindow("CSC8508 Game technology!", 1280, 720);
	GameManager gameManager(w);
	if (!w->HasInitialised()) {
		return -1;
	}

	w->ShowOSPointer(true);
	w->LockMouseToWindow(true);
	w->GetTimer()->GetTimeDeltaSeconds(); //Clear the timer so we don't get a larget first dt!


	w->GetTimer()->GetTimeDeltaSeconds(); //Clear the timer so we don't get a larget first dt!

	PushdownAutomata(w);
	

	// ImGUI Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	Window::DestroyGameWindow();
}