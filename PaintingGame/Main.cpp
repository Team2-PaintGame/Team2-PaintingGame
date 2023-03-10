#include <Window.h>
#include "Debug.h"

#include "PaintingGame.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_opengl3.h>
//#include <Win32Window.h>
#include "GameManager.h"
#include <windows.h>
#include <stdio.h>
#include <Psapi.h>

using namespace NCL;
using namespace CSC8508;


int main() {
	Window* w = Window::CreateGameWindow("Painting Game", 1280, 720);
	GameManager gameManager(w);
	if (!w->HasInitialised()) {
		return -1;
	}

	w->ShowOSPointer(true);
	w->LockMouseToWindow(true);
	
	w->GetTimer()->GetTimeDeltaSeconds(); //Clear the timer so we don't get a larget first dt!

	while (w->UpdateWindow()) {
		float dt = w->GetTimer()->GetTimeDeltaSeconds();
		if (dt > 0.1f) {
			std::cout << "Skipping large time delta" << std::endl;
			continue; //must have hit a breakpoint or something to have a 1 second frame time!
		}
		w->SetTitle("Painting Game Frame time: " + std::to_string(1000.0f * dt));
		if (!gameManager.RunGame(dt)) {
			return 0;
		}
			
	}

	Window::DestroyGameWindow();
}