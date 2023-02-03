#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include "Vector3.h"

namespace NCL {
	static reactphysics3d::Vector3 operator~(Maths::Vector3 v) {
		reactphysics3d::Vector3 rp3dVector;
		rp3dVector.x = v.x;
		rp3dVector.y = v.y;
		rp3dVector.z = v.z;
		return rp3dVector;
	}

	static reactphysics3d::Quaternion operator~(Maths::Quaternion q) {
		reactphysics3d::Quaternion rp3dQauternion;
		rp3dQauternion.x = q.x;
		rp3dQauternion.y = q.y;
		rp3dQauternion.z = q.z;
		rp3dQauternion.w = q.w;
		return rp3dQauternion;
	}
}
