#pragma once
#include "NavigationMap.h"
#include <string>
#include <MeshGeometry.h>
#include <ShaderBase.h>
#include <TextureBase.h>
#include <GameWorld.h>
#include <map>

namespace NCL {
	using namespace Rendering;
	namespace CSC8508 {
		struct GridNode {
			GridNode* parent;

			GridNode* connected[4];
			int		  costs[4];

			Vector3		position;

			float f;
			float g;

			int type;

			GridNode() {
				for (int i = 0; i < 4; ++i) {
					connected[i] = nullptr;
					costs[i] = 0;
				}
				f = 0;
				g = 0;
				type = 0;
				parent = nullptr;
			}
			~GridNode() {	}
		};

		class NavigationGrid : public NavigationMap	{
		public:
			NavigationGrid();
			NavigationGrid(const std::string& filename);
			NavigationGrid(const std::string&filename, std::map<std::string, MeshGeometry*>& meshes, std::map<std::string, TextureBase*>& textures, ShaderBase* shader, GameWorld* world);
			~NavigationGrid();

			bool FindPath(const Vector3& from, const Vector3& to, NavigationPath& outPath) override;
			Vector3 GetStartPosition() { return startPosition; }
			Vector3 GetEndPosition() { return endPosition; }
			vector<Vector3> GetEnemyPositions() { return enemyPositions; }
			Vector3 GetGridPosition(const Vector3& worldPos);

		protected:
			bool		NodeInList(GridNode* n, std::vector<GridNode*>& list) const;
			GridNode*	RemoveBestNode(std::vector<GridNode*>& list) const;
			float		Heuristic(GridNode* hNode, GridNode* endNode) const;

			int nodeSize;
			int gridWidth;
			int gridHeight;
			Vector3 startPosition;
			Vector3 endPosition;
			vector<Vector3> enemyPositions;
			GridNode* allNodes;
		};
	}
}

