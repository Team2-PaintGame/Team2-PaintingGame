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
}
