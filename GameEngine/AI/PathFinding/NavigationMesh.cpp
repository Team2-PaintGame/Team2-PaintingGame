#include "NavigationMesh.h"
#include "Assets.h"
#include "Maths.h"
#include <fstream>
using namespace NCL;
using namespace CSC8508;
using namespace std;

NavigationMesh::NavigationMesh()
{
}

NavigationMesh::NavigationMesh(const std::string&filename)
{
	ifstream file(Assets::DATADIR + filename);

	int numVertices = 0;
	int numIndices	= 0;

	file >> numVertices; // read in number of vertices
	file >> numIndices; // read in number of indices

	for (int i = 0; i < numVertices; ++i) {
		Vector3 vert;
		file >> vert.x;
		file >> vert.y;
		file >> vert.z;

		allVerts.emplace_back(vert); // reads in x,y,z vertex positions and stores in allVerts
	}

	allTris.resize(numIndices / 3);  // resizes allTris vector to the number of triangles

	for (int i = 0; i < allTris.size(); ++i) {  // Loops over every triangle 
		NavTri* tri = &allTris[i];				// Passes by reference triangle at allTris[i]
		file >> tri->indices[0];				// reads the index position from file and stores in NavTri.indices
		file >> tri->indices[1];
		file >> tri->indices[2];
												// fills out NavTri.centroid position, totals all vertex positions of triangle
		tri->centroid = allVerts[tri->indices[0]] + allVerts[tri->indices[1]] + allVerts[tri->indices[2]];

		tri->centroid = allTris[i].centroid / 3.0f;	// divides NavTri.centroid by three

													
		tri->triPlane =								//creates a plane from NavTri vertex positions
			Plane::PlaneFromTri(allVerts[tri->indices[0]], allVerts[tri->indices[1]], allVerts[tri->indices[2]]);
													
		tri->area =									// calculates the area of NavTri
			Maths::CrossAreaOfTri(allVerts[tri->indices[0]], allVerts[tri->indices[1]], allVerts[tri->indices[2]]);
	}

	for (int i = 0; i < allTris.size(); ++i) {			// Loops over allTris vector
		NavTri* tri = &allTris[i];
		for (int j = 0; j < 3; ++j) {					// Loops over all neighbours of allTri
			int index = 0;
			file >> index;								// reads in neighbour index from file
			if (index != -1) {							// if the neighbour can be traversed too, ie does not equal -1
				tri->neighbours[j] = &allTris[index];	// At index j (0-2), passes by references to alltri the index of the neighbour
														// triangle

														//Then fill in the costs for traversal of each neighbour
				tri->costs[j] = (tri->centroid - tri->neighbours[j]->centroid).Length();
			}
		}
	}
}

NavigationMesh::~NavigationMesh()
{
}

bool NavigationMesh::FindPath(const Vector3& from, const Vector3& to, NavigationPath& outPath) {
	/*const*/ NavTri* start	= GetTriForPosition(from);
	std::cout << "Size of outpath: " << outPath.waypoints.size() << "\n";

	std::cout << "Size of outpath: " << outPath.waypoints.size() << "\n";
	if (start == nullptr) {
		std::cout << "start is outside of the navmesh\n";
		return false;
	}
	/*const*/ NavTri* end	= GetTriForPosition(to);
	if (end == nullptr) {
		std::cout << "end is outside of the navmesh\n";
		return false;
	}

	std::vector<NavTri*> openList;
	std::vector<NavTri*> closedList;

	openList.emplace_back(start); // can't use push_back????

	start->f = 0;
	start->g = 0;
	start->parent = nullptr;

	NavTri* currentBestTri = nullptr;

	while (!openList.empty()) {
		currentBestTri = RemoveBestTri(openList);

		if (currentBestTri == end) {
			NavTri* tri = end;
			triRoute.clear();
			allEdges.clear();
			//outPath.clear();
			while (tri != nullptr) {
				//outPath.PushWaypoint(tri->centroid);
				triRoute.emplace_back(*tri);
				tri = tri->parent;
			}
			std::cout << "triRoute Size: " << triRoute.size();
			FindEdges();
			std::cout << "\nallEdges size: " << allEdges.size();
			FindMidPath(outPath);
			return true;
		}
		else {
			for (int i = 0; i < 3; ++i) {
				NavTri* neighbour = currentBestTri->neighbours[i];
				if (!neighbour) {
					continue;
				}
				bool inClosed = TriInList(neighbour, closedList);
				if (inClosed) {
					continue;
				}

				float h = Heuristic(neighbour, end);
				float g = currentBestTri->g + currentBestTri->costs[i];
				float f = h + g;

				bool inOpen = TriInList(neighbour, openList);

				if (!inOpen) {
					openList.emplace_back(neighbour);
				}
				if (!inOpen || f < neighbour->f) {
					neighbour->parent = currentBestTri;
					neighbour->f = f;
					neighbour->g = g;
				}
			}
			closedList.emplace_back(currentBestTri);

		}

	}
	return false;
}

void NavigationMesh::StringPull(Vector3 securitytPos) {
	Vector3 vertexA, vertexB;
	float angle, nextAngle;
	VertexIndices indices = FindSharedVertices(triRoute.rbegin()[1]);
	vertexA = allVerts[indices.a];
	vertexB = allVerts[indices.b];
	vertexA -= securitytPos;
	vertexB -= securitytPos;
	angle = AngleBetweenVectors(vertexA, vertexB);


}

int NavigationMesh::FindNextVertex(VertexIndices vertIndices, NavigationMesh::NavTri tri) {
	int index;
	for (int i = 0; i < 3; ++i) {
		if (tri.indices[i] != vertIndices.a && tri.indices[i] != vertIndices.b) {
			index = tri.indices[i];
			break;
		}
	}
	return index;
}

void NavigationMesh::FindEdges() {
	int numTris = triRoute.size();
	vector<NavTri>::reverse_iterator it = triRoute.rbegin() + 1; //2nd last element of triRoute, ie. second waypoint
	VertexIndices vertexIndices;

	for (; it != triRoute.rend(); it++) {
		vertexIndices = FindSharedVertices(*it);
		allEdges.emplace_back(vertexIndices);
	}
}

void NavigationMesh::FindMidPath(NavigationPath& outPath) {

	//for (auto i : allEdges) {
	//	Vector3 vertexA = allVerts[i.a];
	//	Vector3 vertexB = allVerts[i.b];
	//	Vector3 vectorAB = vertexA - vertexB;
	//	Vector3 midPoint = vertexB + (vectorAB * 0.5);
	//	outPath.PushWaypoint(midPoint);
	//}

	vector< VertexIndices>::reverse_iterator it = allEdges.rbegin();
	for (; it != allEdges.rend(); ++it) {
		Vector3 vertexA = allVerts[it->a];                  
		Vector3 vertexB = allVerts[it->b];
		Vector3 vectorAB = vertexA - vertexB;
		Vector3 midPoint = vertexB + (vectorAB * 0.5);
		outPath.PushWaypoint(midPoint);
	}
}

VertexIndices NavigationMesh::FindSharedVertices(NavTri tri) {
	int indexA, indexB;
	VertexIndices vertexIndices;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			indexA = tri.indices[i];
			indexB = tri.parent->indices[j];
			if (indexA == indexB) {
				vertexIndices.AddIndex(indexA);
			}
		}
	}
	std::cout << "\nIndexA: " << vertexIndices.a << "\n";
	std::cout << "IndexB: " << vertexIndices.b << "\n";
	return vertexIndices;
}

float NavigationMesh::AngleBetweenVectors(Vector3 a, Vector3 b) {
	float dot = Vector3::Dot(a, b);
	float angle;
	angle = acos(dot / (a.Length() * b.Length()));
	return angle;
}

NavigationMesh::NavTri* NavigationMesh::RemoveBestTri(std::vector<NavTri*>& list) {
	std::vector<NavTri*>::iterator bestI = list.begin();

	NavTri* bestTri = *list.begin();

	for (auto i = list.begin(); i != list.end(); ++i) {
		if ((*i)->f < bestTri->f) {
			bestTri = (*i);
			bestI = i;
		}
	}
	list.erase(bestI);

	return bestTri;
}

/*
If you have triangles on top of triangles in a full 3D environment, you'll need to change this slightly,
as it is currently ignoring height. You might find tri/plane raycasting is handy.
*/
bool NavigationMesh::TriInList(NavigationMesh::NavTri* n, std::vector<NavTri*>& list) /*const*/ {
	std::vector<NavTri*>::iterator i = std::find(list.begin(), list.end(), n);
	return i == list.end() ? false : true;
}

float NavigationMesh::Heuristic(NavTri* hNode, NavTri* endNode) /*const*/ {
	return (hNode->centroid - endNode->centroid).Length();
}

/*const*/ NavigationMesh::NavTri* NavigationMesh::GetTriForPosition(const Vector3& pos) /*const*/ {
	for (/*const*/ NavTri& t : allTris) {
		Vector3 planePoint = t.triPlane.ProjectPointOntoPlane(pos);

		float ta = Maths::CrossAreaOfTri(allVerts[t.indices[0]], allVerts[t.indices[1]], planePoint);
		float tb = Maths::CrossAreaOfTri(allVerts[t.indices[1]], allVerts[t.indices[2]], planePoint);
		float tc = Maths::CrossAreaOfTri(allVerts[t.indices[2]], allVerts[t.indices[0]], planePoint);

		float areaSum = ta + tb + tc;

		if (abs(areaSum - t.area)  > 0.001f) { //floating points are annoying! Are we more or less inside the triangle?
			continue;
		}
		return &t;
	}
	return nullptr;
}