#pragma once
#include "Player.h"

namespace NCL {
	using namespace Rendering;
	using namespace CSC8508;

	class PlayerController {
	public:
		PlayerController(Player* player);
		void Update(float dt);

		// Get the input for moving forward
		virtual bool MoveForward() = 0;
		virtual bool MoveBackward() = 0;
		virtual bool MoveRight() = 0;
		virtual bool MoveLeft() = 0;
		virtual bool Shoot() = 0;
		virtual bool Pause() = 0;

		virtual float ViewDx() = 0;
		virtual float ViewDy() = 0;
		virtual const Vector2& GetCursorPosition(float dt) = 0;
	protected:
		Player* player;
	};

	// The abstract factory interface for player controller
	class PlayerControllerFactory {
	public:
		virtual ~PlayerControllerFactory() {}
		virtual PlayerController* createPlayerController(Player* player) = 0;
	};
}

