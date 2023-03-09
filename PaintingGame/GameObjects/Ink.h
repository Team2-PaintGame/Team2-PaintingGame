#pragma once
#include "ParticleSystem.h"
#include "Particle.h"

namespace NCL {
	template <class T>
	static ParticleSystem<T>* CreateInkSplash(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 emitterPosition, MeshGeometry* mesh, Vector4 color, ShaderBase* shader) {
		ParticleSystem<T>* inkSplash = new ParticleSystem<T>(physicsCommon, physicsWorld, emitterPosition, 45.0f, mesh, color, shader, true, 0.1f, 0.5f, 10.0f);
		inkSplash->SetParticleEmissionRate(30);
		inkSplash->SetRunTime(3.0f);
		return inkSplash;
	}

	template <class T>
	static ParticleSystem<T>* CreateInkStream(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 emitterPosition, MeshGeometry* mesh, Vector4 color, ShaderBase* shader) {
		ParticleSystem<T>* inkStream = new ParticleSystem<T>(physicsCommon, physicsWorld, emitterPosition, 5.0f, mesh, color, shader, true, 0.5, 1.0f, 15.0f);
		inkStream->SetParticleEmissionRate(30);
		inkStream->SetRunTime(4.0f);
		return inkStream;
	}
}