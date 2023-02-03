#include "GameObject.h"
#include "RenderObject.h"
#include "MeshGeometry.h"
#include "TextureBase.h"
#include "ShaderBase.h"

#pragma once
namespace NCL {
	using namespace Rendering;
	using namespace CSC8508;
	class Bullet : public GameObject {
	public:
		Bullet(Vector3 position, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, int size);
		~Bullet();

		virtual void Update(float dt) {
		}
	protected:
		RenderObject* renderObject;
		rp3d::BoxShape* boundingVolume;


	};

}

