#include <Window.h>
#include "Debug.h"

#include "PaintingGame.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_opengl3.h>
//#include <Win32Window.h>
#include "GameManager.h"

using namespace NCL;
using namespace CSC8508;


int main() {
	Window* w = Window::CreateGameWindow("CSC8508 Game technology!", 1280, 720);
	GameManager gameManager(w);
	if (!w->HasInitialised()) {
		return -1;
	}

	w->ShowOSPointer(true);
	w->LockMouseToWindow(true);
	w->GetTimer()->GetTimeDeltaSeconds(); //Clear the timer so we don't get a larget first dt!


	Window::DestroyGameWindow();
}