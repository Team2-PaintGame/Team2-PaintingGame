#pragma once
#include "Player.h"

namespace NCL {
	using namespace Rendering;
	using namespace CSC8508;
	class PlayerController {
	public:
		PlayerController(Player* player);
		virtual void Update(float dt);

		// Get the input for moving forward
		virtual bool MoveForward() = 0;
		virtual bool MoveBackward() = 0;
		virtual bool MoveRight() = 0;
		virtual bool MoveLeft() = 0;
		virtual bool Shoot() = 0;
		virtual bool Pause() = 0;

		virtual float ViewDx() = 0;
		virtual float ViewDy() = 0;
		Vector2 GetCursorPosition(float dt) { return Vector2(); }
		virtual ~PlayerController() {}
	protected:
		Player* player;
		Vector2 cursorPosition;
	};

	// The abstract factory interface for player controller
	class PlayerControllerFactory {
	public:
		enum class Type {
			None,
			PS4,
			XBox,
			Win32,
		};
		virtual ~PlayerControllerFactory() {}
		virtual PlayerController* createPlayerController(Player* player) = 0;
		const Type& GetType() const { return type; }
	protected:
		Type type = Type::None;
	};
}

