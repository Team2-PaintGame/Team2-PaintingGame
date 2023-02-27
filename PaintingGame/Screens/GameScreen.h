#pragma once
#include "BaseScreen.h"

namespace NCL {
	namespace CSC8508 {
		class SplashScreen : public BaseScreen
		{
		public:
			SplashScreen();
			~SplashScreen();
			
		protected:
			virtual void MenuFrame();
		};

	}
}
