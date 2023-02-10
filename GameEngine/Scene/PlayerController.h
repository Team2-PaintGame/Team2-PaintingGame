#pragma once

#include "Camera.h"
#include "PlayerBase.h"

class Gamepad;

namespace NCL {
	using namespace Rendering;
	using namespace CSC8508;

	class PlayerController {
	public:
		PlayerController(Camera* cam, GameObject* player);

		void Update(float dt);

	protected:
		void UpdateKeys();
		void ConnectGamePad();
		void UpdateGamePad();

	protected:
		Camera* camera;
		GameObject* playerObject;

		Gamepad* gamepad = NULL;
		bool wasConnected = true;
	};
}

