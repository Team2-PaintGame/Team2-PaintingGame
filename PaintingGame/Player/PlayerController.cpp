#include "PlayerController.h"
#include "Utils.h"
#include <algorithm>
#include "GameScreen.h"
using namespace NCL;

PlayerController::PlayerController(Player* player) : player(player) {}

void PlayerController::Update(float dt) {
	if (Pause()) {
		GameScreen::GamePauseCallback();
		return;
	}

	Matrix4 view = player->GetCamera()->BuildViewMatrix();
	Matrix4 camWorld = view.Inverse();

	Vector3 rightAxis = Vector3(camWorld.GetColumn(0)); //view is inverse of model!

	Vector3 upAxis = Vector3(0, 1, 0);
	Vector3 fwdAxis = Vector3::Cross(upAxis, rightAxis);

	fwdAxis.y = 0.0f;
	fwdAxis.Normalise();
	rightAxis.Normalise();

	if (player->GetHasRespawned() == true)
	{
		player->SetRespawnTimer(dt);
		if (player->GetRespawnTimer() >= 3.0)
		{
			player->ResetSpawnTimer();
			player->SetHasRespawnedFalse();
		}
		return;
	}
	
	float force = 5000.f;
	float side_damping = 0.33f;

	if (MoveForward()) {
		player->GetRigidBody()->applyWorldForceAtCenterOfMass(~fwdAxis * force);
	}

	if (MoveBackward()) {
		player->GetRigidBody()->applyWorldForceAtCenterOfMass(~-fwdAxis * force);
	}

	if (MoveLeft()) {
		player->GetRigidBody()->applyWorldForceAtCenterOfMass(~- rightAxis * force * (1 - side_damping));
	}

	if (MoveRight()) {
		player->GetRigidBody()->applyWorldForceAtCenterOfMass(~rightAxis * force * (1 - side_damping));
	}

	//player->GetGun()->SetActive(player->GetIsMoving());

	player->SetYawPitch(ViewDx(), ViewDy());
	
	if (Shoot()) {
		player->Shoot();
	}
}
