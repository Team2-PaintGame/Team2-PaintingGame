#pragma once
#include "CollisionVolume.h"
#include "Vector3.h"
#include <OBBVolume.h>

namespace NCL {
	using namespace NCL::Maths;
	class AABBVolume : CollisionVolume
	{
	public:
		AABBVolume(const Vector3& halfDims) {
			type		= VolumeType::AABB;
			halfSizes	= halfDims;
		}

		AABBVolume(const OBBVolume& volume) {
			type = VolumeType::AABB;
			halfSizes = volume.GetHalfDimensions();
		}
		~AABBVolume() {

		}

		Vector3 GetHalfDimensions() const {
			return halfSizes;
		}
		void GetAxes(const Transform& worldTransform, std::vector<Vector3>& axes) const {
			axes.push_back(Vector3(1.0f, 0.0f, 0.0f)); //X - Axis
			axes.push_back(Vector3(0.0f, 1.0f, 0.0f)); //Y - Axis
			axes.push_back(Vector3(0.0f, 0.0f, 1.0f)); //Z - Axis
		}
		/*void GetMinMaxVertices(const Transform& worldTransform, Vector3& min, Vector3& max) const {
			min = worldTransform.GetPosition() - halfSizes;
			max = worldTransform.GetPosition() + halfSizes;
		}*/

	protected:
		Vector3 halfSizes;
	};
}
