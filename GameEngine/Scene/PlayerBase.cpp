#include "PlayerBase.h"
#include "RenderObject.h"
#include "PhysicsObject.h"
#include "AABBVolume.h"
#include "Window.h"

using namespace NCL;

PlayerBase::PlayerBase(Vector3 position, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, int size) {
	name = "BasePlayer";
	transform
		.SetScale(Vector3(size))
		.SetPosition(position);

	renderObject = new RenderObject(&transform, mesh, shader);
	renderObject->AddTexture(0, texture, "mainTex");

	float inverseMass = 0.5f;

	AABBVolume* volume = new AABBVolume(Vector3(size/2));

	boundingVolume = (CollisionVolume*)volume;
	physicsObject = new PhysicsObject(&transform, boundingVolume);

	physicsObject->SetInverseMass(inverseMass);
	physicsObject->InitSphereInertia();
	//in case of material
	/*int meshLayers = mesh->GetSubMeshCount();
	for (int i = 0; i < meshLayers; i++) {
		renderObject->AddTexture(i, material->GetMaterialForLayer(i)->GetEntry("Diffuse"), "mainTex");
	}*/

}

void PlayerBase::Update(float dt) {
	/*
	//Window::GetMouse()->GetWindowPosition()
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::W)) {
		currentRunSpeed = -runSpeed;
	}
	else if (Window::GetKeyboard()->KeyDown(KeyboardKeys::S)) {
		currentRunSpeed = runSpeed;
	}
	else {
		currentRunSpeed = 0;
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::A)) {
		currentTurnSpeed = turnSpeed;
	}
	else if (Window::GetKeyboard()->KeyDown(KeyboardKeys::D)) {
		currentTurnSpeed = -turnSpeed;
	}
	else {
		currentTurnSpeed = 0;
	}

	transform.IncreaseRotation(Vector3(0, 1, 0), currentTurnSpeed * dt);
	transform.IncreasePosition(currentRunSpeed * dt);*/
}



