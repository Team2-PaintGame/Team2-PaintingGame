#include "ParticleSystem.h"
#include "Utils.h"

using namespace NCL;

ParticleSystem::ParticleSystem(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 emitterPosition, MeshGeometry* emitterMesh, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, float startSize, float startLifetime, float startSpeed, std::string name) : GameObject(physicsCommon, physicsWorld, name), emitter(emitterMesh) {
	this->startSize = startSize;
	this->startLifetime = startLifetime;
	this->startSpeed = startSpeed;
	
	transform.SetPosition(emitterPosition);

	renderObject = new RenderObject(transforms, mesh, shader);
	renderObject->AddTexture(texture);
	renderObject->SetInstanceCount(particles.size());
	renderObject->SetIsInstanced(true);
}

void ParticleSystem::Update(float dt) {

	accumulator += dt;
	GenerateParticles();

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

void ParticleSystem::GenerateParticles() {
	while (accumulator > 1.0 / emitter.GetParticleEmissionRate()) {
		particles.push_back(new Particle(physicsCommon, physicsWorld, transform.GetPosition(), Vector3(), startSize, startLifetime, startSpeed, emitter.GetEmissionDirection()));
		transforms.push_back(&particles.back()->GetTransform());
		accumulator -= 1.0 / emitter.GetParticleEmissionRate();
	}
}

Particle::Particle(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 emitterPosition, Vector3 particlePosition, float size, float lifeSpan, float speed, Vector3 direction) : GameObject(physicsCommon, physicsWorld) {
	transform
		.SetPosition(emitterPosition)
		.SetScale(Vector3(size));
	
	this->lifeSpan = lifeSpan;
	this->speed = speed;
	this->direction = direction;

	boundingVolume = physicsCommon.createSphereShape(size);
	reactphysics3d::Transform rp3d_transform(~transform.GetPosition(), rp3d::Quaternion::identity());

	rigidBody = physicsWorld->createRigidBody(rp3d_transform);
	rigidBody->addCollider(boundingVolume, rp3d::Transform::identity()); 
	rigidBody->enableGravity(false);
}

void Particle::Update(float dt) {
	elapsedTime += dt;

	//need to call this here because particles itself are not part of game world
	GameObject::UpdateTransform();

	rigidBody->applyWorldForceAtCenterOfMass(~(direction * speed));

	if (elapsedTime >= lifeSpan) {
		this->SetActive(false);
	}
}

Particle::~Particle() {
	physicsCommon.destroySphereShape(boundingVolume);
}

