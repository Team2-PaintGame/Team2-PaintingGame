#include "NavigationMesh.h"
#include "Assets.h"
#include "Maths.h"
#include <fstream>
#include "Debug.h"
using namespace NCL;
using namespace CSC8508;

NavigationMesh::NavigationMesh()
{
}

NavigationMesh::NavigationMesh(const std::string&filename)
{
	std::ifstream file(Assets::DATADIR + filename);

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

	if (start == nullptr) {
		//std::cout << "start is outside of the navmesh\n";
		isOutNavMesh = true;
		return false;
	}
	/*const*/ NavTri* end	= GetTriForPosition(to);
	if (end == nullptr) {
		//std::cout << "end is outside of the navmesh\n";
		return false;
	}

	std::vector<NavTri*> openList;
	std::vector<NavTri*> closedList;

	openList.emplace_back(start); 

	start->f = 0;
	start->g = 0;
	start->parent = nullptr;
	NavTri* currentBestTri = nullptr;

	while (!openList.empty()) {
		currentBestTri = RemoveBestTri(openList);

		if (currentBestTri == end) {
			NavTri* tri = end;
			triRoute.clear();
			while (tri != nullptr) {
				triRoute.emplace_back(*tri);
				tri = tri->parent;
			}
			if (triRoute.size() > 2)
			{
				FindRouteVertices();
				FindPortalEdges(to);
			}
			StringPull(from, to, outPath);
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


void NavigationMesh::FindRouteVertices() {
	routeVertices.clear();
	std::vector<NavTri>::reverse_iterator rit = triRoute.rbegin();
	for (; rit != triRoute.rend(); rit++) {
		for (int i = 0; i < 3; ++i)
		{
			Vector3 vertex = allVerts[rit->indices[i]];
			routeVertices.emplace_back(vertex);
		}
	}
}

inline float vdistsqr(const Vector3 a, const Vector3 b)
{
	const float dx = b[0] - a[0];
	const float dy = b[1] - a[1];
	const float dz = b[2] - a[2];
	float ans = dx * dx + dy * dy + dz * dz;
	//sqrt(ans);
	return ans;  // maybe this should be square root?
}

inline bool vequal(const Vector3 a, const Vector3 b)
{
	static const float eq = 0.001f * 0.001f;
	return vdistsqr(a, b) < eq;
}

void NavigationMesh::FindLeftAndRightVertex(Vector3 portalApex, int triIndex, Vector3& portalLeft, Vector3& portalRight) {
	int k = 0;
	Vector3 edgeVertex[2];
	
	for (int i = triIndex * 3; i < (triIndex * 3) + 3; ++i)
	{
		for (int j = (triIndex * 3 ) +3; j < (triIndex * 3 ) + 6; ++j)
		{
			if (routeVertices[i] == routeVertices[j])
			{
				edgeVertex[k] = routeVertices[i];
				++k;
			}
		}
	}
	Vector3 midEdge = (edgeVertex[0] - edgeVertex[1]) * 0.5f;
	midEdge += edgeVertex[0];

	float triArea = Maths::FloatAreaOfTri(portalApex, edgeVertex[0], edgeVertex[1]);

	if (triArea <= 0.0f)
	{
		portalLeft = edgeVertex[1];
		portalRight = edgeVertex[0];
	}
	else
	{
		portalLeft = edgeVertex[0];
		portalRight = edgeVertex[1];
	}

}

void NavigationMesh::StringPull(Vector3 startPosition, Vector3 endPosition, NavigationPath& outPath) {

	outPath.Clear();
	if (triRoute.size() <= 2)
	{
		float yPos = triRoute.back().centroid.y;
		Vector3 end = endPosition;
		end.y = yPos;
		outPath.PushWaypoint(end);
		return;
	}

	// Find straight path.
	int npts = 0;
		
	int maxPts = triRoute.size(); //what should the max points be??
	int nPortals = maxPts - 1;
	Vector3 portalApex, portalLeft, portalRight;
	int apexIndex = 0, leftIndex = 0, rightIndex = 0;

	float yPos = triRoute.back().centroid.y;
	portalApex = startPosition;
	portalApex.y = yPos;

	portalLeft = portalEdges[0].leftPortal;
	portalRight = portalEdges[0].rightPortal;
	Vector3 left, right;

	// Add start point.
	outPath.PushWaypoint(portalApex);
	npts++;

	bool equal;
	float triArea;
	for (int i = 1; i <= nPortals && npts < maxPts; ++i)
	{
		left = portalEdges[i-1].left;
		right = portalEdges[i-1].right;

		triArea = Maths::FloatAreaOfTri(portalApex, portalRight, right);
		// Update right vertex.
		if (triArea <= 0.0f) // if right tightens the funnel
		{
			equal = vequal(portalApex, portalRight);
			triArea = Maths::FloatAreaOfTri(portalApex, portalLeft, right);
			if (equal || triArea > 0.0f) // check if right is also inside leftportal
			{
				// Tighten the funnel.
				portalRight = right;
				rightIndex = i;
			}
			else
			{
				// Right over left, insert left to path and restart scan from portal left point.
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
		// Update left vertex.
		if (triArea >= 0.0f)
		{
			equal = vequal(portalApex, portalLeft);
			triArea = Maths::FloatAreaOfTri(portalApex, portalRight, left);
			if (equal || triArea < 0.0f)
			{
				// Tighten the funnel.
				portalLeft = left;
				leftIndex = i;
			}
			else
			{
				// Left over right, insert right to path and restart scan from portal right point.
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

	if (npts < maxPts )
	{
		Vector3 end = endPosition;
		yPos = triRoute.front().centroid.y;
		end.y = yPos;
		outPath.waypoints.insert(outPath.waypoints.begin(), end);
		npts++;
	}
	return;
}

void NavigationMesh::FindPortalEdges(const Vector3& to) {
	int numTris = triRoute.size();
	std::vector<NavTri>::reverse_iterator it = triRoute.rbegin();
	int i = 0;
	portalEdges.clear();
	for (; it != triRoute.rend() - 1; it++)
	{
		PortalEdge portalEdge;
		Vector3 portalLeft, portalRight;
		FindLeftAndRightVertex(it->centroid, i, portalLeft, portalRight);
		portalEdge.leftPortal = portalLeft;
		portalEdge.rightPortal = portalRight;
		portalEdges.emplace_back(portalEdge);
		i++;
	}

	
	for (int i = 0; i < portalEdges.size(); ++i)
	{
		if (i == portalEdges.size() - 1)
		{
			portalEdges[i].left = to;
			portalEdges[i].right = to;	
		}
		else
		{
			portalEdges[i].left = portalEdges[i + 1].leftPortal;
			portalEdges[i].right = portalEdges[i + 1].rightPortal;
		}
	}
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

		if (abs(areaSum - t.area) > 0.001f) { //floating points are annoying! Are we more or less inside the triangle?
			continue;
		}
		return &t;
	}
	return nullptr;
}

Vector3 NavigationMesh::FindClosestPoint(Vector3 to) {
	NavTri* end = GetTriForPosition(to);
	if (end == nullptr) {
		float minDistance = FLT_MAX;
		float distance = FLT_MAX;
		Vector3 centroid;
		for (auto i : allTris)
		{
			distance = (i.centroid - to).Length();
			if (distance < minDistance)
			{
				centroid = i.centroid;
				minDistance = distance;
				
			}
		}
		end = GetTriForPosition(centroid);

		Vector3 centroidToPoint = to - centroid;
		float ratio = 0.95;
		Vector3 toPoint = centroid + centroidToPoint * ratio;
		NavTri* test = GetTriForPosition(toPoint);
		while (test == nullptr)
		{
			//std::cout << "Not close enough\n";
			ratio -= 0.05;
			toPoint = centroid + centroidToPoint * ratio;
			test = GetTriForPosition(toPoint);
		}
		//Debug::DrawLine(centroid, toPoint, Debug::YELLOW, 1);

		return toPoint;

	}
	else // destination is inside the navmesh
	{
		return to;
	}



}


