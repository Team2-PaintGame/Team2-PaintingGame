#include "Player.h"
#include <CollisionDetection.h>
#include <Debug.h>

using namespace NCL;
using namespace CSC8508;

void Player::Update(float dt) {
	PlayerBase::Update(dt);
	AnimatedObject::Update(dt);

	if (gun ) {

		gun->GetTransform().SetPosition(transform.GetPosition() + (transform.GetOrientation() * gunOffset));

		if (camera)
		{
			gun->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(pitch, yaw, 0));
		}
		else
		{
			Vector3 euler = transform.GetOrientation().ToEuler();
			gun->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(pitch, euler.y, euler.z));
		}
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
	//Debug::DrawLine(startPos, endPos, Vector4(1, 1, 1, 1), 3);
}

void Player::SetMemberVariables(Gun* gun) {
	if (camera)
	{
		camera->SetBasicCameraParameters(this, 0.1f, 500.0f);
		camera->SetPerspectiveCameraParameters(Window::GetWindow()->GetScreenAspect());
		camera->SetThirdPersonCamera();
	}
	layer = Layer::Player;
	this->gun = gun;
}
