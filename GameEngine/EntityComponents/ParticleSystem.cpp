#include "ParticleSystem.h"

using namespace NCL;

ParticleSystem::ParticleSystem(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, int size, std::string name) : GameObject(physicsCommon, physicsWorld, name) {
	for (int i = 0; i < numParticles; i++) {
		particles.push_back(Particle(size));
	}

	std::vector<Transform*> transforms;
	for (auto& particle : particles) {
		transforms.push_back(&particle.GetTransform());
	}

	renderObject = new RenderObject(transforms, mesh, shader);
	renderObject->AddTexture(texture);
	mesh->SetInstanceCount(numParticles);
}
