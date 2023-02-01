#pragma once

#include "Camera.h"
#include "PlayerBase.h"

namespace NCL {
	using namespace Rendering;
	using namespace CSC8508;

	class PlayerController {
	public:
		PlayerController(Camera* cam, GameObject* player);

		void Update(float dt);

	protected:
		void UpdateKeys();

	protected:
		Camera* camera;
		GameObject* player_object;
	};
}

