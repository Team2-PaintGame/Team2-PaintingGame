#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include "Vector3.h"
#include "MeshGeometry.h"

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

	static reactphysics3d::ConcaveMeshShape* CreateConcaveCollisionBody(reactphysics3d::PhysicsCommon& physicsCommon, MeshGeometry* mesh) {
		float nbvertices = mesh->GetVertexCount();
		int indices = mesh->GetIndexCount();
		int trianglesCount = indices / 3;

		const void* meshVertStart = mesh->GetPositionData().data();
		const void* meshIndexStart = mesh->GetIndexData().data();

		reactphysics3d::TriangleVertexArray* triangleArray =
			new reactphysics3d::TriangleVertexArray(nbvertices, meshVertStart, sizeof(Vector3), trianglesCount,
				meshIndexStart, 3 * sizeof(int),
				reactphysics3d::TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
				reactphysics3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE);

		reactphysics3d::TriangleMesh* triangleMesh = physicsCommon.createTriangleMesh();

		// Add the triangle vertex array to the triangle mesh 
		triangleMesh->addSubpart(triangleArray);

		// Create the concave mesh shape 
		reactphysics3d::ConcaveMeshShape* concaveMeshBoundingVolume = physicsCommon.createConcaveMeshShape(triangleMesh);
		return concaveMeshBoundingVolume;
	}
}
