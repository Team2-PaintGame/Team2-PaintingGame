#pragma once
#include "GameObject.h"

namespace NCL {
	
	using namespace CSC8503;
	class PlayerBase : public GameObject {
		PlayerBase() = default;
		virtual void Update(float dt) {}
		virtual ~PlayerBase() {}
	};
}

