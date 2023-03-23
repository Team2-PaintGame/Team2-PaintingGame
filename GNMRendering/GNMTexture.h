#pragma once
#ifdef __ORBIS__
#include "TextureBase.h"
#include <string>
#include <gnm\texture.h>
#include "PS4MemoryAware.h"

namespace NCL {
	namespace Rendering {
		class GNMTexture : public TextureBase, public PS4::PS4MemoryAware
		{
		public:
			GNMTexture();
			~GNMTexture();

			static GNMTexture* LoadTextureFromFile(const std::string& filename);
			const sce::Gnm::Texture& GetAPITexture() const { return apiTexture; }

			static TextureBase* RGBATextureFromFilename(const std::string& name);
		protected:						
			sce::Gnm::Texture apiTexture;
		};
	}
}
#endif

