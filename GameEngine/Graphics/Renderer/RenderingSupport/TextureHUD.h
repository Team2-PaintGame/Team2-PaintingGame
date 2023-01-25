#pragma once
#include "OGLMesh.h"
#include "TextureBase.h"
#include "Vector2.h"

namespace NCL {
	using namespace NCL::Rendering;
	struct TextureHUD {
		TextureBase* texture;
		Vector2 position;
		Vector2 scale;

		TextureHUD(TextureBase* texture, Vector2 position, Vector2 scale) {
			this->texture = texture;
			this->position = position;
			this->scale = scale;
		}
	};
};