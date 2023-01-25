#include "Transform.h"

using namespace NCL::CSC8508;

Transform::Transform()	{
	scale = Vector3(1, 1, 1);
}

Transform::~Transform()	{

}

void Transform::UpdateMatrix() {
	matrix =
		Matrix4::Translation(position) *
		Matrix4(orientation) *
		Matrix4::Scale(scale);
}

Transform& Transform::SetPosition(const Vector3& worldPos) {
	position = worldPos;
	UpdateMatrix();
	return *this;
}

Transform& Transform::SetScale(const Vector3& worldScale) {
	scale = worldScale;
	UpdateMatrix();
	return *this;
}

Transform& Transform::SetOrientation(const Quaternion& worldOrientation) {
	orientation = worldOrientation;
	UpdateMatrix();
	return *this;
}

void NCL::CSC8508::Transform::IncreasePosition(float speed) {
	//position += worldPos;
	position += Matrix3(orientation) * Vector3(0.0f, 0.0f, speed);
	UpdateMatrix();
}

void NCL::CSC8508::Transform::IncreaseRotation(const Vector3& axis, float degrees) {
	orientation = Quaternion::AxisAngleToQuaterion(axis, degrees) * orientation;
	UpdateMatrix();
}
