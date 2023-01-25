#pragma once

#include <vector>
#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix4.h"
#include "Matrix3.h"

namespace NCL {
	using namespace Maths;

	namespace CSC8508 {
		class Transform
		{
		public:
			Transform();
			~Transform();

			Transform& SetPosition(const Vector3& worldPos);
			Transform& SetScale(const Vector3& worldScale);
			Transform& SetOrientation(const Quaternion& newOr);
			void IncreasePosition(float speed);
			void IncreaseRotation(const Vector3& axis, float degrees);

			Vector3 GetPosition() const {
				return position;
			}

			Vector3 GetScale() const {
				return scale;
			}

			Quaternion GetOrientation() const {
				return orientation;
			}

			Matrix4 GetMatrix() const {
				return matrix;
			}
			void UpdateMatrix();
		protected:
			Matrix4		matrix;
			Quaternion	orientation;
			Vector3		position;

			Vector3		scale;
		};
	}
}

