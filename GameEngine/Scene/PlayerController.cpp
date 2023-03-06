#include "PlayerController.h"
#include "Utils.h"

using namespace NCL;

PlayerController::PlayerController(PlayerBase* player) : player(player) {}

void PlayerController::Update(float dt) {
	Matrix4 view = player->GetCamera()->BuildViewMatrix();
	Matrix4 camWorld = view.Inverse();

	Vector3 rightAxis = Vector3(camWorld.GetColumn(0)); //view is inverse of model!

	Vector3 rightAxis3d = Vector3(rightAxis.x, rightAxis.y, rightAxis.z);
	Vector3 upAxis = Vector3(0, 1, 0);
	Vector3 fwdAxis = Vector3::Cross(upAxis, rightAxis3d);

	fwdAxis.y = 0.0f;
	fwdAxis.Normalise();
	rightAxis3d.Normalise();

	float force = 5000.f;
	float side_damping = 0.33f;

	if (MoveForward()) {
		player->GetRigidBody()->applyWorldForceAtCenterOfMass(~fwdAxis * force);
	}

	if (MoveBackward()) {
		player->GetRigidBody()->applyWorldForceAtCenterOfMass(~-fwdAxis * force);
	}

	if (MoveLeft()) {
		player->GetRigidBody()->applyWorldForceAtCenterOfMass(~-rightAxis3d * force * (1 - side_damping));
	}

	if (MoveRight()) {
		player->GetRigidBody()->applyWorldForceAtCenterOfMass(~rightAxis3d * force * (1 - side_damping));
	}
}
