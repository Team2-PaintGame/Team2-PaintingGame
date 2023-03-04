#pragma once
#ifdef _ORBIS
#include "pad.h"
#include "InputBase.h"
#include "PlayerController.h"

namespace NCL {
	namespace PS4 {
		class PS4Gamepad : public InputBase
		{
		public:
			PS4Gamepad();
			~PS4Gamepad();

			void Poll();

		protected:
			void InitController();

			int32_t padHandle;
			ScePadControllerInformation padInfo;
		};
	}

	class PS4Controller : public PlayerController {
	public:
		PS4Controller(PlayerBase* player) : PlayerController(player) {}

		// Get the input for moving forward from the Xbox controller
		bool MoveForward() override {
			return gamepad.GetAxis(0).y > 0.0f;
		}
		bool MoveBackward() override {
			return gamepad.GetAxis(0).y < 0.0f;
		}
		bool MoveRight() override {
			return gamepad.GetAxis(0).x > 0.0f;
		}
		bool MoveLeft() override {
			return gamepad.GetAxis(0).x < 0.0f;
		}

		float Pitch() override {
			gamepad.GetAxis(0).y;
		}
		float Yaw() override {
			gamepad.GetAxis(0).x;
		}
	protected:
		PS4Gamepad gamepad;
	};

	// Concrete factory for creating PS4 Player Controller
	class PS4ControllerFactory : public PlayerControllerFactory {
	public:
		PlayerController* createPlayerController(PlayerBase* player) override {
			return new PS4Controller(player);
		}
	};
}
#endif