#pragma once
#include "NavigationMap.h"
#include "Plane.h"
#include <string>
#include <vector>
namespace NCL {
	namespace CSC8508 {

		struct PortalEdge {
			Vector3 leftPortal;
			Vector3 rightPortal;
			Vector3 left;
			Vector3 right;
		};

		class NavigationMesh : public NavigationMap	{
		public:
			NavigationMesh();
			NavigationMesh(const std::string&filename);
			~NavigationMesh();

			bool FindPath(const Vector3& from, const Vector3& to, NavigationPath& outPath) override;
			void StringPull(Vector3 startPosition, Vector3 endPosition, NavigationPath& outPath);
			void FindRouteVertices();
			void FindPortalEdges(const Vector3& to);
			

		protected:
			struct NavTri {
				Plane   triPlane;
				Vector3 centroid;
				float	area;
				NavTri* neighbours[3];
				float costs[3];

				int indices[3];
				float f;
				float g;
				NavTri* parent;

				NavTri() {
					area = 0.0f;
					neighbours[0] = nullptr;
					neighbours[1] = nullptr;
					neighbours[2] = nullptr;

					indices[0] = -1;
					indices[1] = -1;
					indices[2] = -1;

					costs[0] = -1;
					costs[1] = -1;
					costs[2] = -1;

					f = 0;
					g = 0;
					parent = nullptr;

				}
			};

		public:
			
			std::vector<NavTri>		allTris;
			std::vector<Vector3>	allVerts;
			std::vector<NavTri> triRoute;
			std::vector<Vector3> routeVertices;
			std::vector<PortalEdge> portalEdges;
			void SetIsOutNavMeshFalse() { isOutNavMesh = false; }
			bool GetIsOutNavMesh() { return isOutNavMesh; }
			void FindLeftAndRightVertex(Vector3 portalApex, int triIndex, Vector3& portalLeft, Vector3& portalRight);
			NavTri* RemoveBestTri(std::vector<NavTri*>& list);
			/*const*/ NavTri* GetTriForPosition(const Vector3& pos) /*const*/;
			bool  TriInList(NavTri* n, std::vector<NavTri*>& list) /*const*/;
			float Heuristic(NavTri* hNode, NavTri* endNode) /*const*/;
			Vector3 FindClosestPoint(Vector3 to);
			
		protected:
			bool isOutNavMesh = false;

		};
	}
}