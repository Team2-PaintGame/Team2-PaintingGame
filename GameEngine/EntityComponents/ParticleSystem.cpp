#include "ParticleSystem.h"
#include "Utils.h"

using namespace NCL;

ParticleSystem::ParticleSystem(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 emitterPosition, MeshGeometry* emitterMesh, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, bool enableGravity, float startSize, float startLifetime, float startSpeed, std::string name) : GameObject(physicsCommon, physicsWorld, name), emitter(emitterMesh) {
	this->startLifetime = startLifetime;
	this->startSpeed = startSpeed;
	this->enableGravity = enableGravity;

	transform.SetPosition(emitterPosition);
	transform.SetScale(startSize);
	//transform.SetOrientation(Quaternion::AxisAngleToQuaterion(Vector3(1, 0, 0), 45) * Quaternion::AxisAngleToQuaterion(Vector3(0, 1, 0), 0));
	
	renderObject = new RenderObject(transforms, mesh, shader);
	renderObject->AddTexture(texture);
	renderObject->SetInstanceCount(particles.size());
	renderObject->SetIsInstanced(true);
}

void ParticleSystem::Update(float dt) {
	accumulator += dt;
	elapsedTime += dt;
	GenerateParticles();

	for (size_t i = 0; i < particles.size();) {
		particles[i]->Update(dt);

		if (!particles[i]->IsActive()) {
			std::swap(particles[i], particles.back());
			particles.pop_back();
			std::swap(transforms[i], transforms.back());
			transforms.pop_back();
		}
		else
			++i;
	}
	
	renderObject->SetTransforms(transforms);
	renderObject->SetInstanceCount(particles.size());
}

ParticleSystem::~ParticleSystem() {
	particles.clear();
}

void ParticleSystem::GenerateParticles() {
	if (elapsedTime < duration || looping) {
		while (accumulator > 1.0 / emitter.GetParticleEmissionRate() && particles.size() < maxParticles) {
			particles.emplace_back(std::make_unique<Particle>(physicsCommon, physicsWorld, transform, Vector3(), startLifetime, startSpeed, emitter.GetEmissionDirection(), enableGravity));
			transforms.emplace_back(&particles.back()->GetTransform());
			accumulator -= 1.0 / emitter.GetParticleEmissionRate();
		}
	}
}

Particle::Particle(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Transform emitterTransform, Vector3 particlePosition, float lifeSpan, float speed, Vector3 direction, bool enableGravity) : GameObject(physicsCommon, physicsWorld) {
	transform
		.SetPosition(emitterTransform.GetPosition() + particlePosition)
		.SetScale(emitterTransform.GetScale())
		.SetOrientation(emitterTransform.GetOrientation());
	
	this->lifeSpan = lifeSpan;
	this->speed = speed;
	this->direction = direction;

	boundingVolume = physicsCommon.createSphereShape(emitterTransform.GetScale().x);
	reactphysics3d::Transform rp3d_transform(~transform.GetPosition(), ~transform.GetOrientation());

	rigidBody = physicsWorld->createRigidBody(rp3d_transform);
	rigidBody->addCollider(boundingVolume, rp3d::Transform::identity()); 
	rigidBody->enableGravity(enableGravity);
}

void Particle::Update(float dt) {
	elapsedTime += dt;

	//need to call this here because particles itself are not part of game world
	GameObject::UpdateTransform();

	rigidBody->applyWorldForceAtCenterOfMass(~(transform.GetOrientation() * direction * speed));

	if (elapsedTime >= lifeSpan) {
		this->SetActive(false);
	}
}

Particle::~Particle() {
	if (rigidBody) {
		physicsWorld->destroyRigidBody(rigidBody);
	}
	physicsCommon.destroySphereShape(dynamic_cast<rp3d::SphereShape*>(boundingVolume));
}

Emitter::Emitter(MeshGeometry* emitterMesh) {
	this->emitterMesh = emitterMesh;
	emissionDirections = emitterMesh->GetNormalData();
	vIter = emissionDirections.begin();
}

Vector3 Emitter::GetEmissionDirection() {
	Vector3 dir = *vIter;
	vIter = vIter + 1 == emissionDirections.end() ? emissionDirections.begin() : vIter + 1;
	return dir;
}
