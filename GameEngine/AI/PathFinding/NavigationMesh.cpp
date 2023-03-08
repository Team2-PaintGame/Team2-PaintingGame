#include "NavigationMesh.h"
#include "Assets.h"
#include "Maths.h"
#include <fstream>
#include "Debug.h"
using namespace NCL;
using namespace CSC8508;
using namespace std;

NavigationMesh::NavigationMesh()
{
}

NavigationMesh::NavigationMesh(const std::string&filename)
{
	float triArea = Maths::FloatAreaOfTri(Vector3(0.5, 0, 0.5), Vector3(2, 0, 0), Vector3(2, 0, 2));

	std::cout << endl << endl << endl;
	std::cout << "Tri Area pRight: " << triArea << endl;
	triArea = Maths::FloatAreaOfTri(Vector3(0.5, 0, 0.5), Vector3(0, 0, 2), Vector3(2, 0, 2));
	std::cout << "Tri Area pLeft: " << triArea << endl;

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
			
			FindEdges();
			StringPull(from, to, outPath);
		//	std::cout << "Outpath size: " << outPath.waypoints.size();
			//FindMidPath(outPath);
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
//	std::cout << "\nIndexA: " << vertexIndices.a << "\n";
//	std::cout << "IndexB: " << vertexIndices.b << "\n";
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

inline float vdistsqr(const Vector3 a, const Vector3 b)
{
	const float dx = b[0] - a[0];
	const float dy = b[1] - a[1];
	const float dz = b[2] - a[2];
	return sqrt(dx * dx + dy * dy + dz * dz);  // maybe this should be square root?
}

inline bool vequal(const Vector3 a, const Vector3 b)
{
	static const float eq = 0.001f * 0.001f;
	return vdistsqr(a, b) < eq;
}

bool IsLeftOfApex(Vector3 apex, Vector3 midEdge, Vector3 vertex) {
	return ((midEdge.x - apex.x) * (vertex.z - apex.z) * (midEdge.z - apex.z) * (vertex.x - apex.x)) < 0;
}


int NavigationMesh::StringPull(Vector3 startPosition, Vector3 endPosition, NavigationPath& outPath) {
	outPath.Clear();
	// Find straight path.
	int npts = 0;
	int nPortals = allEdges.size();
	int maxPts = nPortals + 1; //what should the max points be??

	Vector3 portalApex, portalLeft, portalRight;
	int apexIndex = 0, leftIndex = 0, rightIndex = 0;

	portalLeft = allVerts[allEdges[0].a];
	portalRight = allVerts[allEdges[0].b];
	
	float yPos = triRoute.back().centroid.y;
	portalApex = startPosition;
	portalApex.y = yPos;

	// Add start point.
	outPath.PushWaypoint(portalApex);

	npts++;

	bool equal;
	float triArea;

	Vector3 midEdgetest;
	Vector3 lengthEdgetest = allVerts[allEdges[1].a] - allVerts[allEdges[1].b];

	midEdgetest = allVerts[allEdges[1].b] + (lengthEdgetest * 0.5f);

	bool isLeft = IsLeftOfApex(portalApex, midEdgetest, allVerts[allEdges[1].a]);

	Vector3 lefttest, righttest;
	if (isLeft)
	{
		lefttest = allVerts[allEdges[1].a];
		righttest = allVerts[allEdges[1].b];
	}
	else
	{
		righttest = allVerts[allEdges[1].a];
		lefttest = allVerts[allEdges[1].b];
	}

	Debug::DrawLine(portalApex, lefttest, Debug::YELLOW, 5);
	Debug::DrawLine(portalApex, righttest, Debug::RED, 5);
	Debug::DrawLine(portalApex, midEdgetest, Debug::BLUE, 5);

	for (int i = 1; i < nPortals && npts < maxPts; ++i)
	{
		Vector3 midEdge;
		Vector3 lengthEdge = allVerts[allEdges[i].a] - allVerts[allEdges[i].b];

		midEdge = allVerts[allEdges[i].b] + (lengthEdge * 0.5f);
		
		bool isLeft = IsLeftOfApex(portalApex, midEdge, allVerts[allEdges[i].a]);

		std::cout << "IsLeft: " << isLeft  << endl;
		Vector3 left, right;
		if (isLeft) 
		{
			left = allVerts[allEdges[i].a];
			right = allVerts[allEdges[i].b]; // this probably is incorrect, may need to take on from index i and the other from i+1
		}
		else
		{
			right = allVerts[allEdges[i].a];
			left = allVerts[allEdges[i].b];
		}
				
		triArea = Maths::FloatAreaOfTri(portalApex, portalRight, right);
		std::cout << "triArea: " << triArea << endl;
		// Update right vertex.
		if (triArea <= 0.0f)
		{
			std::cout << "Update Right Vertex." << endl;
			equal = vequal(portalApex, portalRight);
			triArea = Maths::FloatAreaOfTri(portalApex, portalLeft, right);
			std::cout << "equal: " << equal << endl;
			std::cout << "triArea: " << triArea << endl;
			if (equal || triArea > 0.0f)
			{
				// Tighten the funnel.
				std::cout << "Update Right Vertex. Tighten Funnel" << endl;
				portalRight = right;
				rightIndex = i;
			}
			else
			{
				// Right over left, insert left to path and restart scan from portal left point.
				std::cout << "Update Right Vertex. Right Over left" << endl;
				outPath.waypoints.insert(outPath.waypoints.begin(), portalLeft);
				npts++;
				// Make current left the new apex.
				portalApex = portalLeft;
				apexIndex = leftIndex;
				// Reset portal
				portalLeft = portalApex;
				portalRight = portalApex;
				leftIndex = apexIndex;
				rightIndex = apexIndex;
				// Restart scan
				i = apexIndex;
				continue;
			}
		}
		triArea = Maths::FloatAreaOfTri(portalApex, portalLeft, left);
		std::cout << "triArea: " << triArea << endl;
		// Update left vertex.
		if (triArea >= 0.0f)
		{
			std::cout << "Update Left Vertex." << endl;
			equal = vequal(portalApex, portalLeft);
			triArea = Maths::FloatAreaOfTri(portalApex, portalRight, left);
			std::cout << "equal: " << equal << endl;
			std::cout << "triArea: " << triArea << endl;
			if (equal || triArea < 0.0f)
			{
				// Tighten the funnel.
				std::cout << "Update Left Vertex. Tighten Funnel" << endl;
				portalLeft = left;
				leftIndex = i;

			}
			else
			{
				// Left over right, insert right to path and restart scan from portal right point.
			//	outPath.PushWaypoint(portalRight);
				std::cout << "Update Left Vertex. Left Over Right" << endl;
				outPath.waypoints.insert(outPath.waypoints.begin(), portalRight);
				npts++;
				// Make current right the new apex.
				portalApex = portalRight;
				apexIndex = rightIndex;
				// Reset portal
				portalLeft = portalApex;
				portalRight = portalApex;
				leftIndex = apexIndex;
				rightIndex = apexIndex;
				// Restart scan
				i = apexIndex;
				continue;

			}
		}
	}
	if (npts < maxPts)
	{
		Vector3 end = endPosition;
		yPos = triRoute.front().centroid.y;
		end.y = yPos;
		outPath.waypoints.insert(outPath.waypoints.begin(), end);
		npts++;
	}
	std::cout << "Number of points : " << npts << endl;
	std::cout << endl << endl;
	return npts;
}