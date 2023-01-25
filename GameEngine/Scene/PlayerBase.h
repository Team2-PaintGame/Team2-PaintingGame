#pragma once
#include "GameObject.h"
#include "MeshGeometry.h"
#include "TextureBase.h"
#include "ShaderBase.h"

namespace NCL {
	using namespace Rendering;
	using namespace CSC8508;
	class PlayerBase : public GameObject {
		PlayerBase() = default;
		PlayerBase(Vector3 position, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, int size);
		virtual void Update(float dt) {}
		virtual ~PlayerBase() {}
	};
}

