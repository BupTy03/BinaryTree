#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include "BinaryNode.h"
#include "range.hpp"

using namespace std;
using namespace my;

int main()
{
	system("chcp 1251");
	system("cls");

	BinaryNode<int> root(5);
	root.addNode(2);
	root.addNode(6);
	root.addNode(1);
	root.addNode(4);
	root.addNode(9);
	root.addNode(3);
	root.addNode(7);
	root.addNode(10);
	root.addNode(8);

	TraverseDepthFirst(&root, [](int i) { cout << i << " "; });
	cout << endl;

	root.deleteNode(9);

	TraverseDepthFirst(&root, [](int i) { cout << i << " "; });
	cout << endl;

#if 0
	linear_congruential_engine<unsigned int, 1, 1, 10> rand_engine(((chrono::high_resolution_clock::now()).time_since_epoch()).count());
	int tmp = rand_engine();
	cout << "[ " << tmp << " ";
	BinaryNode<int> root(tmp);

	for (auto i : range<int>(9))
	{
		tmp = rand_engine();
		root.addNode(tmp);
		cout << tmp << " ";
	}

	cout << "]\nTraversePreorder: [ ";
	TraversePreorder(&root, [](int t) { cout << t << " "; });

	cout << "]\nTraverseInorder: [ ";
	TraverseInorder(&root, [](int t) { cout << t << " "; });

	cout << "]\nTraversePostorder: [ ";
	TraversePostorder(&root, [](int t) { cout << t << " "; });

	cout << "]\nTraverseDepthFirst: [ ";
	TraverseDepthFirst(&root, [](int t) { cout << t << " "; });
	cout << "]" << endl;

	cout << "Enter value to find: ";
	int val{}; cin >> val;

	BinaryNode<int>* node = root.findNode(val);
	if (node == nullptr)
		cout << "Node is not found :(" << endl;
	else
		cout << "Node is found: " << node->data() << endl;
#endif

	system("pause");
	return 0;
}