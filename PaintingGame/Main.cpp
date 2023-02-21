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

#define NETWORKING_ENABLED	(0)	// (0) - off, (1) - on

using namespace NCL;
using namespace CSC8508;

void GameLoop(Window* window, PaintingGame paintingGame) {
	while (window->UpdateWindow() && !Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE)) {
		float time = window->GetTimer()->GetTotalTimeSeconds();
		float dt = window->GetTimer()->GetTimeDeltaSeconds();
		if (dt > 0.1f) {
			std::cout << "Skipping large time delta" << std::endl;
			continue; //must have hit a breakpoint or something to have a 1 second frame time!
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::PRIOR)) {
			window->ShowConsole(true);
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NEXT)) {
			window->ShowConsole(false);
		}

		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::T)) {
			window->SetWindowPosition(0, 0);
		}
		window->SetTitle("Gametech frame time:" + std::to_string(1000.0f * dt));
		paintingGame.UpdateGame(dt);
	}
}

//void PushdownAutomata(Window* window, PaintingGame* paintingGame) {
void PushdownAutomata(Window* window, ) {
	//PushdownMachine machine(new IntroScreen(window, paintingGame));
	PushdownMachine machine(new IntroScreen(window, nullptr));
	while (window->UpdateWindow()) {
		float dt = window->GetTimer()->GetTimeDeltaSeconds();
		if (!machine.Update(dt)) {
			return;
		}
	}
}

int main() {
	Window* w = Window::CreateGameWindow("CSC8508 Game technology!", 1280, 720);

	if (!w->HasInitialised()) {
		return -1;
	}

	w->ShowOSPointer(true);
	w->LockMouseToWindow(true);
	w->GetTimer()->GetTimeDeltaSeconds(); //Clear the timer so we don't get a larget first dt!


	w->GetTimer()->GetTimeDeltaSeconds(); //Clear the timer so we don't get a larget first dt!

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//Init Win32
	ImGui_ImplWin32_Init(dynamic_cast<NCL::Win32Code::Win32Window*>(w)->GetHandle());
	//Init OpenGL Imgui Implementation
	ImGui_ImplOpenGL3_Init();
	// Setup style
	ImGui::StyleColorsClassic();

	//PushdownAutomata(w, paintingGame);
	PushdownAutomata(w);
	

	// ImGUI Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	Window::DestroyGameWindow();
}