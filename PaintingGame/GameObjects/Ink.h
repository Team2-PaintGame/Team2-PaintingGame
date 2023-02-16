#pragma once
#include "ParticleSystem.h"

namespace NCL {
	
	class InkParticle : public Particle {
	public:
		InkParticle(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Transform emitterTransform, Vector3 particlePosition, float lifeSpan, float speed, Vector3 direction, bool enableGravity);
		virtual ~InkParticle();
	};
	
	static ParticleSystem<InkParticle>* CreateInkSplash(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 emitterPosition, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader) {
		ParticleSystem<InkParticle>* inkStream = new ParticleSystem<InkParticle>(physicsCommon, physicsWorld, emitterPosition, 45.0f, mesh, texture, shader, true, 0.5f, 0.5f, 10.0f);
		inkStream->SetParticleEmissionRate(30);
		inkStream->SetRunTime(3.0f);
		return inkStream;
	}

	static ParticleSystem<InkParticle>* CreateInkStream(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 emitterPosition, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader) {
		ParticleSystem<InkParticle>* inkSplash = new ParticleSystem<InkParticle>(physicsCommon, physicsWorld, emitterPosition, 5.0f, mesh, texture, shader, true, 1, 1.0f, 15.0f);
		inkSplash->SetParticleEmissionRate(30);
		inkSplash->SetRunTime(4.0f);
		return inkSplash;
	}
}