#include <iostream>
#include <vector>

using namespace std;
struct node;
typedef struct node Node;
struct node {
	int a;
	Node* next;
};
class Test {
public:
	vector<Node*> vec;
	// vector<Node> vec;
	Test() {
		Node* node1, *node2;
		node1 = (Node*)malloc(sizeof(Node));
		node2 = (Node*)malloc(sizeof(Node));
		node1->a = 1;
		node2->a = 2;
		node1->next = node2;

		vec.push_back(node1);
		vec.push_back(node2);

		node1->a = 0;
	}
	void Traversal(){
		vector<Node*>::iterator it;
		for (it = vec.begin(); it != vec.end(); it++)
		{
			cout << (*it)->a << endl;
		}
		cout << endl;
		cout << vec[0]->next->a << endl;
	}

};

void main()
{
	Test t;
	t.Traversal();
}