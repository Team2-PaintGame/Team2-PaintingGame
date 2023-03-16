#include "AnimatedObject.h"
#include "GameObject.h"

using namespace NCL;
using namespace CSC8508;

//Mesh Material Constructor
AnimatedObject::AnimatedObject(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, 
	MeshGeometry* mesh, MeshMaterial* meshMaterial, ShaderBase* shader, const std::unordered_map<std::string, MeshAnimation*>& animations, 
	int size, std::string objectName)
	: GameObject(physicsCommon, physicsWorld, objectName) 
{

}

//TextureBase Constructor
AnimatedObject::AnimatedObject(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position,
	MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, const std::unordered_map<std::string, MeshAnimation*>& animations, 
	int size, std::string objectName)
	: GameObject(physicsCommon, physicsWorld, objectName)
{

}

AnimatedObject::~AnimatedObject()
{
	delete animationController;
}



void AnimatedObject::Update(float dt)
{
	animationController->Update(dt);
}

void NCL::AnimatedObject::SetAnimControler(const std::unordered_map<std::string, MeshAnimation*>& animations)
{
	animationController = new AnimationController(this, animations);
	renderObject->SetAnimationController(animationController);
	renderObject->SetRigged(true);
}
