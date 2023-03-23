#pragma once
#include "ParticleSystem.h"
#include "Particle.h"

namespace NCL {
	
	typedef ParticleSystem<SphereParticle> Ink;

	static Ink* CreateInkSplash(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 emitterPosition, MeshGeometry* mesh, Vector4 color, ShaderBase* shader) {
		Ink* inkSplash = new Ink(physicsCommon, physicsWorld, emitterPosition, 45.0f, mesh, color, shader, true, 0.1f, 0.5f, 50.0f);
		inkSplash->SetParticleEmissionRate(15);
		inkSplash->SetRunTime(1.0f);
		return inkSplash;
	}

	static Ink* CreateInkStream(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 emitterPosition, MeshGeometry* mesh, Vector4 color, ShaderBase* shader) {
		Ink* inkStream = new Ink(physicsCommon, physicsWorld, emitterPosition, 5.0f, mesh, color, shader, true, 0.5, 1.0f, 50.0f);
		inkStream->SetParticleEmissionRate(15);
		inkStream->SetRunTime(1.0f);
		return inkStream;
	}
}