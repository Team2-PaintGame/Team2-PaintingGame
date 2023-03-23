#pragma once
#include <map>
#include <string>
#include "MeshAnimation.h"
#include "MeshMaterial.h"
#include "MeshGeometry.h"
#include "ShaderBase.h"
#include <AssetLoader.h>
#include <unordered_map>

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

		void ReloadMeshes();
		void ReloadShaders();
		void ReloadDebug();

		void Load();
	protected:
		Assets::AssetLoader* loader;
		void LoadMeshes();
		void LoadMeshMaterials();
		void LoadMeshAnimations();
		void LoadTextures();
		void LoadShaders();

		std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> meshes;
		std::unordered_map<std::string, std::unique_ptr<MeshMaterial>> meshMaterials;
		std::unordered_map<std::string, std::unique_ptr<MeshAnimation>> meshAnimations;

		std::unordered_map<std::string, std::unique_ptr<TextureBase>> textures;
		std::unordered_map<std::string, std::unique_ptr<ShaderBase>> shaders;

	};
}