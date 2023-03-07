#pragma once
#ifdef _WIN32
#include<iostream>
#include<Windows.h>
#include<Xinput.h>
#include "PlayerController.h"
#include "Win32Window.h"

namespace NCL {
	class XBoxGamepad {
	public:
		XBoxGamepad() : deadzoneX(0.05f), deadzoneY(0.02f), leftStickX(0.0f), leftStickY(0.0f), rightStickX(0.0f), rightStickY(0.0f) {}
		XBoxGamepad(float dzX, float dzY) : deadzoneX(dzX), deadzoneY(dzY) {}

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

	// Concrete factory for creating XBox Player Controller
	class XBoxPlayerControllerFactory : public PlayerControllerFactory {
	public:
		PlayerController* createPlayerController(PlayerBase* player) override {
			return new XBoxController(player);
		}
	};
}
#endif