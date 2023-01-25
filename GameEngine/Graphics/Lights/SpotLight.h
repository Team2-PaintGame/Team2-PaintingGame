#pragma once
#include "PointLight.h"

namespace NCL::Rendering {
	class SpotLight : public PointLight {
	public:
		SpotLight() = default;
		SpotLight(Vector3 position, Vector3 axis, float cutOff) : PointLight(position) {
			this->axis = axis;
			this->cutOff = cutOff;
		}
		SpotLight(Vector3 position) : PointLight(position) {}
		SpotLight(Vector3 position, float constant, float linear, float quadratic) : PointLight(position, constant, linear, quadratic) {}
	protected:
		Vector3 axis = Vector3(0.0f, 1.0f, 0.0f);	// this is the axis of the spotlight, points in this direction receives maximum illumination
		float cutOff = 45.0f;						// cut off angle in degrees, fragment shader will convert this angle to radians
	};
};

