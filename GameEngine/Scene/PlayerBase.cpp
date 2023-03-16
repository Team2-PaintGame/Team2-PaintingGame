#include "PlayerBase.h"
#include "RenderObject.h"
#include "Window.h"
#include "Debug.h"
#include "GameWorld.h"


using namespace NCL;
using namespace CSC8508;

PlayerBase::PlayerBase(reactphysics3d::PhysicsCommon & physicsCommon, reactphysics3d::PhysicsWorld * physicsWorld, Vector3 position, MeshGeometry * mesh, TextureBase * texture, ShaderBase * shader, int size) : GameObject(physicsCommon, physicsWorld, "BasePlayer") {
	SetMemberVariables(physicsCommon, physicsWorld, position, mesh, shader, size);
	renderObject->SetDefaultTexture(texture);
}
PlayerBase::PlayerBase(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, MeshGeometry* mesh, MeshMaterial* meshMaterial, ShaderBase* shader, int size) : GameObject(physicsCommon, physicsWorld, "BasePlayer") {
	SetMemberVariables(physicsCommon, physicsWorld, position, mesh, shader, size);
	renderObject->LoadMaterialTextures(meshMaterial);
	raycastManager = new RaycastManager();
	raycastManager->setIgnore(rigidBody);
}

PlayerBase::~PlayerBase() {
	if (rigidBody) {
		physicsWorld->destroyRigidBody(rigidBody);
	}
	physicsCommon.destroyCapsuleShape((rp3d::CapsuleShape*)(boundingVolume));

	delete camera;
}

void PlayerBase::Update(float dt) {
	camera->Update(dt);
	CameraSpring(camera);
}

void PlayerBase::SetYawPitch(float dx, float dy) {
	yaw -= dx; 
	pitch -= dy;

	pitch = std::clamp(pitch, -25.f, 25.0f);

	if (yaw < 0) {
		yaw += 360.0f;
	}
	if (yaw > 360.0f) {
		yaw -= 360.0f;
	}
}

void PlayerBase::SetMemberVariables(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, MeshGeometry* mesh, ShaderBase* shader, int size) {
	
	transform.SetScale(Vector3(size))
			 .SetPosition(position);

	renderObject = new RenderObject(&transform, mesh, shader);

	//boundingVolume = physicsCommon.createBoxShape(~transform.GetScale() / 2.0f);
	boundingVolume = physicsCommon.createCapsuleShape(size*.35f,size);
	reactphysics3d::Transform rp3d_transform(~position, rp3d::Quaternion::identity());

	// Create a rigid body in the physics world
	rigidBody = physicsWorld->createRigidBody(rp3d_transform);
	rigidBody->addCollider(boundingVolume, rp3d::Transform::identity()); //collider
	rigidBody->updateMassPropertiesFromColliders();
	rigidBody->setLinearDamping(1.5f);
  
	camera = new Camera();
}

void PlayerBase::CameraSpring(Camera* cam) {

	RaycastManager raycastManager = RaycastManager();

	ray = reactphysics3d::Ray(~transform.GetPosition(), ~transform.GetPosition() + ~camera->GetNormalizedRotation() * camera->GetMaxOffSet().Length());
	//Debug::DrawLine(transform.GetPosition(), ray.point2, Vector4(1, 1, 1, 1),1.0f);

	
	 physicsWorld->raycast(ray, &raycastManager);
	 if (raycastManager.isHit()) { //if it hits something
		 SceneContactPoint* closestCollision = raycastManager.getHit();
		 Vector3 new_rotated_offset = closestCollision->hitPos - ~transform.GetPosition();
		 camera->SetOffsetFromPlayer(camera->GetMaxOffSet().Normalised() * new_rotated_offset.Length());
	 }
	 else {
		 camera->SetOffsetFromPlayer(camera->GetMaxOffSet());
	 }
		
}




