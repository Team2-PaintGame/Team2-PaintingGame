#include "SceneNode.h"
#include "GameWorld.h"

using namespace NCL;
using namespace CSC8508;

SceneNode::SceneNode(MeshGeometry* mesh, ShaderBase* shader, TextureBase* texture) {
	SetRenderObject(mesh, shader, texture);
}

SceneNode::~SceneNode() {
	delete renderObject;
}

void SceneNode::SetRenderObject(RenderObject* newObject) {
	renderObject = newObject;
}

void SceneNode::SetRenderObject(MeshGeometry* mesh, ShaderBase* shader, TextureBase* texture) {
	renderObject = new RenderObject(NULL, mesh, shader);
	renderObject->SetDefaultTexture(texture);
}
