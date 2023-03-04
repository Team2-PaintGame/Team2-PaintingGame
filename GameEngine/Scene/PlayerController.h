#pragma once
#include "PlayerBase.h"

namespace NCL {
	using namespace Rendering;
	using namespace CSC8508;

	class PlayerController {
	public:
		PlayerController(PlayerBase* player);
		void Update(float dt);

		// Get the input for moving forward
		virtual bool MoveForward() = 0;
		virtual bool MoveBackward() = 0;
		virtual bool MoveRight() = 0;
		virtual bool MoveLeft() = 0;

		virtual float Pitch() = 0;
		virtual float Yaw() = 0;
	protected:
		PlayerBase* player;
	};
}

