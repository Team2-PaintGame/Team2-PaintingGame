#include "Camera.h"
#include "Window.h"
#include <algorithm>
#include <math.h>
#include "Utils.h"
#include "PlayerBase.h"

using namespace NCL;

void Camera::SetBasicCameraParameters(PlayerBase* player, float znear, float zfar) {
	this->player = player;
	this->znear = znear;
	this->zfar = zfar;
}

void Camera::SetFirstPersonCamera() {
	viewType = ViewType::FirstPerson;
	this->offsetFromPlayer = Vector3(0, 5.0f, 0);
}

void Camera::SetThirdPersonCamera(Vector3 offsetFromPlayer) {
	viewType = ViewType::ThirdPerson;
	this->offsetFromPlayer = offsetFromPlayer;
}

void Camera::SetPerspectiveCameraParameters(float aspect, float fov) {
	this->znear = znear;
	this->zfar = zfar;
	this->aspect = aspect;
	this->fov = fov;
	camType = CameraType::Perspective;
}

void Camera::SetOrthographicCameraParameters(float right, float left, float top, float bottom) {
	this->right = right;
	this->left = left;
	this->top = top;
	this->bottom = bottom;
	camType = CameraType::Orthographic;
}

void Camera::SetVpSize(float x, float y) {
	vpSize = Vector2(x, y);
}

void Camera::Update(float dt) {
	yaw = player->GetYaw();
	pitch = player->GetPitch();
	Matrix4 rotation = Matrix4::Rotation(yaw, { 0, 1, 0 });
	rotated_offset = rotation * Matrix4::Rotation(pitch, { 1, 0, 0 }) * offsetFromPlayer;
	position = player->GetTransform().GetPosition() + rotated_offset;

	//setting player's orientation w.r.t. camera
	Quaternion player_orientation(rotation);
	reactphysics3d::Transform newRBTransform = reactphysics3d::Transform(player->GetRigidBody()->getTransform().getPosition(), ~player_orientation);
	player->GetRigidBody()->setTransform(newRBTransform);
}

/*
Generates a view matrix for the camera's viewpoint. This matrix can be sent
straight to the shader...it's already an 'inverse camera' matrix.
*/
Matrix4 Camera::BuildViewMatrix() const {
	//Why do a complicated matrix inversion, when we can just generate the matrix
	//using the negative values ;). The matrix multiplication order is important!
	return	Matrix4::Translation(-offsetFromPlayer) *
		Matrix4::Rotation(-pitch, Vector3(1, 0, 0)) *
		Matrix4::Rotation(-yaw, Vector3(0, 1, 0)) *
		Matrix4::Translation(-position);
}

/*
And here's how we generate an inverse view matrix. It's pretty much
an exact inversion of the BuildViewMatrix function of the Camera class!
*/
Matrix4 Camera::GenerateInverseView() const {
	Matrix4 iview =
		Matrix4::Translation(position) *
		Matrix4::Rotation(yaw, Vector3(0, 1, 0)) *
		Matrix4::Rotation(pitch, Vector3(1, 0, 0)) * 
		Matrix4::Translation(offsetFromPlayer);

	return iview;
}
Matrix4 Camera::BuildProjectionMatrix() const {
	if (camType == CameraType::Orthographic) {
		return Matrix4::Orthographic(left, right, bottom, top, znear, zfar);
	}
	else if (camType == CameraType::Perspective) {
		return Matrix4::Perspective(znear, zfar, aspect, fov);
	}
}
Matrix4 Camera::GenerateInverseProjection() const
{
	Matrix4 m;

	float t = tan(fov * PI_OVER_360);

	float neg_depth = znear - zfar;

	const float h = 1.0f / t;

	float c = (zfar + znear) / neg_depth;
	float e = -1.0f;
	float d = 2.0f * (znear * zfar) / neg_depth;

	m.array[0][0] = aspect / h;
	m.array[1][1] = tan(fov * PI_OVER_360);
	m.array[2][2] = 0.0f;

	m.array[2][3] = 1.0f / d;

	m.array[3][2] = 1.0f / e;
	m.array[3][3] = -c / (d * e);

	return m;
}


