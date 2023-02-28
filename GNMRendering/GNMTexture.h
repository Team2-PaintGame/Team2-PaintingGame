#pragma once
#ifdef _ORBIS
#include "TextureBase.h"
#include <string>
#include <gnm\texture.h>

namespace NCL {
	namespace Rendering {
		class GNMTexture : public TextureBase
		{
		public:
			GNMTexture();
			~GNMTexture();

			static GNMTexture* LoadTextureFromFile(const std::string& filename);
			const sce::Gnm::Texture& GetAPITexture() const { return apiTexture; }
			
		protected:						
			sce::Gnm::Texture apiTexture;
		};
	}
}
#endif

