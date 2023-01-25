#pragma once
#include "Vector4.h"
#include "Matrix4.h"
#include "TextureBase.h"
#include "ShaderBase.h"
#include "MeshAnimation.h"
#include "MeshGeometry.h"
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

			/*void SetDefaultTexture(TextureBase* t) {
				texture = t;
			}

			TextureBase* GetDefaultTexture() const {
				return texture;
			}*/

			void AddTexture(int subMeshIndex, TextureBase* t, std::string uniform) {
				if (t) {
					if (subMeshTextures.count(subMeshIndex)) {
						//if this submesh pair already exists, add to the inner map
						subMeshTextures.at(subMeshIndex).push_back(std::make_pair(uniform, t));
					}
					else {
						subMeshTextures.insert(std::make_pair(subMeshIndex, std::vector<std::pair<std::string, TextureBase*>>{std::make_pair(uniform, t)}));
					}
				}
			}

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

			Vector4 GetColour() const {
				return colour;
			}

			void SetRigged(bool rigged) {
				this->rigged = rigged;
			}

			bool IsRigged() const {
				return rigged;
			}

			void GetFrameMatrices(vector<Matrix4>& frameMatrices) const {
				const std::vector<Matrix4> invBindPose = mesh->GetInverseBindPose();
				const Matrix4* frameData = animation->GetJointData(currentFrame);
				for (unsigned int i = 0; i < mesh->GetJointCount(); ++i) {
					frameMatrices.emplace_back(frameData[i] * invBindPose[i]);
				}
				/*OGLShader* shader = (OGLShader*)(this->shader);
				glUniformMatrix4fv(glGetUniformLocation(shader->GetProgramID(), "joints"), frameMatrices.size(), false, (float*)frameMatrices.data());*/
			}

			MeshAnimation* animation;
			int currentFrame = 0;
			float frameTime = 0.0f;
		protected:
			MeshGeometry*	mesh;
			std::map<int, std::vector<std::pair<std::string, TextureBase*>>> subMeshTextures;

			//std::vector<TextureBase*>	textures;
			ShaderBase*		shader;
			Transform*		transform;
			Vector4			colour;
			bool	rigged = false;
		};
	}
}
