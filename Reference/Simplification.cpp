#include "Simplification.h"
#include "Mesh.h"

void Simplification::InitSimplification(Mesh *mesh_in)
{
	mesh = mesh_in;
	n_active_faces = mesh->n_faces;

	AssignInitialQ();
	for (EdgeIter ei = mesh->edges.begin(); ei != mesh->edges.end(); ei++)
		ComputeOptimalCoordAndCost(ei);   //为每条边计算collapse后产生的vertex

}

void Simplification::AssignInitialQ()
{
	// 对于mesh的每个顶点
	for (VertexIter vi = mesh->vertices.begin; vi != mesh->vertices.end(); vi++) {
		// 初始化为0
		for (int i = 0; i < 10; i++)
			vi->Q[i] = 0.0;

		HalfEdge *startHalfEdge, *endHalfEdge;
		if (vi->isBoundary == false)
			startHalfEdge = vi->neighborHe;  // 以该点为出发的边
		else
			startHalfEdge = FindBoundaryEdgeIncidentToVertexInCW(vi->neighborHe);  // ?

		HalfEdge *hep = startHalfEdge;  // 循环控制变量，从startHalfEdge开始
		// 遍历该点邻接的每个边
		do {
			CumulateQ(vi, hep->face->normal, -DotProduct(hep->face->normal, hep->face->halfedge[0].vertex->coord));
			if (vi->isBoundary && hep->prev->mate == NULL) {
				endHalfEdge = hep->prev;
				break;
			}
			hep = hep->prev->mate;
		} while (hep != startHalfEdge && hep != NULL);

		//如果是边界点另外处理（把Error变得很大？）
		if (vi->isBoundary)
		{
			// add pseudo face infomation to vi->Q
			// vi邻接一个虚拟的面
			double boundaryVector[3], pseudoNormal[3]; //边界边向量

			for (int i = 0; i < 3; i++)  // 该点关联的第一条边界边
				boundaryVector[i] = startHalfEdge->next->vertex->coord[i] - startHalfEdge->vertex->coord[i];

			CrossProduct(boundaryVector, startHalfEdge->face->normal, pseudoNormal);
			Normalize(pseudoNormal);
			CumulateQ(vi, pseudoNormal, -DotProduct(pseudoNormal, startHalfEdge->vertex->coord));

			for (int i = 0; i < 3; i++)
				boundaryVector[i] = endHalfEdge->next->vertex->coord[i] - endHalfEdge->vertex->coord[i];

			CrossProduct(boundaryVector, endHalfEdge->face->normal, pseudoNormal);
			Normalize(pseudoNormal);
			CumulateQ(vi, pseudoNormal, -DotProduct(pseudoNormal, endHalfEdge->vertex->coord));
		}
	}
}