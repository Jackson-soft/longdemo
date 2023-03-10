#pragma once
// 二叉树
#include <iostream>

template<typename T>
class BinaryTree {
public:
    struct TreeNode {
        T         data;
        TreeNode *left;
        TreeNode *right;

        TreeNode() {
            left = right = nullptr;
        }
    };

    BinaryTree() = default;

    ~BinaryTree() {
        root = nullptr;
    }

    // 压入
    void Push(T t) {
        auto *tn = new TreeNode();
        tn->data = t;
        if (root == nullptr) {
            root = tn;
        }
    }

    // 弹出
    void Pop() {}

    //
    void Display() {
        PreOrder(root);
    }

private:
    // 前序遍历
    void PreOrder(TreeNode *r) {
        if (r != nullptr) {
            std::cout << r->data << " ";
            PreOrder(r->left);
            PreOrder(r->right);
        }
    }

    TreeNode *root{nullptr};
};
