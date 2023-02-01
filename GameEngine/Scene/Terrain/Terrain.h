#pragma once
//#include "TerrainMesh.h"
#include "Vector2.h"
#include "GameObject.h"
#include "MeshGeometry.h"
#include "TextureBase.h"
#include "ShaderBase.h"
#include "TerrainTexturePack.h"

namespace NCL {
	using namespace Maths;
	using namespace CSC8508;
	using namespace Rendering;

	class Terrain: public GameObject {
	public:
		Terrain(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector2 gridPosition, MeshGeometry* mesh, TerrainTexturePack texturePack, ShaderBase* shader, int size = 200);
		~Terrain(void);
	protected:
		rp3d::HeightFieldShape* boundingVolume;
	};
};

