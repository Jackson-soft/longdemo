#pragma once
//二叉树
#include <iostream>

template <typename T>
class BinaryTree
{
public:
	struct TreeNode {
		T data;
		TreeNode *left;
		TreeNode *right;

		TreeNode() { left = right = nullptr; }
	};
	BinaryTree() { root = nullptr; }
	~BinaryTree() { root = nullptr; }

	//压入
	void Push(T t);

	//弹出
	void Pop();

	//
	void Display() { PreOrder(root); }

private:
	TreeNode *root;

	//前序遍历
	void PreOrder(TreeNode *r);
};

template <typename T>
void BinaryTree<T>::PreOrder(TreeNode *r)
{
	if (r != nullptr) {
		std::cout << r->data << " ";
		PreOrder(r->left);
		PreOrder(r->right);
	}
}

template <typename T>
void BinaryTree<T>::Push(T t)
{
	TreeNode *tn = new TreeNode();
	tn->data	 = t;
	if (root == nullptr) {
		root = tn;
	}
}

template <typename T>
void BinaryTree<T>::Pop()
{
}
