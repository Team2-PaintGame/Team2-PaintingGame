#include "ParticleSystem.h"
#include "Utils.h"

using namespace NCL;

ParticleSystem::ParticleSystem(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, int startSize, float startLifetime, std::string name) : GameObject(physicsCommon, physicsWorld, name) {
	this->startSize = startSize;
	for (int i = 0; i < numParticles; i++) {
		particles.push_back(new Particle(physicsCommon, physicsWorld, position, startSize, startLifetime));
	}

	for (auto& particle : particles) {
		transforms.push_back(&particle->GetTransform());
	}

	renderObject = new RenderObject(transforms, mesh, shader);
	renderObject->AddTexture(texture);
	renderObject->SetInstanceCount(numParticles);
	renderObject->SetIsInstanced(true);
}

void ParticleSystem::Update(float dt) {
	for (auto& p : particles) {
		p->Update(dt);

		if (!p->IsActive()) {
			particles.erase(std::remove(particles.begin(), particles.end(), p), particles.end());
			transforms.erase(std::remove(transforms.begin(), transforms.end(), &p->GetTransform()), transforms.end());
		}
	}

	renderObject->SetTransforms(transforms);
	renderObject->SetInstanceCount(particles.size());
}

ParticleSystem::~ParticleSystem() {
	for (auto const& particle : particles) {
		delete particle;
	}
}

Particle::Particle(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 emitterPosition, int size, float lifeSpan) : GameObject(physicsCommon, physicsWorld) {
	Vector3 position = Vector3(rand() % 15 + 10, 10, rand() % 15);
	transform
		.SetPosition(position + emitterPosition)
		.SetScale(Vector3(size));
	
	this->lifeSpan = lifeSpan;

	boundingVolume = physicsCommon.createSphereShape(size);
	reactphysics3d::Transform rp3d_transform(~transform.GetPosition(), rp3d::Quaternion::identity());

	rigidBody = physicsWorld->createRigidBody(rp3d_transform);
	rigidBody->addCollider(boundingVolume, rp3d::Transform::identity()); 
	rigidBody->updateMassPropertiesFromColliders();
	rigidBody->enableGravity(false);
}

void Particle::Update(float dt) {
	//need to call this here because particles itself are not part of game world
	GameObject::UpdateTransform();

	rigidBody->applyLocalForceAtCenterOfMass(~(transform.GetOrientation() * velocity));
	elapsedTime += dt;

	if (elapsedTime >= lifeSpan) {
		this->SetActive(false);
	}
}

Particle::~Particle() {
	physicsCommon.destroySphereShape(boundingVolume);
}

