#pragma once
#include "Light.h"

namespace NCL::Rendering {
	class DirectionalLight : public Light {
	public:
		DirectionalLight() = default;
		DirectionalLight(Vector3 direction) : Light(), direction(direction) {}
	protected:
		Vector3 direction;
	};
};

