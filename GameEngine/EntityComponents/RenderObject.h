#pragma once
#include "Vector4.h"
#include "Matrix4.h"
#include "TextureBase.h"
#include "ShaderBase.h"
#include "MeshAnimation.h"
#include "MeshGeometry.h"
#include "MeshMaterial.h"
#include <map>
#include "AnimationController.h"

namespace NCL {
	using namespace NCL::Rendering;

	namespace CSC8508 {
		class Transform;

		using namespace Maths;

		class RenderObject
		{
		public:
			RenderObject(Transform* parentTransform, MeshGeometry* mesh, ShaderBase* shader);

			//constructor for instanced render objects using the same mesh and shader with different transforms
			RenderObject(const std::vector<Transform*>& parentTransforms, MeshGeometry* mesh, ShaderBase* shader);
			~RenderObject();
			void LoadMaterialTextures(MeshMaterial* material);
			void SetDefaultTexture(TextureBase* t);
			TextureBase* GetDefaultTexture() const;
			void AddTexture(TextureBase* t, std::string uniform = "mainTex", int subMeshIndex = 0);

			std::vector<std::pair<std::string, TextureBase*>> GetTextures(int subMeshIndex) const {
				return subMeshTextures.at(subMeshIndex);
			}

			bool HasTextureAtIndex(int subMeshIndex) const {
				return subMeshTextures.count(subMeshIndex) > 0;
			}

			unsigned int GetInstanceCount()  const {
				return (unsigned int)numInstances;
			}

			void SetInstanceCount(unsigned int num) {
				numInstances = num;
			}

			MeshGeometry*	GetMesh() const {
				return mesh;
			}

			Transform*		GetTransform() const {
				return transform;
			}

			std::vector<Transform*> GetTransforms() const {
				return transforms;
			}

			void SetTransforms(const std::vector<Transform*>& parentTransforms) {
				this->transforms = parentTransforms;
			}

			ShaderBase*		GetShader() const {
				return shader;
			}

			void SetColour(const Vector4& c) {
				colour = c;
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

			void SetIsInstanced(bool isInstanced) {
				this->isInstanced = isInstanced;
			}

			bool GetIsInstanced() const {
				return isInstanced;
			}
			void GetFrameMatrices(vector<Matrix4>& frameMatrices) const;

			void SetAnimationController(AnimationController* a) {
				animationController = a;
			}

			bool isSingleTextured() const {
				return !multipleTextures;
			}

			void SetIsOccluded(bool val) {
				isOccluded = val;
			}

			bool GetIsOccluded() const {
				return isOccluded;
			}
		protected:
			MeshGeometry*	mesh;
			AnimationController* animationController;
			//for mutiple textures
			std::map<int, std::vector<std::pair<std::string, TextureBase*>>> subMeshTextures;
			//for single texture
			TextureBase*	texture = NULL;
			ShaderBase*		shader;
			Transform*		transform;
			std::vector<Transform*> transforms;
			Vector4			colour;
			bool	rigged = false;
			unsigned int numInstances = 0;
			bool isInstanced = false;
			bool multipleTextures = false;
			bool isOccluded = true; //When an object is occluded, it is not rendered by the graphics pipeline if it is not visible to the viewer
		};
	}
}
