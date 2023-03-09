#pragma once
#include "NavigationMap.h"
#include "Plane.h"
#include <string>
#include <vector>
namespace NCL {
	namespace CSC8508 {

		struct VertexIndices {
			int a;
			int b;
			VertexIndices() {
				a = -1;
				b = -1;
			}
			void AddIndex(int index) {
				if (a == -1) {
					a = index;
				}
				else {
					b = index;
				}
			}
		};

		struct PortalEdge {
			Vector3 leftPortal;
			Vector3 rightPortal;

		};

		class NavigationMesh : public NavigationMap	{
		public:
			NavigationMesh();
			NavigationMesh(const std::string&filename);
			~NavigationMesh();

			bool FindPath(const Vector3& from, const Vector3& to, NavigationPath& outPath) override;
			int StringPull(Vector3 startPosition, Vector3 endPosition, NavigationPath& outPath);
			float AngleBetweenVectors(Vector3 a, Vector3 b);
			void FindEdges();
			void FindMidPath(NavigationPath& outPath);
			void FindRouteVertices();
			void FindPortalEdges();

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
			std::vector<VertexIndices> allEdges;
			std::vector<Vector3> routeVertices;
			std::vector<PortalEdge> portalEdges;

			bool IsNextEdgeVertex(Vector3 portalVertex, int triIndex);
			void FindLeftAndRightVertex(Vector3 portalApex, int triIndex, Vector3& portalLeft, Vector3& portalRight);
			VertexIndices FindSharedVertices(NavTri tri);
			int FindNextVertex(VertexIndices vertIndices, NavigationMesh::NavTri tri);
			NavTri* RemoveBestTri(std::vector<NavTri*>& list);
			/*const*/ NavTri* GetTriForPosition(const Vector3& pos) /*const*/;
			bool  TriInList(NavTri* n, std::vector<NavTri*>& list) /*const*/;
			float Heuristic(NavTri* hNode, NavTri* endNode) /*const*/;
		};
	}
}