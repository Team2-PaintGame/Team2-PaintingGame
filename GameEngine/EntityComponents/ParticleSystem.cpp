#include "ParticleSystem.h"

using namespace NCL;

ParticleSystem::ParticleSystem(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, int size, std::string name) : GameObject(physicsCommon, physicsWorld, name) {
	for (int i = 0; i < numParticles; i++) {
		particles.push_back(new Particle(physicsCommon, physicsWorld, size));
	}

	std::vector<Transform*> transforms;
	for (auto& particle : particles) {
		transforms.push_back(&particle->GetTransform());
	}

	renderObject = new RenderObject(transforms, mesh, shader);
	renderObject->AddTexture(texture);
	renderObject->SetInstanceCount(numParticles);
}

void ParticleSystem::Update(float dt) {
	for (auto& p : particles) {
		p->Update(dt);
	}
}

ParticleSystem::~ParticleSystem() {
	for (auto const& particle : particles) {
		delete particle;
	}
}

Particle::Particle(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, int size) : GameObject(physicsCommon, physicsWorld) {
	Vector3 position = Vector3(rand() % 50, 10, rand() % 50);

	transform
		.SetPosition(position)
		.SetScale(Vector3(size));

	boundingVolume = physicsCommon.createBoxShape(~transform.GetScale() / 2.0f);
	reactphysics3d::Transform rp3d_transform(~position, rp3d::Quaternion::identity());

	rigidBody = physicsWorld->createRigidBody(rp3d_transform);
	rigidBody->addCollider(boundingVolume, rp3d::Transform::identity()); 
	rigidBody->updateMassPropertiesFromColliders();
}

void Particle::Update(float dt) {
	//need to call this here because particles itself are not part of game world
	GameObject::UpdateTransform();

	elapsedTime += dt;
}

Particle::~Particle() {
	physicsCommon.destroyBoxShape(boundingVolume);
}

