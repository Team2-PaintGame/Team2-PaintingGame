#pragma once
#include "TextureBase.h"

namespace NCL {
	using namespace Rendering;
	struct TerrainTexturePack {
		TextureBase* bgTex;
		TextureBase* rTex;
		TextureBase* gTex;
		TextureBase* bTex;
		TextureBase* splatMap;

		TerrainTexturePack(TextureBase* splatMap, TextureBase* rTex, TextureBase* gTex, TextureBase* bTex, TextureBase* bgTex) {
			this->splatMap = splatMap;
			this->rTex = rTex;
			this->gTex = gTex;
			this->bTex = bTex;
			this->bgTex = bgTex;
		}
	};
};

