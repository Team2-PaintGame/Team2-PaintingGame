#pragma once
#ifdef __ORBIS__
#include "pad.h"
#include "InputBase.h"

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
}
#endif