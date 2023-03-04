/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#pragma once
#include "Matrix4.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Maths.h"
#include "PlayerBase.h"

namespace NCL {
	using namespace NCL::Maths;
	enum class CameraType {
		Orthographic,
		Perspective
	};
	enum class ViewType {
		FirstPerson,
		ThirdPerson
	};
	class Camera {
	public:
		Camera(void) = default;
		void SetBasicCameraParameters(float pitch, float yaw, const Vector3& position, float znear = 1.0f, float zfar = 100.0f);
		void SetFirstPersonCamera();
		void SetThirdPersonCamera(PlayerBase* player, float angleAroundPlayer = 180.0f, Vector3 distanceFromPlayer = Vector3(0, 3.0f, 13));
		void SetPerspectiveCameraParameters(float aspect, float fov = 45.0f);
		void SetOrthographicCameraParameters(float right, float left, float top, float bottom);
		~Camera(void) = default;

		void UpdateCamera(float dt);

		void CalculateFirstPersonView();
		void CalculateThirdPersonView(bool init = false);
		
		//Builds a view matrix for the current camera variables, suitable for sending straight to a vertex shader (i.e it's already an 'inverse camera matrix').
		Matrix4 BuildViewMatrix() const;
		Matrix4 GenerateInverseView() const;

		virtual Matrix4 BuildProjectionMatrix() const;
		virtual Matrix4 GenerateInverseProjection() const;

		float GetPitch() { return pitch;	}
		float GetYaw()	 { return yaw;		}

		//Gets position in world space
		Vector3 GetPosition() const { return position; }
	protected:
		float znear = 1.0f;
		float zfar = 100.0f;

		float aspect = 0.0f;
		float fov = 45.0f;

		float right = 0.0f;
		float left = 0.0f;
		float top = 0.0f;
		float bottom = 0.0f;

		float	yaw = 0.0f;
		float	pitch = 0.0f;
		Vector3 position;

		ViewType viewType = ViewType::ThirdPerson;
		CameraType camType = CameraType::Perspective;

		//third person camera params
		Vector3 offsetFromPlayer = Vector3(0, 3.0f, 13);
		float angleAroundPlayer = 180.0f;
		PlayerBase* player = NULL;
	};
}