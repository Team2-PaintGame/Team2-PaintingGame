#include "ParticleSystem.h"
#include "Utils.h"

using namespace NCL;

Emitter::Emitter(MeshGeometry* emitterMesh) {
	this->emitterMesh = emitterMesh;
	emissionDirections = emitterMesh->GetNormalData();
	vIter = emissionDirections.begin();
}

Emitter::Emitter(float angle) {
	this->angle = angle;
}

Vector3 Emitter::GetEmissionDirection() {
	if (emitterMesh) {
		Vector3 dir = *vIter;
		vIter = vIter + 1 == emissionDirections.end() ? emissionDirections.begin() : vIter + 1;
		return dir;
	}
	else if (transform) {
		float pitch = fmod(rand(), (2.0f * angle + 1) - angle);
		float yaw	= fmod(rand(), (2.0f * angle + 1) - angle);
		float roll	= fmod(rand(), (2.0f * angle + 1) - angle);

		return
			(Quaternion::AxisAngleToQuaterion(Vector3(-1, 0, 0), pitch)
			* Quaternion::AxisAngleToQuaterion(Vector3(0, -1, 0), yaw)
			* Quaternion::AxisAngleToQuaterion(Vector3(0, 0, -1), roll)).ToEuler() 
			* transform->GetMatrix().GetColumn(2); // forward direction
	}
	else {
		return Vector3();
	}
}
