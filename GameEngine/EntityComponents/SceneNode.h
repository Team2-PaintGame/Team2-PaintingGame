#pragma once
#include "RenderObject.h"

namespace NCL {
	class Camera;
}
namespace NCL::CSC8508 {
	class GameWorld;
	typedef std::function<void(Camera*)> CameraFunc;
	class SceneNode {
	public:
		SceneNode() = default;
		SceneNode(MeshGeometry* mesh, ShaderBase* shader, TextureBase* texture);
		virtual ~SceneNode();
		RenderObject* GetRenderObject() const { return renderObject; }
		void SetRenderObject(RenderObject* newObject);
		void SetRenderObject(MeshGeometry* mesh, ShaderBase* shader, TextureBase* texture);
		virtual void Update(float dt) {}
		virtual GameWorld* GetWorld() const { return nullptr; }
		virtual reactphysics3d::PhysicsWorld* GetPhysicsWorld() const { return nullptr; }
		virtual void OperateOnCameras(CameraFunc f) {}
	protected:
		RenderObject* renderObject = NULL;
	};
}