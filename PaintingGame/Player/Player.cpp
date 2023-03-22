#include "Player.h"
#include <CollisionDetection.h>
#include <Debug.h>

using namespace NCL;
using namespace CSC8508;

void Player::Update(float dt) {
	PlayerBase::Update(dt);
	AnimatedObject::Update(dt);

	if (gun) {
		gun->GetTransform()
			.SetPosition(transform.GetPosition() + (transform.GetOrientation() * gunOffset))
			//.SetOrientation(transform.GetOrientation());
			.SetOrientation(Quaternion::EulerAnglesToQuaternion(pitch, yaw, 0));
	}
}

void Player::Shoot() {
	/*Ray r = CollisionDetection::BuildRayFromMouse(*camera);
	Vector3 startPos = r.GetPosition();
	Vector3 endPos = r.GetPosition() + r.GetDirection() * 1000;

	shootRay = reactphysics3d::Ray(
		reactphysics3d::Vector3(startPos.x, startPos.y + 5, startPos.z),
		reactphysics3d::Vector3(endPos.x, endPos.y, endPos.z));
	Debug::DrawLine(startPos, endPos, Vector4(1, 1, 1, 1), 3);*/
	gun->Shoot();
}

void Player::SetMemberVariables(Gun* gun) {
	camera->SetBasicCameraParameters(this, 0.1f, 500.0f);
	camera->SetPerspectiveCameraParameters(Window::GetWindow()->GetScreenAspect());
	camera->SetThirdPersonCamera();
	layer = Layer::Player;
	this->gun = gun;
}
