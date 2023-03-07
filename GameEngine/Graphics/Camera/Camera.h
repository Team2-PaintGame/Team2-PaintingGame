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
#include "Vector2.h"

namespace NCL {
	using namespace NCL::Maths;
	class PlayerBase;
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
		void SetBasicCameraParameters(PlayerBase* player, float znear = 1.0f, float zfar = 100.0f);
		void SetFirstPersonCamera();
		void SetThirdPersonCamera(Vector3 offsetFromPlayer = Vector3(0, 3.0f, 13));
		void SetPerspectiveCameraParameters(float aspect, float fov = 45.0f);
		void SetOrthographicCameraParameters(float right, float left, float top, float bottom);
		void SetViewportDivider(float divider);
		float GetViewportDivider() const { return viewportDivider; }
		void SetViewportSize(Vector2 v) { viewportSize = v; }
		Vector2 GetViewportSize() const { return viewportSize; }
		~Camera(void) = default;

		void Update(float dt);
		void CalculateThirdPersonView();
		
		//Builds a view matrix for the current camera variables, suitable for sending straight to a vertex shader (i.e it's already an 'inverse camera matrix').
		Matrix4 BuildViewMatrix() const;
		Matrix4 GenerateInverseView() const;

		virtual Matrix4 BuildProjectionMatrix() const;
		virtual Matrix4 GenerateInverseProjection() const;

		float GetPitch() const { return pitch;	}
		float GetYaw()	const { return yaw;		}

		float GetNearPlane() const {
			return znear;
		}

		float GetFarPlane() const {
			return zfar;
		}

		float GetFieldOfVision() const {
			return fov;
		}
		//Gets position in world space
		Vector3 GetPosition() const { return position; }
	protected:
		float znear = 1.0f;
		float zfar = 100.0f;

		float aspect = 0.0f;
		float fov = 45.0f;
		float viewportDivider = 1.0f;
		Vector2 viewportSize = Vector2(0.0f, 0.0f);

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
		PlayerBase* player = NULL;
	};
}