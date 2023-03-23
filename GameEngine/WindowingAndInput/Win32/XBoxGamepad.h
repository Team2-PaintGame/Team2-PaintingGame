#pragma once
#ifdef _WIN32
#include<iostream>
#include<Windows.h>
#include<Xinput.h>

namespace NCL {
	class XBoxGamepad {
	public:
		XBoxGamepad() : deadzoneX(0.2f), deadzoneY(0.2f), leftStickX(0.0f), leftStickY(0.0f), rightStickX(0.0f), rightStickY(0.0f) {}
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
}
#endif