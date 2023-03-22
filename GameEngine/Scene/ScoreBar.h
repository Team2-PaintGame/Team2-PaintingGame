#pragma once
#include "GameObject.h"
#include "OGLMesh.h"

namespace NCL {
	using namespace Rendering;
	using namespace CSC8508;
	class ScoreBar : public GameObject {
	public:
		ScoreBar(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, ShaderBase* shader, std::string name = "") : GameObject(physicsCommon, physicsWorld, "ScoreBar") {
			scoreQuad = new OGLMesh();
			scoreQuad->SetVertexPositions({ Vector3(-1, 1, 1), Vector3(-1, -1, 1), Vector3(1, -1, 1), Vector3(1, 1, 1) });
			scoreQuad->SetVertexTextureCoords({ Vector2(0.0f,1.0f), Vector2(0.0f,0.0f), Vector2(1.0f,0.0f), Vector2(1.0f,1.0f) });
			scoreQuad->SetVertexIndices({ 0,1,2,2,3,0 });
			scoreQuad->UploadToGPU();

			renderObject = new RenderObject(&transform, scoreQuad, shader);
		}
	protected:
		OGLMesh* scoreQuad;
	};
}