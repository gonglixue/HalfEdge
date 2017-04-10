#include "mesh.h"

void Mesh::ReadObj(string filename)
{
	ifstream input_file(filename);  // 打开obj文件
	const int LINE_LENGTH = 100;
	n_vertices = 0;
	n_edges = 0;
	n_faces = 0;

	while (input_file) {
		string type;
		input_file >> type;  // 读取一词(space split) v or f
		
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
				int a, b, c; //面的顶点索引
				input_file >> a >> b >> c;
				// 创建3个HalfEdge
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
				// 插入到map
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
					// ab 存在pair ba
					he1->pair = mapIt->second;  // 迭代器seconde就是value
					
				}

				mapIt = heMap.find(EdgeIndexPair(c, b));
				if (mapIt != heMap.end()) {
					he2->pair = mapIt->second;
				}
				
				mapIt = heMap.find(EdgeIndexPair(a, c));
				if (mapIt != heMap.end()) {
					he3->pair = mapIt->second;
				}

				// 此时对于三条边，halfEdge结构构建完毕
				// 构建面
				Face f(he1, he2, he3, n_faces);
				this->faces.push_back(f);

				n_faces++;
			}
			}
		}
	}
}