#include "Camera.h"
#include "Window.h"
#include <algorithm>
#include <math.h>

using namespace NCL;
void Camera::SetBasicCameraParameters(float pitch, float yaw, const Vector3& position, float znear, float zfar) {
	this->pitch = pitch;
	this->yaw = yaw;
	this->position = position;
	this->znear = znear;
	this->zfar = zfar;
}

void Camera::SetFirstPersonCamera() {
	viewType = ViewType::FirstPerson;
	distanceFromPlayer = 0.0f;
	angleAroundPlayer = 0.0f;

	CalculateFirstPersonView();
}

void Camera::SetThirdPersonCamera(PlayerBase* player, float angleAroundPlayer, float distanceFromPlayer) {
	viewType = ViewType::ThirdPerson;
	this->player = player;
	this->distanceFromPlayer = distanceFromPlayer;
	this->angleAroundPlayer = angleAroundPlayer;

	CalculateThirdPersonView(true);
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
/*
Polls the camera for keyboard / mouse movement.
Should be done once per frame! Pass it the msec since
last frame (default value is for simplicities sake...)
*/
void Camera::UpdateCamera(float dt) {
	
	pitch	-= (Window::GetMouse()->GetRelativePosition().y);

	//Bounds check the pitch, to be between straight up and straight down ;)
	pitch = std::min(pitch, 90.0f);
	pitch = std::max(pitch, -90.0f);

	if (viewType == ViewType::FirstPerson) {
		CalculateFirstPersonView();
	}
	else if (viewType == ViewType::ThirdPerson) {
		CalculateThirdPersonView();
	}

	Matrix4 rotation = Matrix4::Rotation(yaw, Vector3(0, 1, 0));
	Vector3 forward = rotation * Vector3(0, 0, -1);
	Vector3 right = rotation * Vector3(1, 0, 0);
	float speed = 10.0f * dt;

	if (Window::GetKeyboard()->KeyDown(NCL::KeyboardKeys::W)) {
		position += forward * speed;
	}
	if (Window::GetKeyboard()->KeyDown(NCL::KeyboardKeys::S)) {
		position -= forward * speed;
	}
	if (Window::GetKeyboard()->KeyDown(NCL::KeyboardKeys::A)) {
		position -= right * speed;
	}
	if (Window::GetKeyboard()->KeyDown(NCL::KeyboardKeys::D)) {
		position += right * speed;
	}
	if (Window::GetKeyboard()->KeyDown(NCL::KeyboardKeys::SHIFT)) {
		position.y += speed;
	}
	if (Window::GetKeyboard()->KeyDown(NCL::KeyboardKeys::SPACE)) {
		position.y -= speed;
	}
}

void Camera::CalculateFirstPersonView() {
	yaw -= (Window::GetMouse()->GetRelativePosition().x);
	if (yaw < 0) {
		yaw += 360.0f;
	}
	if (yaw > 360.0f) {
		yaw -= 360.0f;
	}
}

void Camera::CalculateThirdPersonView(bool init) {
	distanceFromPlayer -= Window::GetMouse()->GetWheelMovement() * 0.1f;
	angleAroundPlayer -= Window::GetMouse()->GetRelativePosition().x;
	
	float vDist = distanceFromPlayer * cos(Maths::DegreesToRadians(pitch));
	float hDist = distanceFromPlayer * sin(Maths::DegreesToRadians(pitch));

	//euler angles (x => pitch, y => yaw, z => roll)
	//yaw angle is the rotation around y axis

	float theta = Maths::DegreesToRadians(player->GetTransform().GetOrientation().ToEuler().y + angleAroundPlayer);
	float xOffset = hDist * sin(theta);
	float zOffset = hDist * cos(theta);

	Vector3 playerPosition = player->GetTransform().GetPosition();

	position.x = playerPosition.x - xOffset;
	position.z = playerPosition.z - zOffset;
	position.y = playerPosition.y + vDist + 14;

	Matrix4 temp = Matrix4::BuildViewMatrix(position, playerPosition, Vector3(0, 1, 0));

	Matrix4 modelMat = temp.Inverse();

	Quaternion q(modelMat);
	Vector3 angles = q.ToEuler();
	if (init) pitch = angles.x;
	
	yaw = angles.y;
}

/*
Generates a view matrix for the camera's viewpoint. This matrix can be sent
straight to the shader...it's already an 'inverse camera' matrix.
*/
Matrix4 Camera::BuildViewMatrix() const {
	//Why do a complicated matrix inversion, when we can just generate the matrix
	//using the negative values ;). The matrix multiplication order is important!
	return	Matrix4::Translation(-Vector3(0, 0, distanceFromPlayer)) *
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
		Matrix4::Translation(Vector3(0, 0, distanceFromPlayer));

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


