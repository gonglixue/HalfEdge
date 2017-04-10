#pragma once
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

struct HalfEdge;
struct Vertex;
struct Face;
struct Edge;

typedef pair<int, int> EdgeIndexPair;
typedef map<EdgeIndexPair, HalfEdge*> HalfEdgeMap;

struct HalfEdge {
	Vertex* vert;
	Face* face;

	HalfEdge* next;
	HalfEdge* prev;
	HalfEdge* pair;
};

struct Edge {
	HalfEdge *pairHalfEdge[2];

	Edge(HalfEdge* half1) {
		pairHalfEdge[0] = half1;
	}
	Edge(HalfEdge* half1, HalfEdge* half2)
	{
		pairHalfEdge[0] = half1;
		pairHalfEdge[1] = half2;
	}
};

struct Vertex {
	float position[3];
	float normal[3];
	float color[3];
	int id;

	HalfEdge* halfEdge;  // outgoning edge？一个顶点的outgoing edge不是有很多吗？

	Vertex(float x, float y, float z, int _id) {
		position[0] = x;
		position[1] = y;
		position[2] = z;
		id = _id;
	}
};

struct Face {
	HalfEdge* circularEdge[3];  // circularEdge[0]->next = circularEdge[1];
	int id;

	Face(Vertex* v0, Vertex* v1, Vertex* v2, int _id)
	{
		circularEdge[0]->vert = v0;
		circularEdge[1]->vert = v1;
		circularEdge[2]->vert = v2;
		id = _id;
	}
	Face(HalfEdge* he1, HalfEdge* he2, HalfEdge* he3, int _id)
	{
		circularEdge[0] = he1;
		circularEdge[1] = he2;
		circularEdge[2] = he3;
		id = _id;
	}
};

class Mesh {
public:
	vector<Vertex> vertices;
	vector<Face> faces;
	vector<Edge> edges;
	HalfEdgeMap heMap;
	int n_vertices, n_faces, n_edges;

	void ReadObj(string filename);
};