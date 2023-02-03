#include "Bullet.h"
#include "RenderObject.h"
#include "Utils.h"

using namespace NCL;

Bullet::Bullet(Vector3 position, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, int size) : GameObject(physicsCommon, physicsWorld, "Bullet") {
	transform.SetPosition(position);
	transform.SetScale(Vector3(size));

	renderObject = new RenderObject(&transform, mesh, shader);
	renderObject->AddTexture(texture);

	boundingVolume = physicsCommon.createBoxShape(~transform.GetScale() / 2.0f);
	reactphysics3d::Transform rp3d_transform(~position, rp3d::Quaternion::identity());

	rigidBody = physicsWorld->createRigidBody(rp3d_transform);
	rigidBody->addCollider(boundingVolume, rp3d::Transform::identity());
	rigidBody->updateMassPropertiesFromColliders();
	rigidBody->setLinearDamping(1.5f);
}


//void Bullet::Update(float dt) {

//}

Bullet::~Bullet() {
	physicsCommon.destroyBoxShape(boundingVolume);
}

