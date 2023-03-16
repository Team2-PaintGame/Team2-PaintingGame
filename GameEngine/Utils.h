#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include "Vector3.h"
#include "MeshGeometry.h"
#include "Window.h"
#include "Camera.h"
#include "Vector4.h"

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

	static Vector3 Unproject(const Vector3& screenPos, Camera* cam) {
		Vector2 screenSize = Window::GetWindow()->GetScreenSize();

		cam->SetPerspectiveCameraParameters(Window::GetWindow()->GetScreenAspect() * cam->GetAspectMultiplier());
		//Create our inverted matrix! Note how that to get a correct inverse matrix,
		//the order of matrices used to form it are inverted, too.
		Matrix4 invVP = cam->GenerateInverseView() * cam->GenerateInverseProjection();

		Matrix4 proj = cam->BuildProjectionMatrix(/*aspect*/);

		//Our mouse position x and y values are in 0 to screen dimensions range,
		//so we need to turn them into the -1 to 1 axis range of clip space.
		//We can do that by dividing the mouse values by the width and height of the
		//screen (giving us a range of 0.0 to 1.0), multiplying by 2 (0.0 to 2.0)
		//and then subtracting 1 (-1.0 to 1.0).
		Vector4 clipSpace = Vector4(
			(screenPos.x / (float)screenSize.x) * 2.0f - 1.0f,
			(screenPos.y / (float)screenSize.y) * 2.0f - 1.0f,
			(screenPos.z),
			1.0f
		);

		//Then, we multiply our clipspace coordinate by our inverted matrix
		Vector4 transformed = invVP * clipSpace;

		//our transformed w coordinate is now the 'inverse' perspective divide, so
		//we can reconstruct the final world space by dividing x,y,and z by w.
		return Vector3(transformed.x / transformed.w, transformed.y / transformed.w, transformed.z / transformed.w);
	}
}
