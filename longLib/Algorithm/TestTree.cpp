#include <iostream>

#include "BinaryTree.hpp"

int main()
{
	BinaryTree<int> bt;
	bt.Push(3);
	bt.Push(4);
	bt.Push(8);
	bt.Push(5);
	bt.Display();
	return 0;
}
