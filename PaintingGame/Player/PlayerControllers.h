#pragma once
#include <Window.h>
#include <XBoxGamepad.h>
#include "PlayerController.h"
#include <PS4Gamepad.h>
#include "Vector2.h"

namespace NCL {
#ifdef _WIN32
	class Win32Controller : public PlayerController {
	public:
		Win32Controller(Player* player) : PlayerController(player) {}

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
		bool Pause() override {
			return Window::GetKeyboard()->KeyDown(NCL::KeyboardKeys::ESCAPE);
		}

		float ViewDy() override {
			return Window::GetMouse()->GetRelativePosition().y;
		}
		float ViewDx() override {
			return Window::GetMouse()->GetRelativePosition().x;
		}
		//const Vector2& GetCursorPosition(float dt) override {

		//	// these are not actual positions but the change between last frame and now
		//	float speed = 2; //pixels per second
		//	float h = speed * Window::GetMouse()->GetRelativePosition().x * dt;
		//	float v = speed * Window::GetMouse()->GetRelativePosition().y * dt;

		//	// add the changes to the actual cursor position
		//	cursorPosition.x += h;
		//	cursorPosition.y += v;

		//	return cursorPosition;
		//}
	};

	class XBoxController : public PlayerController {
	public:
		XBoxController(Player* player) : PlayerController(player), gamepad() { Connect(); }
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
			return gamepad.IsPressed(XINPUT_GAMEPAD_RIGHT_SHOULDER);
		}
		bool Pause() override {
			return gamepad.IsPressed(XINPUT_GAMEPAD_START);
		}

		float ViewDy() override {
			return gamepad.rightStickY;
		}
		float ViewDx() override {
			return gamepad.rightStickX;
		}
		//const Vector2& GetCursorPosition(float dt) override {
		//	// these are not actual positions but the change between last frame and now
		//	float speed = 2; //pixels per second
		//	float h = speed * gamepad.rightStickX * dt;
		//	float v = speed * gamepad.rightStickY * dt;

		//	// add the changes to the actual cursor position
		//	cursorPosition.x += h;
		//	cursorPosition.y += v;

		//	return cursorPosition;
		//}

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
				std::cout << "Controller connect on port " << gamepad.GetPort() << "\n";
			}
		}

		void Update(float dt) override
		{
			Connect();
			PlayerController::Update(dt);
		}
	protected:
		XBoxGamepad gamepad;
		bool wasConnected = false;
	};

	// Concrete factory for creating Win32 Player Controller
	class Win32PlayerControllerFactory : public PlayerControllerFactory {
	public:
		Win32PlayerControllerFactory() { Type::Win32; }
		PlayerController* createPlayerController(Player* player) override {
			return new Win32Controller(player);
		}
	};

	// Concrete factory for creating XBox Player Controller
	class XBoxPlayerControllerFactory : public PlayerControllerFactory {
	public:
		XBoxPlayerControllerFactory() { Type::XBox; }
		PlayerController* createPlayerController(Player* player) override {
			return new XBoxController(player);
		}
	};
#endif
#ifdef __ORBIS__
	class PS4Controller : public PlayerController {
	public:
		PS4Controller(Player* player) : PlayerController(player) {}

		// Get the input for moving forward from the Xbox controller
		bool MoveForward() override {
			return gamepad.GetAxis(0).y < 0.5f;
		}
		bool MoveBackward() override {
			return gamepad.GetAxis(0).y > -0.5f;
		}
		bool MoveRight() override {
			return gamepad.GetAxis(0).x > 0.5f;
		}
		bool MoveLeft() override {
			return gamepad.GetAxis(0).x < -0.5f;
		}
		bool Shoot() override {
			return false; // gamepad.GetButtonDown(gamepad.GetButton(0));
		}
		bool Pause() override {
			//return gamepad.GetButtonDown(VK_PAD_START);
			return false;
		}

		float ViewDy() override {
			return gamepad.GetAxis(1).y;
		}
		float ViewDx() override {
			return gamepad.GetAxis(1).x;
		}
		//const Vector2& GetCursorPosition(float dt) override {
		//	// these are not actual positions but the change between last frame and now
		//	float speed = 2; //pixels per second
		//	float h = speed * gamepad.GetAxis(1).x * dt;
		//	float v = speed * gamepad.GetAxis(1).y * dt;

		//	// add the changes to the actual cursor position
		//	cursorPosition.x += h;
		//	cursorPosition.y += v;

		//	return cursorPosition;
		//}

		void Update(float dt) override
		{
			gamepad.Poll();
			PlayerController::Update(dt);
		}
	protected:
		PS4::PS4Gamepad gamepad;
	};

	// Concrete factory for creating PS4 Player Controller
	class PS4ControllerFactory : public PlayerControllerFactory {
	public:
		PS4ControllerFactory() { type = Type::PS4; }
		PlayerController* createPlayerController(Player* player) override {
			return new PS4Controller(player);
		}
	};
#endif
}