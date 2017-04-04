#pragma once
#include <vector>
#include <list>
#include <iostream>
using namespace std;

struct HalfEdge;
struct Vertex;
struct Face;
struct Edge;

struct HalfEdge {
	Vertex* vert;
	Face* face;

	HalfEdge* next;
	HalfEdge* prev;
	HalfEdge* pair;
};

struct Edge {
	HalfEdge *pairHalfEdge[2];
};

struct Vertex {
	float position[3];
	float normal[3];
	float color[3];
	int id;

	HalfEdge* halfEdge;  // outgoning edge
};

struct Face {
	HalfEdge* circularEdge[3];  // circularEdge[0]->next = circularEdge[1];
	int id;
};

class Mesh {
public:
	list<Vertex> vertices;
	list<Face> faces;
	list<Edge> edges;
	int n_vertices, n_faces, n_edges;
};