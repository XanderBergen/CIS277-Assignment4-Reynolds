#include "../TreeExplorer.h"

#include <climits>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {
struct Expected {
    std::vector<int> pre;
    std::vector<int> in;
    std::vector<int> post;
    std::vector<int> level;
    std::size_t nodes;
    std::size_t leaves;
    int height;
    long long sum;
};

int checks = 0;
int failures = 0;

std::string show(const std::vector<int>& values) {
    std::ostringstream out;
    out << '[';
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i != 0) {
            out << ", ";
        }
        out << values[i];
    }
    out << ']';
    return out.str();
}

template<typename T>
std::string show(const T& value) {
    std::ostringstream out;
    out << value;
    return out.str();
}

template<typename T>
void check(const std::string& description, const T& actual, const T& expected) {
    ++checks;
    const bool pass = (actual == expected);
    if (!pass) {
        ++failures;
    }
    std::cout << "  " << (pass ? "PASS" : "FAIL") << " " << description
              << " | expected: " << show(expected)
              << " | actual: " << show(actual) << '\n';
}

void checkTree(const std::string& label, const TreeNode* root, const Expected& expected) {
    std::cout << "\n" << label << "\n";
    check("Preorder", preorder(root), expected.pre);
    check("Inorder", inorder(root), expected.in);
    check("Postorder", postorder(root), expected.post);
    check("Level order", levelOrder(root), expected.level);
    check("Nodes", countNodes(root), expected.nodes);
    check("Leaves", countLeaves(root), expected.leaves);
    check("Height", treeHeight(root), expected.height);
    check("Sum", sumValues(root), expected.sum);
}
} // namespace

int main() {
    // 1. Empty tree and double destruction.
    TreeNode* root = nullptr;
    checkTree("Empty tree", root, {{}, {}, {}, {}, 0, 0, 0, 0});
    destroyTree(root);
    destroyTree(root);
    check("Root remains null after two destroyTree calls", root == nullptr, true);

    // 2. One negative value: simultaneously a root and a leaf.
    root = new TreeNode{-4};
    checkTree("Singleton -4", root, {{-4}, {-4}, {-4}, {-4}, 1, 1, 1, -4});
    destroyTree(root);
    check("Singleton root cleared", root == nullptr, true);

    // 3. Four-node left-only chain: 8 ->left 6 ->left 4 ->left 2.
    root = new TreeNode{8};
    root->left = new TreeNode{6};
    root->left->left = new TreeNode{4};
    root->left->left->left = new TreeNode{2};
    checkTree("Left-only chain 8-6-4-2", root,
              {{8, 6, 4, 2}, {2, 4, 6, 8}, {2, 4, 6, 8}, {8, 6, 4, 2}, 4, 1, 4, 20});
    destroyTree(root);
    check("Left chain root cleared", root == nullptr, true);

    // 4. Four-node right-only chain with the SAME values.
    root = new TreeNode{8};
    root->right = new TreeNode{6};
    root->right->right = new TreeNode{4};
    root->right->right->right = new TreeNode{2};
    checkTree("Right-only chain 8-6-4-2", root,
              {{8, 6, 4, 2}, {8, 6, 4, 2}, {2, 4, 6, 8}, {8, 6, 4, 2}, 4, 1, 4, 20});
    destroyTree(root);
    check("Right chain root cleared", root == nullptr, true);

    // 5. Zero / negative / repeated values; links form:
    //          0
    //        /   \ (right branch)
    //      -3     0
    //     /      /
    //   -3      2
    root = new TreeNode{0};
    root->left = new TreeNode{-3};
    root->right = new TreeNode{0};
    root->left->left = new TreeNode{-3};
    root->right->left = new TreeNode{2};
    checkTree("Zero, negative and repeated values", root,
              {{0, -3, -3, 0, 2}, {-3, -3, 0, 2, 0}, {-3, -3, 2, 0, 0},
               {0, -3, 0, -3, 2}, 5, 2, 3, -4});

    // 6. Returned vectors are independent snapshots, and functions are read-only.
    std::vector<int> snapshot = preorder(root);
    std::vector<int> another = preorder(root);
    check("Two initial preorder calls match", snapshot, another);
    snapshot[0] = 999;
    check("Mutating a snapshot does not affect a new result", preorder(root),
          std::vector<int>{0, -3, -3, 0, 2});
    check("Root value unchanged", root->value, 0);
    check("Inorder unchanged after traversals", inorder(root),
          std::vector<int>{-3, -3, 0, 2, 0});
    check("Node count unchanged", countNodes(root), std::size_t{5});
    destroyTree(root);
    check("Mixed-value root cleared", root == nullptr, true);

    // 7. Rebuild after destruction; the new allocation is a separate tree.
    root = new TreeNode{7};
    root->right = new TreeNode{-2};
    checkTree("Rebuilt two-node tree", root,
              {{7, -2}, {7, -2}, {-2, 7}, {7, -2}, 2, 1, 2, 5});
    destroyTree(root);
    destroyTree(root);
    check("Rebuilt tree destroyed safely twice", root == nullptr, true);


    // 8. Additional test: uneven zigzag tree.
    root = new TreeNode{ 12 };
    root->left = new TreeNode{ 4 };
    root->right = new TreeNode{ 18 };
    root->left->right = new TreeNode{ 9 };
    root->left->right->left = new TreeNode{ 7 };
    root->left->right->left->right = new TreeNode{ -2 };
    root->right->left = new TreeNode{ 16 };

    checkTree("Additional test: uneven zigzag tree", root,
        { {12, 4, 9, 7, -2, 18, 16},
         {4, 7, -2, 9, 12, 16, 18},
         {-2, 7, 9, 4, 16, 18, 12},
         {12, 4, 18, 9, 16, 7, -2},
         7, 2, 5, 64 });

    destroyTree(root);
    check("Uneven zigzag root cleared", root == nullptr, true);

    std::cout << "\nTOTAL: " << (checks - failures)
        << "/" << checks << " checks passed\n";

    return failures == 0 ? 0 : 1;
}

