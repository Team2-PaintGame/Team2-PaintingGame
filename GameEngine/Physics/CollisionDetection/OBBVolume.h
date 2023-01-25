#pragma once
#include "Transform.h"
#include "CollisionVolume.h"
#include "Matrix3.h"

using namespace NCL::CSC8503;
using namespace NCL::Maths;

namespace NCL {
	class OBBVolume : CollisionVolume {
	public:
		OBBVolume(const Maths::Vector3& halfDims) {
			type		= VolumeType::OBB;
			halfSizes	= halfDims;
		}
		~OBBVolume() {}

		Maths::Vector3 GetHalfDimensions() const {
			return halfSizes;
		}
		void GetAxes(const Transform& worldTransform, std::vector<Vector3>& axes) const {
			Matrix3 orientation = Matrix3(worldTransform.GetOrientation());
			axes.push_back(orientation * Vector3(1.0f, 0.0f, 0.0f)); //X - Axis
			axes.push_back(orientation * Vector3(0.0f, 1.0f, 0.0f)); //Y - Axis
			axes.push_back(orientation * Vector3(0.0f, 0.0f, 1.0f)); //Z - Axis
		}
		void GetMinMaxOnAxis(const Transform& worldTransform, Vector3& minVertex, Vector3& maxVertex, float& minOnAxis, float& maxOnAxis, const Vector3& axis) const {
			/*
			* A -------------- B
			*	|			 |
			*	|			 |
			*	|			 |
			*	|			 |
			* C -------------- D
			*/
			//transforming OBB into an ABB to get the min and max of the box
			/*Quaternion orientation = worldTransform.GetOrientation();
			Matrix3 invOrientation = Matrix3(orientation.Conjugate());
			Vector3 localPos = invOrientation * worldTransform.GetPosition();
			min = localPos - halfSizes;
			max = localPos + halfSizes;

			min = orientation * min;
			max = orientation * min;*/
			Quaternion orientation = worldTransform.GetOrientation();
			Matrix3 invOrientation = Matrix3(orientation.Conjugate());
			Vector3 localPos = invOrientation * worldTransform.GetPosition();
			Vector3 min = localPos - halfSizes;
			Vector3 max = localPos + halfSizes;
			min = orientation * min;
			max = orientation * min;
			std::vector<Vector3> vertices;
			vertices.push_back({ min.x, max.y, min.z }); //A
			vertices.push_back({ max.x, max.y, min.z }); //B
			vertices.push_back({ min.x, min.y, min.z }); //C
			vertices.push_back({ max.x, min.y, min.z }); //D
			vertices.push_back({ min.x, max.y, max.z }); //E
			vertices.push_back({ max.x, max.y, max.z }); //F
			vertices.push_back({ min.x, min.y, max.z }); //G
			vertices.push_back({ max.x, min.y, max.z }); //H

			minOnAxis = HUGE, maxOnAxis = -HUGE;
			for (int i = 0; i < vertices.size(); i++)
			{
				// project each vertex on the given axis to get the min and max value on that axis
				float dotVal = Vector3::Dot(vertices[i], axis);
				if (dotVal < minOnAxis) { 
					minOnAxis = dotVal; 
					minVertex = vertices[i];
				}
				if (dotVal > maxOnAxis) {
					maxOnAxis = dotVal;
					maxVertex = vertices[i];
				}
			}
		}
		Vector3 OBBSupport(const Transform& worldTransform, Vector3 worldDir) const {
			Vector3 localDir = worldTransform.GetOrientation().Conjugate() * worldDir;
			Vector3 vertex;
			vertex.x = localDir.x < 0 ? -0.5f : 0.5f;
			vertex.y = localDir.y < 0 ? -0.5f : 0.5f;
			vertex.z = localDir.z < 0 ? -0.5f : 0.5f;
			return worldTransform.GetMatrix() * vertex;
		}
	protected:
		Maths::Vector3 halfSizes;
		const int numVertices = 8;
	};
}

