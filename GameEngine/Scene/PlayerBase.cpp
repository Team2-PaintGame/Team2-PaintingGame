#include "PlayerBase.h"
#include "RenderObject.h"
#include "Window.h"
#include "Debug.h"
#include "GameWorld.h"


using namespace NCL;
using namespace CSC8508;

//TextureBase Constructor
PlayerBase::PlayerBase(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position,
	MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, const std::unordered_map<std::string, MeshAnimation*>& animations,
	int size, std::string objectName, bool networked)
	: AnimatedObject(physicsCommon, physicsWorld, position, mesh, texture, shader, animations, size, objectName) 
{
	AnimatedObject::SetAnimControler(animations);
	SetMemberVariables(physicsCommon, physicsWorld, position, mesh, shader, size, networked);
	renderObject->SetDefaultTexture(texture);
}

//Mesh Material Constructor
PlayerBase::PlayerBase(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position,
	MeshGeometry* mesh, MeshMaterial* meshMaterial, ShaderBase* shader, const std::unordered_map<std::string, MeshAnimation*>& animations,
	int size, std::string objectName, bool networked)
	: AnimatedObject(physicsCommon, physicsWorld, position, mesh, meshMaterial, shader, animations, size, objectName) 
{
	SetMemberVariables(physicsCommon, physicsWorld, position, mesh, shader, size, networked);
	AnimatedObject::SetAnimControler(animations);
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
	if (camera)
	{
		camera->Update(dt);
		CameraSpring(camera);
	}
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

void PlayerBase::SetMemberVariables(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, MeshGeometry* mesh, ShaderBase* shader, int size, bool networked) {
	
	transform.SetScale(Vector3(size))
			 .SetPosition(position);

	spawnPosition = position;

	renderObject = new RenderObject(&transform, mesh, shader);

	//boundingVolume = physicsCommon.createBoxShape(~transform.GetScale() / 2.0f);
	boundingVolume = physicsCommon.createCapsuleShape(size*.35f,size);
	reactphysics3d::Transform rp3d_transform(~position, rp3d::Quaternion::identity());

	// Create a rigid body in the physics world
	rigidBody = physicsWorld->createRigidBody(rp3d_transform);
	reactphysics3d::Collider* collider = rigidBody->addCollider(boundingVolume, rp3d::Transform::identity()); //collider
	//collider->setIsTrigger(true);
	rigidBody->updateMassPropertiesFromColliders();
	rigidBody->setLinearDamping(1.5f);
	rigidBody->setUserData(this);
  
	layer = Layer::Player;

	if (!networked)
	{
		camera = new Camera();
	}
}

void PlayerBase::CameraSpring(Camera* cam) {

	RaycastManager raycastManager = RaycastManager();

	ray = reactphysics3d::Ray(~transform.GetPosition(), ~transform.GetPosition() + ~camera->GetNormalizedRotation() * (camera->GetMaxOffSet().Length() + 15)) ;
//	ray = reactphysics3d::Ray(~transform.GetPosition(), ~camera->GetPosition());
	
	
	 physicsWorld->raycast(ray, &raycastManager);
	 if (raycastManager.isHit()) { //if it hits something
		 SceneContactPoint* closestCollision = raycastManager.getHit();
		 Vector3 new_rotated_offset = closestCollision->hitPos - ~transform.GetPosition();
		 Vector3 newOffest = camera->GetMaxOffSet().Normalised() * (new_rotated_offset.Length() / 2 );
//		 camera->SetOffsetFromPlayer(camera->GetMaxOffSet().Normalised() * new_rotated_offset.Length());
		 camera->SetOffsetFromPlayer(newOffest);
		// Debug::DrawLine(transform.GetPosition(), ray.point2, Vector4(1, 1, 1, 1), 0.50f);
	 }
	 else {
		 camera->SetOffsetFromPlayer(camera->GetMaxOffSet());
	 }
		
}




