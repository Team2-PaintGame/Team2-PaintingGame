#pragma once
#include "Vector3.h"
#include "GameObject.h"

namespace NCL {
	using namespace Maths;
	using namespace CSC8508;

	class Particle {
		Vector3 position;
		Vector3 velocity;
	};
	class ParticleSystem : public GameObject {
	public:
		
		//this will have meshgeometry

		//sending to shader
		//one particle system
		//binds the mesh
		//
	protected:
		
	};
}