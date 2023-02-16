#pragma once
#include "ParticleSystem.h"

namespace NCL {
	
	class InkParticle : public Particle {
	public:
		InkParticle(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Transform emitterTransform, Vector3 particlePosition, float lifeSpan, float speed, Vector3 direction, bool enableGravity);
		virtual ~InkParticle();
	};
	/*class InkStream : public ParticleSystem<InkParticle> {
	public:
		Ink(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 emitterPosition, MeshGeometry* emitterMesh, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader) : ParticleSystem(physicsCommon, physicsWorld, emitterPosition, emitterMesh, mesh, texture, shader) {
			
		}

		virtual ~Ink() {}
	protected:
	};*/

}