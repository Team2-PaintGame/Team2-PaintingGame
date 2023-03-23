#include <Window.h>
#include "Debug.h"
#include "PaintingGame.h"
#include "GameManager.h"
#include <windows.h>
#include <stdio.h>
#include <Psapi.h>


using namespace NCL;
using namespace CSC8508;

/* Only Required for PS4 */
unsigned int sceLibcHeapExtendedAlloc = 1;			/* Switch to dynamic allocation */
size_t       sceLibcHeapSize = 1024 * 1024 * 1024;	/* Set up heap area upper limit as 256 MiB */

int main() {
	Window* w = Window::CreateGameWindow("Painting Game", 1280, 720);
//	Window* w = Window::CreateGameWindow("Painting Game", 1920, 1080, true);
	GameManager gameManager(w);
	if (!w->HasInitialised()) {
		return -1;
	}

	//SoundSystem::Initialise();
	//Sound::AddSound("H:/2022/csc8508/project/Assets/Sounds/14615__man__canon.wav");
	//SoundEmitter* s = new SoundEmitter();

	//s->SetSound(Sound::GetSound("H:/2022/csc8508/project/Assets/Sounds/14615__man__canon.wav"));
	//s->SetLooping(true);
	//s->SetPosition(Vector3(0, 0, 0));
	//
	//SoundSystem::GetSoundSystem()->AddSoundEmitter(s);
	//SoundSystem::GetSoundSystem()->SetMasterVolume(1.0);

	w->ShowOSPointer(true);
	w->LockMouseToWindow(true);
	
	w->GetTimer()->GetTimeDeltaSeconds(); //Clear the timer so we don't get a larget first dt!

	w->SetConsolePosition(1950, 100);
	while (w->UpdateWindow()) {
		float dt = w->GetTimer()->GetTimeDeltaSeconds();
	//	w->LockMouseToWindow(true);
		if (dt > 0.5f) {
			std::cout << "Skipping large time delta: " << dt << std::endl;
			continue; //must have hit a breakpoint or something to have a 1 second frame time!
		}

		//SoundSystem::GetSoundSystem()->Update(dt);

		w->SetTitle("Painting Game Frame time: " + std::to_string(1000.0f * dt));
		if (!gameManager.RunGame(dt)) {
			return 0;
		}
			
	}

	//Sound::DeleteSounds();
	//SoundSystem::Destroy();

	Window::DestroyGameWindow();
}