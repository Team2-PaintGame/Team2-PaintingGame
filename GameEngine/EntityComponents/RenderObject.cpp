#include "RenderObject.h"
#include "MeshGeometry.h"

#include "OGLTexture.h"

using namespace NCL::CSC8508;
using namespace NCL;

RenderObject::RenderObject(Transform* parentTransform, MeshGeometry* mesh, ShaderBase* shader) {
	this->transform	= parentTransform;
	this->mesh		= mesh;
	this->shader	= shader;
	this->colour	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}

RenderObject::RenderObject(const std::vector<Transform*>& parentTransforms, MeshGeometry* mesh, ShaderBase* shader) {
	this->transforms = parentTransforms;
	this->mesh = mesh;
	this->shader = shader;
	this->colour = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}

RenderObject::~RenderObject() {

}

void RenderObject::SetDefaultTexture(TextureBase* t) {
	multipleTextures = false;
	texture = t;
}

TextureBase* RenderObject::GetDefaultTexture() const {
	return texture;
}

void RenderObject::LoadMaterialTextures(MeshMaterial* material) {
	multipleTextures = true;
	int meshLayers = mesh->GetSubMeshCount();
	for (int i = 0; i < meshLayers; i++) {
		AddTexture(material->GetMaterialForLayer(i)->GetEntry("Diffuse"), "mainTex", i);
	}
}

void RenderObject::AddTexture(TextureBase* t, std::string uniform, int subMeshIndex) {
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

void RenderObject::GetFrameMatrices(vector<Matrix4>& frameMatrices) const {
	const std::vector<Matrix4> invBindPose = mesh->GetInverseBindPose();
	if (animationController->GetCurrentAnimation()) {
		const Matrix4* frameData = animationController->GetCurrentAnimation()->GetJointData(animationController->GetCurrentFrame());
		for (unsigned int i = 0; i < mesh->GetJointCount(); ++i) {
			frameMatrices.emplace_back(frameData[i] * invBindPose[i]);
		}
	}
}