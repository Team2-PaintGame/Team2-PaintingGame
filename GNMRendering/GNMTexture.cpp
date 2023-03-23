#ifdef __ORBIS__
#include "GNMTexture.h"
#include <fstream>
#include <gnf.h>
#include <..\samples\sample_code\graphics\api_gnm\toolkit\allocators.h>
#include <..\samples\sample_code\graphics\api_gnm\toolkit\stack_allocator.h>
#include <TextureLoader.h>
#include "Assets.h"

using namespace sce;
using namespace NCL;
using namespace Rendering;

GNMTexture::GNMTexture()
{
}


GNMTexture::~GNMTexture()
{
}

GNMTexture* GNMTexture::LoadTextureFromFile(const std::string& filename) {
	std::string binaryFilename = filename.substr(0, filename.find_last_of('.')) + ".gnf";
	std::ifstream file(binaryFilename, std::ios::binary);

	if (!file) {
		return NULL;
	}

	Gnf::Header header;
	file.read((char*)&header, sizeof(header));

	if (header.m_magicNumber != sce::Gnf::kMagic) { 
		return NULL; //This wasn't actually a gnf file!!!
	}

	char* rawContents = new char[header.m_contentsSize];
	file.read((char*)rawContents, header.m_contentsSize);


	Gnf::Contents* contentsDesc = (Gnf::Contents*)rawContents;

	Gnm::SizeAlign dataParams	= getTexturePixelsSize(contentsDesc, 0);

	void *pixelsAddr = garlicAllocator->allocate(dataParams);
	Gnm::registerResource(nullptr, ownerHandle, pixelsAddr, dataParams.m_size, binaryFilename.c_str(), Gnm::kResourceTypeTextureBaseAddress, 0);

	file.seekg(getTexturePixelsByteOffset(contentsDesc, 0), ios::cur); //fast forward in the file a bit
	file.read((char*)pixelsAddr, dataParams.m_size);

	GNMTexture* tex = new GNMTexture();
	tex->apiTexture = *patchTextures(contentsDesc, 0, 1, &pixelsAddr);

	//tex->width  = tex->apiTexture.getWidth();
	//tex->height = tex->apiTexture.getHeight();
	//tex->bpp	= tex->apiTexture.getDepth();

	tex->apiTexture.setResourceMemoryType(Gnm::kResourceMemoryTypeRO);

	file.close();
	delete[] rawContents;
	return tex;
}

TextureBase* GNMTexture::RGBATextureFromFilename(const std::string& name) {
	std::string realPath = Assets::TEXTUREDIR + name;
	return LoadTextureFromFile(realPath);

	/*std::ifstream file(name, std::ios::binary);

	if (!file) {
		return NULL;
	}

	char* texData = nullptr;
	int width = 0;
	int height = 0;
	int channels = 0;
	int flags = 0;
	TextureLoader::LoadTexture(name, texData, width, height, channels, flags);

	TextureBase* gnmTex = LoadTextureFromFile(name);

	free(texData);

	return gnmTex;*/
}
#endif