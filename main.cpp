#include "TreeExplorer.h"

#include <iostream>
#include <string>
#include <vector>

namespace {
    TreeNode* buildTreeA() {
        // All seven nodes are individually allocated; links match the diagram.
        TreeNode* root = new TreeNode{ 50 };
        root->left = new TreeNode{ 30 };
        root->right = new TreeNode{ 70 };
        root->left->left = new TreeNode{ 20 };
        root->left->right = new TreeNode{ 40 };
        root->right->left = new TreeNode{ 60 };
        root->right->right = new TreeNode{ 80 };
        return root;
    }

    TreeNode* buildTreeB() {
        // Intentionally NOT a BST: 30 lies to the left of 10.
        TreeNode* root = new TreeNode{ 10 };
        root->left = new TreeNode{ 30 };
        root->right = new TreeNode{ 20 };
        root->left->right = new TreeNode{ 5 };
        root->right->right = new TreeNode{ 15 };
        return root;
    }

    void printSequence(const std::string& label, const std::vector<int>& values) {
        std::cout << label << ":";
        for (int value : values) {
            std::cout << ' ' << value;
        }
        std::cout << '\n';
    }

    void displayTree(const std::string& label, const TreeNode* root) {
        std::cout << label << '\n';
        printSequence("  Preorder", preorder(root));
        printSequence("  Inorder", inorder(root));
        printSequence("  Postorder", postorder(root));
        printSequence("  Level order", levelOrder(root));
        std::cout << "  Nodes: " << countNodes(root) << '\n'
            << "  Leaves: " << countLeaves(root) << '\n'
            << "  Height (levels): " << treeHeight(root) << '\n'
            << "  Sum: " << sumValues(root) << "\n\n";
    }
} // namespace

int main() {
    TreeNode* treeA = buildTreeA();
    TreeNode* treeB = buildTreeB();

    displayTree("Tree A (BST)", treeA);
    displayTree("Tree B (not a BST)", treeB);

    destroyTree(treeA);
    destroyTree(treeB);
    std::cout << std::boolalpha
        << "Tree A root is null after destruction: " << (treeA == nullptr) << '\n'
        << "Tree B root is null after destruction: " << (treeB == nullptr) << '\n';
}
