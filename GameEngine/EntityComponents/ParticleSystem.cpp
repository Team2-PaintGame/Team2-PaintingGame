#include "ParticleSystem.h"
#include "Utils.h"

using namespace NCL;

ParticleSystem::ParticleSystem(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 emitterposition, MeshGeometry* emitterMesh, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, float startSize, float startLifetime, float startSpeed, std::string name) : GameObject(physicsCommon, physicsWorld, name) {
	this->startSize = startSize;
	this->startLifetime = startSize;
	this->startSpeed = startSize;
	this->emitterMesh = emitterMesh;

	std::vector<Vector3> directions = emitterMesh->GetNormalData();
	std::vector<Vector3> particlePositions = emitterMesh->GetPositionData();
	numParticles = directions.size() > maxParticles ? maxParticles : directions.size();

	for (int i = 0; i < numParticles; i++) {
		particles.push_back(new Particle(physicsCommon, physicsWorld, emitterposition, particlePositions[i], startSize, startLifetime, startSpeed, directions[i]));
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

Particle::Particle(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 emitterPosition, Vector3 particlePosition, float size, float lifeSpan, float speed, Vector3 direction) : GameObject(physicsCommon, physicsWorld) {
	transform
		.SetPosition(particlePosition + emitterPosition)
		.SetScale(Vector3(size));
	
	this->lifeSpan = lifeSpan;
	this->speed = speed;
	this->direction = direction;

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

	rigidBody->applyLocalForceAtCenterOfMass(~(direction * speed));
	elapsedTime += dt;

	if (elapsedTime >= lifeSpan) {
		//this->SetActive(false);
	}
}

Particle::~Particle() {
	physicsCommon.destroySphereShape(boundingVolume);
}

