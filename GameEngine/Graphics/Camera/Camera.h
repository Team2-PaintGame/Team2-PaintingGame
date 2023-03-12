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
		void SetThirdPersonCamera(Vector3 offsetFromPlayer = Vector3(0.0f, 3.0f, 13));
		void SetPerspectiveCameraParameters(float aspect, float fov = 45.0f);
		void SetOrthographicCameraParameters(float right, float left, float top, float bottom);
		void SetVpSize(float x = 1.0f, float y = 1.0f);
		Vector2 GetVpSize() const { return vpSize; }
		float GetAspectMultiplier() const { return vpSize.x * vpSize.y; }
		void SetVpStartPos(Vector2 v) { vpStartPos = v; }
		Vector2 GetVpStartPos() const { return vpStartPos; }
		~Camera(void) = default;

		void Update(float dt);
		
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
		void SetPosition(Vector3 pos) { position = pos; }
		void SetOffsetFromPlayer(Vector3 offset) { offsetFromPlayer = offset; }
		Vector3 GetOffsetFromPlayer() const { return offsetFromPlayer; }
		Vector3 GetNormalizedRotation() const { return rotated_offset.Normalised(); }
		Vector3 GetMaxOffSet() const { return maxOffSet; }
	protected:
		float znear = 1.0f;
		float zfar = 100.0f;

		float aspect = 0.0f;
		float fov = 45.0f;
		
		//viewport modifiers
		Vector2 vpSize = Vector2(1.0f, 1.0f);
		Vector2 vpStartPos = Vector2(0.0f, 0.0f);

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
		Vector3 offsetFromPlayer = Vector3(0.0f, 3.0f, 13);
		Vector3 rotated_offset;
		Vector3 maxOffSet = Vector3(0.0f, 3.0f, 13);
		PlayerBase* player = NULL;
	};
}