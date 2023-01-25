#pragma once
#include "Vector3.h"
/*
* The diffuse intensity of a light is the aspect of the light that interacts with diffuse material color,  
* The specular intensity of a light is what interacts with specular material color.
* The ambient intensity of a light in OpenGL is added to the general level of ambient light.
* Light intensity refers to the strength or amount of light produced by a specific lamp source. Range: 0 - 1
* Read more here: https://math.hws.edu/graphicsbook/c4/s1.html
*/

namespace NCL::Rendering {
	using namespace NCL::Maths;
	class Light {
	public:
		Light() = default;
		Light(Vector3 ambient, Vector3 diffuse, Vector3 specular, float intensity) {
			this->ambient = ambient;
			this->diffuse = diffuse;
			this->specular = specular;
			this->intensity = intensity;
		}

		virtual ~Light() {}
		void SetIntensity(float intensity) { this->intensity = intensity; }
		void SetDiffuseColor(Vector3 diffuse) { this->diffuse = diffuse; }

	protected:
		Vector3 ambient = Vector3(0.5f);		// The ambient light is usually set to a low intensity because we don't want the ambient color to be too dominant.
		Vector3 diffuse = Vector3(1.0f);		// exact color we want the light to have
		Vector3 specular = Vector3(1.0f);		// The specular component is usually kept at vec3(1.0) shining at full intensity.
		float intensity = 1.0f;					// how bright the diffused color of the light is
	};
};


