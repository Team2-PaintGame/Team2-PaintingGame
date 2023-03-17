#pragma once
#include <map>
#include <string>
#include "MeshAnimation.h"
#include "MeshMaterial.h"
#include "MeshGeometry.h"
#include "ShaderBase.h"
#include <AssetLoader.h>

namespace NCL {
	using namespace Rendering;
	class GameAssets {
	public:
		GameAssets(Assets::AssetLoader* loader);
		MeshGeometry* GetMesh(const std::string& identifier) const;
		MeshMaterial* GetMeshMaterial(const std::string& identifier) const;
		MeshAnimation* GetMeshAnimation(const std::string& identifier) const;
		TextureBase* GetTexture(const std::string& identifier) const;
		ShaderBase* GetShader(const std::string& identifier) const;

		void Load();
		//SoundBase* GetSound(const std::string& identifier);
	protected:
		Assets::AssetLoader* loader;
		void LoadMeshes();
		void LoadMeshMaterials();
		void LoadMeshAnimations();
		void LoadTextures();
		void LoadShaders();
		//void LoadSounds();

		std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> meshes;
		std::unordered_map<std::string, std::unique_ptr<MeshMaterial>> meshMaterials;
		std::unordered_map<std::string, std::unique_ptr<MeshAnimation>> meshAnimations;

		std::unordered_map<std::string, std::unique_ptr<TextureBase>> textures;
		std::unordered_map<std::string, std::unique_ptr<ShaderBase>> shaders;
		//std::unordered_map<std::string, std::unique_ptr<SoundBase>> sounds;
	};
}