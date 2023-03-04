/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#pragma once
#include "Window.h"

#ifdef _WIN32
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC			((USHORT) 0x01)
#endif

#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE			((USHORT) 0x02)
#endif

#ifndef HID_USAGE_GENERIC_KEYBOARD
#define HID_USAGE_GENERIC_KEYBOARD		((USHORT) 0x06)
#endif

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include "Win32Mouse.h"
#include "Win32Keyboard.h"
#include "PlayerController.h"

namespace NCL::Win32Code {
	class Win32Mouse;
	class Win32Keyboard;

	class Win32Window : public Window {
	public:
		friend class Window;
		void	LockMouseToWindow(bool lock)		override;
		void	ShowOSPointer(bool show)			override;
		void	SetConsolePosition(int x, int y)	override;
		void	ShowConsole(bool state)				override;
		void	SetFullScreen(bool state)			override;
		void	SetWindowPosition(int x, int y)		override;

		HWND		GetHandle()			const { return windowHandle; }
		HINSTANCE	GetInstance()		const { return windowInstance; }

	protected:
		Win32Window(const std::string& title, int sizeX, int sizeY, bool fullScreen, int offsetX, int offsetY);
		virtual ~Win32Window(void);

		bool	InternalUpdate()	override;
		void	UpdateTitle()		override;

		void					CheckMessages(MSG &msg);
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		HWND			windowHandle;
		HINSTANCE		windowInstance;

		bool			forceQuit;

		bool			active;
		bool			fullScreen;
		bool			lockMouse;
		bool			showMouse;
		bool			mouseLeftWindow;
		bool			maximised;

		Win32Mouse*		winMouse;
		Win32Keyboard*  winKeyboard;
	};

	class Win32Controller : public PlayerController {
	public:
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

		float Pitch() override {
			Window::GetMouse()->GetRelativePosition().y;
		}
		float Yaw() override {
			Window::GetMouse()->GetRelativePosition().x;
		}
	};
}
#endif //_WIN32