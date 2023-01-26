#include "PlayerBase.h"
#include "RenderObject.h"
#include "PhysicsObject.h"

using namespace NCL;

PlayerBase::PlayerBase(Vector3 position, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, int size) {
	name = "BasePlayer";
	transform
		.SetScale(Vector3(size, size, size))
		.SetPosition(position);

	renderObject = new RenderObject(&transform, mesh, shader);
	renderObject->AddTexture(0, texture, "mainTex");
	physicsObject = new PhysicsObject(&transform, boundingVolume);
	//in case of material
	/*int meshLayers = mesh->GetSubMeshCount();
	for (int i = 0; i < meshLayers; i++) {
		renderObject->AddTexture(i, material->GetMaterialForLayer(i)->GetEntry("Diffuse"), "mainTex");
	}*/

}



