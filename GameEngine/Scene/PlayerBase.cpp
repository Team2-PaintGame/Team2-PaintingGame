#include "PlayerBase.h"
#include "RenderObject.h"
#include "Window.h"
#include "Utils.h"


using namespace NCL;

PlayerBase::PlayerBase(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, MeshGeometry* mesh, TextureBase* texture, AnimationController* animController, ShaderBase* shader, int size): GameObject(physicsCommon, physicsWorld, "BasePlayer") {
	transform
		.SetScale(Vector3(size))
		.SetPosition(position);

	renderObject = new RenderObject(&transform, mesh, shader);
	this->animationController = animController;
	//animationController->SetRenderer(renderObject);
	animationController->SetGameObject(this);
	animationController->InitStateMachine();
	//renderObject->AddTexture(texture);

	int subMeshes = mesh->GetSubMeshCount();
	for (int index = 0; index < subMeshes; ++index) {
		renderObject->AddTexture(texture, "mainTex", index);
	}
	//renderObject->SetRigged(true);
	//renderObject->animation = meshAnimation;
	boundingVolume = physicsCommon.createBoxShape(~transform.GetScale() / 2.0f);
	reactphysics3d::Transform rp3d_transform(~position, rp3d::Quaternion::identity());
	
	// Create a rigid body in the physics world
	rigidBody = physicsWorld->createRigidBody(rp3d_transform);
	rigidBody->addCollider(boundingVolume, rp3d::Transform::identity()); //collider
	rigidBody->updateMassPropertiesFromColliders();
	rigidBody->setLinearDamping(1.5f);
}





void PlayerBase::Update(float dt) {

	animationController->UpdateAnimations(dt);
}

PlayerBase::~PlayerBase() {
	if (rigidBody) {
		physicsWorld->destroyRigidBody(rigidBody);
	}

	physicsCommon.destroyBoxShape(boundingVolume);
}



