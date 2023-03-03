#pragma once
#include "Vector4.h"
#include "Matrix4.h"
#include "TextureBase.h"
#include "ShaderBase.h"
#include "MeshAnimation.h"
#include "MeshGeometry.h"
#include "MeshMaterial.h"
#include <map>


namespace NCL {
	using namespace NCL::Rendering;

	namespace CSC8508 {
		class Transform;

		using namespace Maths;

		class RenderObject
		{
		public:
			RenderObject(Transform* parentTransform, MeshGeometry* mesh, ShaderBase* shader);
			~RenderObject();
			void LoadMaterialTextures();
			void SetDefaultTexture(TextureBase* t);
			TextureBase* GetDefaultTexture() const;
			void AddTexture(TextureBase* t, std::string uniform = "mainTex", int subMeshIndex = 0);

			std::vector<std::pair<std::string, TextureBase*>> GetTextures(int subMeshIndex) const {
				return subMeshTextures.at(subMeshIndex);
			}

			MeshGeometry*	GetMesh() const {
				return mesh;
			}

			Transform*		GetTransform() const {
				return transform;
			}

			ShaderBase*		GetShader() const {
				return shader;
			}

			void SetColour(const Vector4& c) {
				colour = c;
			}

			void SetMeshMaterial(MeshMaterial* m) {
				multipleTextures = true;
				material = m;
			}

			Vector4 GetColour() const {
				return colour;
			}

			void SetRigged(bool rigged) {
				this->rigged = rigged;
			}

			bool IsRigged() const {
				return rigged;
			}

			void GetFrameMatrices(vector<Matrix4>& frameMatrices) const;

			MeshAnimation* GetMeshAnimation() const {
				return animation;
			}

			void SetMeshAnimation(MeshAnimation* a) {
				animation = a;
			}

			int currentFrame = 0;
			float frameTime = 0.0f;
		protected:
			MeshGeometry*	mesh;
			MeshMaterial* material;
			MeshAnimation* animation;
			//for mutiple textures
			std::map<int, std::vector<std::pair<std::string, TextureBase*>>> subMeshTextures;
			//for single texture
			TextureBase*	texture;
			ShaderBase*		shader;
			Transform*		transform;
			Vector4			colour;
			bool rigged = false;
			bool multipleTextures = false;
		};
	}
}
