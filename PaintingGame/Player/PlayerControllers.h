#pragma once
#include <Window.h>
#include <XBoxGamepad.h>
#include "PlayerController.h"
#include <PS4Gamepad.h>

namespace NCL {
#ifdef _WIN32
	class Win32Controller : public PlayerController {
	public:
		Win32Controller(PlayerBase* player) : PlayerController(player) {}

		// Get the input for moving forward from the Xbox controller
		bool MoveForward() override {
			return Window::GetKeyboard()->KeyDown(NCL::KeyboardKeys::W);
		}
		bool MoveBackward() override {
			return Window::GetKeyboard()->KeyDown(NCL::KeyboardKeys::S);
		}
		bool MoveRight() override {
			return Window::GetKeyboard()->KeyDown(NCL::KeyboardKeys::D);
		}
		bool MoveLeft() override {
			return Window::GetKeyboard()->KeyDown(NCL::KeyboardKeys::A);
		}

		bool Shoot() override {
			return Window::GetMouse()->ButtonPressed(NCL::MouseButtons::RIGHT);
		}

		float ViewDy() override {
			return Window::GetMouse()->GetRelativePosition().y;
		}
		float ViewDx() override {
			return Window::GetMouse()->GetRelativePosition().x;
		}
		bool Pause() override {
			return Window::GetKeyboard()->KeyDown(NCL::KeyboardKeys::ESCAPE);
		}
	};

	class XBoxController : public PlayerController {
	public:
		XBoxController(PlayerBase* player) : PlayerController(player) { Connect(); }
		// Get the input for moving forward from the Xbox controller
		bool MoveForward() override {
			return gamepad.leftStickY > 0.0f;
		}
		bool MoveBackward() override {
			return gamepad.leftStickY < 0.0f;
		}
		bool MoveRight() override {
			return gamepad.leftStickX > 0.0f;
		}
		bool MoveLeft() override {
			return gamepad.leftStickX < 0.0f;
		}
		bool Shoot() override {
			return false;
		}

		float ViewDy() override {
			return gamepad.rightStickY;
		}
		float ViewDx() override {
			return gamepad.rightStickX;
		}
		bool Pause() override {
			return gamepad.GetButtonDown(VK_PAD_START);
		}

		void Connect() {
			if (!gamepad.Refresh())
			{
				if (wasConnected)
				{
					wasConnected = false;
					std::cout << "Connect a Gamepad !";
				}
			}
			else if (!wasConnected)
			{
				wasConnected = true;
				std::cout << "COntroller connect on port " << gamepad.GetPort() << "\n";
			}
		}
	protected:
		XBoxGamepad gamepad;
		bool wasConnected = false;
	};

	// Concrete factory for creating Win32 Player Controller
	class Win32PlayerControllerFactory : public PlayerControllerFactory {
	public:
		PlayerController* createPlayerController(PlayerBase* player) override {
			return new Win32Controller(player);
		}
	};

	// Concrete factory for creating XBox Player Controller
	class XBoxPlayerControllerFactory : public PlayerControllerFactory {
	public:
		PlayerController* createPlayerController(PlayerBase* player) override {
			return new XBoxController(player);
		}
	};
#endif
#ifdef _ORBIS
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
		bool Shoot() override {
			return false;
		}
		bool Pause() override {
			//return gamepad.GetButtonDown(VK_PAD_START);
		}

		float ViewDy() override {
			return gamepad.GetAxis(0).y;
		}
		float ViewDx() override {
			return gamepad.GetAxis(0).x;
		}
	protected:
		PS4::PS4Gamepad gamepad;
	};

	// Concrete factory for creating PS4 Player Controller
	class PS4ControllerFactory : public PlayerControllerFactory {
	public:
		PlayerController* createPlayerController(PlayerBase* player) override {
			return new PS4Controller(player);
		}
	};
#endif
}