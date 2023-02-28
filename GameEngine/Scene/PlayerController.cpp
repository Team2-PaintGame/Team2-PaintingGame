#include "PlayerController.h"
#include "Window.h"
#include "Vector4.h"
#include "InputController.h"
#include<iostream>

using namespace NCL;

PlayerController::PlayerController(Camera* cam, GameObject* player, Gamepad* gamepad) : camera(cam), playerObject(player), gamePad(gamepad)
{
	
}

void PlayerController::Update(float dt)
{
	if (gamePad == NULL) {
		UpdateKeys();
	}
	else {
		ConnectGamePad();
		UpdateGamePad();
	}
	
	

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
		playerObject->GetRigidBody()->applyWorldForceAtCenterOfMass( fwdAxis * force);
		std::cout << "X" << fwdAxis.x << "Y" << fwdAxis.y << fwdAxis.z << "\n";
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::S)) {
		playerObject->GetRigidBody()->applyWorldForceAtCenterOfMass(-fwdAxis * force);
		std::cout << "X" << fwdAxis.x << "Y" << fwdAxis.y << fwdAxis.z << "\n";
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::A)) {
		playerObject->GetRigidBody()->applyWorldForceAtCenterOfMass(-rightAxis3d * force * (1 - side_damping));
		std::cout << rightAxis3d.x << rightAxis3d.y << rightAxis3d.z << "\n";
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::D)) {
		playerObject->GetRigidBody()->applyWorldForceAtCenterOfMass(rightAxis3d * force * (1 - side_damping));
		std::cout << rightAxis3d.x << rightAxis3d.y << rightAxis3d.z << "\n";
	}


	
}

void NCL::PlayerController::ConnectGamePad()
{

	if (!gamePad->Refresh())
	{
		if (wasConnected)
		{
			wasConnected = false;

			std::cout << "Please connect an Xbox controller." << std::endl;
		}
	}
	else if (!wasConnected)
	{
		wasConnected = true;
		std::cout << "Controller connected on port " << gamePad->GetPort() << std::endl;
	}
}

void NCL::PlayerController::UpdateGamePad()
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

	if (gamePad->leftStickY > 0.0f) {
		playerObject->GetRigidBody()->applyWorldForceAtCenterOfMass(fwdAxis * force);
	}

	if (gamePad->leftStickY < 0.0f) {
		playerObject->GetRigidBody()->applyWorldForceAtCenterOfMass(-fwdAxis * force);
	}

	if (gamePad->leftStickX < 0.0f) {
		playerObject->GetRigidBody()->applyWorldForceAtCenterOfMass(-rightAxis3d * force * (1 - side_damping));
	}

	if (gamePad->leftStickX > 0.0f) {
		playerObject->GetRigidBody()->applyWorldForceAtCenterOfMass(rightAxis3d * force * (1 - side_damping));
	}

	
		//std::cout << "Left thumb stick: (" << gamePad->leftStickX << ", " << gamePad->leftStickY << ")   Right thumb stick : (" << gamePad->rightStickX << ", " << gamePad->rightStickY << ")" << std::endl;

		//std::cout << "Left analog trigger: " << gamePad->leftTrigger << "   Right analog trigger: " << gamePad->rightTrigger << std::endl;

		if (gamePad->IsPressed(XINPUT_GAMEPAD_A)) {
			std::cout << "(A) button pressed" << std::endl;
		}


}

