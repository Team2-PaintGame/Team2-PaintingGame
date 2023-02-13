#include "RenderObject.h"
#include "MeshGeometry.h"

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