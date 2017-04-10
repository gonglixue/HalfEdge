#pragma once
#include "Mesh.h"
struct EdgeCollapseTarget {
	EdgeIter ei;  //边的指针
	double cost;
	double optimalCoord[3];  // collapse后的坐标
	int id;

	EdgeCollapseTarget(){}	// constructor
	// 为什么边的指针要引用
	EdgeCollapseTarget(EdgeIter &ei_in, double cost_in, double *optimalCoord_in, int id_in) {
		ei = ei_in;
		cost = cost_in;
		for (int i = 0; i < 3; i++) {
			optimalCoord[i] = optimalCoord_in[i];
		}
		id = id_in;
	}
}; 

struct VertexSplitTarget {
	EdgeIter ei;				// 边的指针。？split将要产生的边？
	double v1OriginalCoord[3];	// ?
	bool v1OriginalIsBoundary;
	vector<HalfEdge*> halfedgesAroundV0;

	int id;

	VertexSplitTarget(){}
};

struct greater {
	bool operator()(EdgeCollapseTarget left, EdgeCollapseTarget right) const
	{
		return left.cost > right.cost;
	}
};

class Simplification
{
	Mesh *mesh;
	priority_queue <EdgeCollapseTarget, deque<EdgeCollapseTarget>, greater> heap;	// 需要的是最小堆，所以是greater不是less
	list<EdgeCollapseTarget> suspendedEdgeCollapseTarget;

	stack<VertexSplitTarget> vertexSplitTarget;
	stack<EdgeCollapseTarget> readedEdgeCollapseTarget;

	int ect_id_base;	// edge collapse id base. 已进行collapse操作的次数
	int n_active_faces;

	void AssignInitialQ();
	void CumulateQ(VertexIter &vi, double *normal, double d);
	void ComputeOptimalCoordAndCost(EdgeIter &ei);

	HalfEdge* FindBoundaryEdgeIncidentToVertexInCW(HalfEdge *baseHalfEdge);	//?
	void FandNeiborhalfEdge(VertexIter &vi, vector<FaceIter> &faceOriginallyIncidentToV0OrV1);

	bool IsFindWillNotBeCreated(EdgeIter &ei);
	void RemoveEdge(EdgeIter &ei, double *optimalCoord, bool isFirstCollapse);

public:
	Simplification() { ect_id_base = 0; }

	void InitSimplification(Mesh *mesh_in);
	bool EdgeCollapse();
	void VertexSplit();
	void ControlLevelOfDetail(int step);

};