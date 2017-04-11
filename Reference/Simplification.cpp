#include "Simplification.h"
#include "Mesh.h"

void Simplification::InitSimplification(Mesh *mesh_in)
{
	mesh = mesh_in;
	n_active_faces = mesh->n_faces;

	AssignInitialQ();
	for (EdgeIter ei = mesh->edges.begin(); ei != mesh->edges.end(); ei++)
		ComputeOptimalCoordAndCost(ei);   //Ϊÿ���߼���collapse�������vertex

}

void Simplification::AssignInitialQ()
{
	// ����mesh��ÿ������
	for (VertexIter vi = mesh->vertices.begin; vi != mesh->vertices.end(); vi++) {
		// ��ʼ��Ϊ0
		for (int i = 0; i < 10; i++)
			vi->Q[i] = 0.0;

		HalfEdge *startHalfEdge, *endHalfEdge;
		if (vi->isBoundary == false)
			startHalfEdge = vi->neighborHe;  // �Ըõ�Ϊ�����ı�
		else
			startHalfEdge = FindBoundaryEdgeIncidentToVertexInCW(vi->neighborHe);  // ?

		HalfEdge *hep = startHalfEdge;  // ѭ�����Ʊ�������startHalfEdge��ʼ
		// �����õ��ڽӵ�ÿ����
		do {
			CumulateQ(vi, hep->face->normal, -DotProduct(hep->face->normal, hep->face->halfedge[0].vertex->coord));
			if (vi->isBoundary && hep->prev->mate == NULL) {
				endHalfEdge = hep->prev;
				break;
			}
			hep = hep->prev->mate;
		} while (hep != startHalfEdge && hep != NULL);

		//����Ǳ߽�����⴦����Error��úܴ󣿣�
		if (vi->isBoundary)
		{
			// add pseudo face infomation to vi->Q
			// vi�ڽ�һ���������
			double boundaryVector[3], pseudoNormal[3]; //�߽������

			for (int i = 0; i < 3; i++)  // �õ�����ĵ�һ���߽��
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