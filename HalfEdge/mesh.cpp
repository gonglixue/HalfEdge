#include "mesh.h"

void Mesh::ReadObj(string filename)
{
	ifstream input_file(filename);  // ��obj�ļ�
	const int LINE_LENGTH = 100;
	n_vertices = 0;
	n_edges = 0;
	n_faces = 0;

	while (input_file) {
		string type;
		input_file >> type;  // ��ȡһ��(space split) v or f
		
		if (type.length == 1) {
			switch (type[0]) {
			case '#': {
				char comment[LINE_LENGTH];
				input_file.getline(comment, LINE_LENGTH);
				printf("# %s\n", comment);
				break;
			}
			case 'v': {
				float x, y, z;
				input_file >> x >> y >> z;
				Vertex vert(x, y, z, n_vertices);
				this->vertices.push_back(vert);

				n_vertices++;
				break;
			}
			case 'f': {
				int a, b, c; //��Ķ�������
				input_file >> a >> b >> c;
				// ����3��HalfEdge
				HalfEdge *he1, *he2, *he3;
				he1 = (HalfEdge*)malloc(sizeof(HalfEdge));
				he2 = (HalfEdge*)malloc(sizeof(HalfEdge));
				he3 = (HalfEdge*)malloc(sizeof(HalfEdge));

				he1->vert = &this->vertices[a];
				he2->vert = &this->vertices[b];
				he3->vert = &this->vertices[c];
				// circular
				he1->next = he2;
				he2->next = he3;
				he3->next = he1;
				he1->prev = he3;
				he2->prev = he1;
				he3->prev = he2;
				// ���뵽map
				EdgeIndexPair edgeIndex(a, b);
				heMap.insert(map<EdgeIndexPair, HalfEdge*>::value_type(edgeIndex, he1));
				edgeIndex = EdgeIndexPair(b, c);
				heMap.insert(map<EdgeIndexPair, HalfEdge*>::value_type(edgeIndex, he2));
				edgeIndex = EdgeIndexPair(c, a);
				heMap.insert(map<EdgeIndexPair, HalfEdge*>::value_type(edgeIndex, he3));

				// TODO Edge
				// ...

				// find pair
				HalfEdgeMap::iterator mapIt = heMap.find(EdgeIndexPair(b,a));
				if (mapIt != heMap.end()) {
					// ab ����pair ba
					he1->pair = mapIt->second;  // ������seconde����value
					
				}

				mapIt = heMap.find(EdgeIndexPair(c, b));
				if (mapIt != heMap.end()) {
					he2->pair = mapIt->second;
				}
				
				mapIt = heMap.find(EdgeIndexPair(a, c));
				if (mapIt != heMap.end()) {
					he3->pair = mapIt->second;
				}

				// ��ʱ���������ߣ�halfEdge�ṹ�������
				// ������
				Face f(he1, he2, he3, n_faces);
				this->faces.push_back(f);

				n_faces++;
			}
			}
		}
	}
}