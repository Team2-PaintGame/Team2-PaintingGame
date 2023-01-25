#include "RenderObject.h"
#include "MeshGeometry.h"

using namespace NCL::CSC8508;
using namespace NCL;

RenderObject::RenderObject(Transform* parentTransform, MeshGeometry* mesh, ShaderBase* shader) {
	this->transform	= parentTransform;
	this->mesh		= mesh;
	this->shader	= shader;
	this->colour	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	/*if (tex) {
		textures.emplace_back(tex);
	}*/
}

RenderObject::~RenderObject() {

}