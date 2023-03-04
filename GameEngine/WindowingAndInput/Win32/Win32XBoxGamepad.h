#include<iostream>
#include<Windows.h>
#include<Xinput.h>
#include "PlayerController.h"

namespace NCL {
	class Win32XBoxGamepad {
	public:
		Win32XBoxGamepad() : deadzoneX(0.05f), deadzoneY(0.02f), leftStickX(0.0f), leftStickY(0.0f), rightStickX(0.0f), rightStickY(0.0f) {}
		Win32XBoxGamepad(float dzX, float dzY) : deadzoneX(dzX), deadzoneY(dzY) {}

		bool	GetButtonDown(unsigned int i);


		float leftStickX;
		float leftStickY;
		float rightStickX;
		float rightStickY;
		float leftTrigger;
		float rightTrigger;

		int  GetPort();
		XINPUT_GAMEPAD* GetState();
		bool CheckConnection();
		bool Refresh();
		bool IsPressed(WORD);
	protected:
		int cId;
		XINPUT_STATE state;
		float deadzoneX;
		float deadzoneY;
	};

	class XboxController : public PlayerController {
	public:
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

		float Pitch() override {
			gamepad.rightStickY;
		}
		float Yaw() override {
			gamepad.rightStickX;
		}

		void Connect() {

		}
	protected:
		Win32XBoxGamepad gamepad;
		bool connected = false;
	};
}