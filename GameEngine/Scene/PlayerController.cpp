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

	//forward is more tricky -  camera forward is 'into' the screen...
	//so we can take a guess, and use the cross of straight up, and
	//the right axis, to hopefully get a vector that's good enough!

	Vector3 fwdAxis = Vector3::Cross(Vector3(0, 1, 0), rightAxis);
	fwdAxis.y = 0.0f;
	fwdAxis.Normalise();


	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::W)) {
		//player_object->GetPhysicsObject()->AddForce(fwdAxis * 50);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::S)) {
		//player_object->GetPhysicsObject()->AddForce(-fwdAxis * 50);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::A)) {
		//player_object->GetPhysicsObject()->AddForce(-rightAxis * 30);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::D)) {
		//player_object->GetPhysicsObject()->AddForce(rightAxis * 30);
	}
}

