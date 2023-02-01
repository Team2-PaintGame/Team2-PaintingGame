#include "PlayerController.h"
#include "Window.h"
#include "Vector4.h"

using namespace NCL;

PlayerController::PlayerController(Camera* cam, GameObject* player) : camera(cam), player_object(player)
{

}

void PlayerController::Update(float dt)
{
	UpdateKeys();
}

void PlayerController::UpdateKeys()
{
	Matrix4 view = camera->BuildViewMatrix();
	Matrix4 camWorld = view.Inverse();

	Vector3 rightAxis = Vector3(camWorld.GetColumn(0)); //view is inverse of model!

	reactphysics3d::Vector3 rightAxis3d = reactphysics3d::Vector3(rightAxis.x, rightAxis.y, rightAxis.z);

	//forward is more tricky -  camera forward is 'into' the screen...
	//so we can take a guess, and use the cross of straight up, and
	//the right axis, to hopefully get a vector that's good enough!
	reactphysics3d::Vector3 upAxis = reactphysics3d::Vector3(0, 1, 0);
	reactphysics3d::Vector3 fwdAxis = upAxis.cross(rightAxis3d);
	fwdAxis.y = 0.0f;
	fwdAxis.normalize();
	rightAxis3d.normalize();

	float force = 5000.f;
	float side_damping = 0.33f;

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::W)) {
		player_object->GetRigidBody()->applyWorldForceAtCenterOfMass( fwdAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::S)) {
		player_object->GetRigidBody()->applyWorldForceAtCenterOfMass(-fwdAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::A)) {
		player_object->GetRigidBody()->applyWorldForceAtCenterOfMass(-rightAxis3d * force * (1 - side_damping));
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::D)) {
		player_object->GetRigidBody()->applyWorldForceAtCenterOfMass(rightAxis3d * force * (1 - side_damping));
	}
}

