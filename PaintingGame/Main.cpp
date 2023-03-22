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
	Window* w = Window::CreateGameWindow("Painting Game", 1280, 720);
	GameManager gameManager(w);
	if (!w->HasInitialised()) {
		return -1;
	}

	SoundSystem::Initialise();
	Sound::AddSound("H:/2022/csc8508/project/Assets/Sounds/14615__man__canon.wav");
	SoundEmitter* s = new SoundEmitter();

	s->SetSound(Sound::GetSound("H:/2022/csc8508/project/Assets/Sounds/14615__man__canon.wav"));
	s->SetLooping(true);
	s->SetPosition(Vector3(0, 0, 0));

	SoundSystem::GetSoundSystem()->AddSoundEmitter(s);
	SoundSystem::GetSoundSystem()->SetMasterVolume(1.0);

	w->ShowOSPointer(true);
	w->LockMouseToWindow(true);

	w->GetTimer()->GetTimeDeltaSeconds(); //Clear the timer so we don't get a larget first dt!

	while (w->UpdateWindow()) {
		float dt = w->GetTimer()->GetTimeDeltaSeconds();
		if (dt > 0.1f) {
			std::cout << "Skipping large time delta" << std::endl;
			continue; //must have hit a breakpoint or something to have a 1 second frame time!
		}

		SoundSystem::GetSoundSystem()->Update(dt);

		w->SetTitle("Painting Game Frame time: " + std::to_string(1000.0f * dt));
		if (!gameManager.RunGame(dt))
			return 0;
	}

	//Sound::DeleteSounds();
	SoundSystem::Destroy();

	Window::DestroyGameWindow();
}