#pragma once
#include "Vector3.h"
#include "Quaternion.h"

namespace NCL {
	using namespace Maths;
	namespace CSC8508 {
		class GameObject;
		class NetworkState	{
		public:
			NetworkState();
			virtual ~NetworkState();

			Vector3		position;
			Quaternion	orientation;
			int			stateID;
		};
	}
}

