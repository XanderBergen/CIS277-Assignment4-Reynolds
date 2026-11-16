# CIS-277 Assignment 4: Binary Tree Explorer

## Student

- **Name:** Alexander Reynolds
- **Course:** CIS-277

## Design

**Original Plan:**
TreeNode will include the fields value, left, and right. The value will hold the actual data of the node, and left and right will be pointers referencing the connected nodes, or nullpointer if there is no connected node.
For the empty-tree base case, the function will return 0, as it cannot reach the result.
Each traversal is only concerned with the data in one node, and the 2 nodes it is referencing. This means it will not have to copy entire subtree vectors.
Each node's parent will be responsible for releasing it's child.

**Changes to the plan after implementation:**
The empty-tree base case needed to be different for measurement functions and traversal functions. Measurement functions return 0, while traversal functions return an empty vector.
The traversal is actaully concerened with the full vector, but to prevent unneccesary copying it is passed by reference to each of the children by its parent, meaning the vector doesn't need to be copied.
I needed to specifically use postorder deletion to make sure child nodes are deleted before their children. This function also sets all of the pointers to nullptr after deletion.
Levelorder needed to use a FIFO queue instead of recursion, because it needs to process nodes one at a time, from left to right.

**Tree A**

```text
        50
       /  \
     30    70
    / \   / \
   20 40 60 80
```

Root: `50`.
Leaves: `20`, `40`, `60`, `80`.
Edges: `50->left=30`, `50->right=70`, `30->left=20`, `30->right=40`, `70->left=60`, `70->right=80`.

**Tree B**

```text
        10
       /  \
     30    20
       \     \
        5     15
```

Root: `10`.
Leaves: `5`, `15`.
Edges: `10->left=30`, `10->right=20`, `30->right=5`, `20->right=15`.

**Predictions for Tree A and B**

| Predicted result | Tree A | Tree B|
|---|---|---|
| Preorder | [50 30 20 40 70 60 80] | [10 30 5 20 15] |
| Inorder | [20 30 40 50 60 70 80] | [30 5 10 20 15] |
| Postorder | [20 40 30 60 80 70 50] | [5 30 15 20 10] |
| Level order | [50 30 70 20 40 70 80] | [10 30 20 5 15] |
| Nodes | [50 30 70 20 40 60 80] | [10 30 20 5 15] |
| Leaves | [4] | [2] |
| Height in levels | [3] | [3] |
| Sum | [350] | [80] |

**Reasoning for Preorder:** For preoder, you first print the node, then go left, then go right. For Tree A, the root node is 50, so you print that. Then you go left, and restart the process. It prints 30, then goes left, printing 20.
This node has no children, so you go back up one level to 30, which still needs to process right. You go right to 40 and print it, go back to 30, which has finished all 3 actions, and go to 50, which still needs to go right. It goes right to 70, prints it, and 
then goes left to 60. It prints 60, heads back up to 70 which still needs to process right, and goes right to 80 and prints it. The function then goes back up to 70, which is finished, and then goes back to 50, which is also finished and is the root, finishing the traversal.

## How to Compile

This project requires a C++23-compatible compiler and Python 3. On Windows, G++ is used through **MSYS2 UCRT64**, while Clang is used through **MSYS2 CLANG64** for memory-sanitizer testing.

From the repository root, compile the programs using G++:

```bash
g++ -std=c++23 -Wall -Wextra -pedantic main.cpp TreeExplorer.cpp -o tree_explorer.exe
g++ -std=c++23 -Wall -Wextra -pedantic -I. tests/student_tests.cpp TreeExplorer.cpp -o student_tests.exe
g++ -std=c++23 -Wall -Wextra -pedantic -I. tests/public_tests.cpp TreeExplorer.cpp -o public_tests.exe
```

For memory-sanitizer builds, use MSYS2 CLANG64:

```bash
clang++ -std=c++23 -g -fsanitize=address,undefined main.cpp TreeExplorer.cpp -o tree_explorer_asan.exe
clang++ -std=c++23 -g -fsanitize=address,undefined -I. tests/student_tests.cpp TreeExplorer.cpp -o student_tests_asan.exe
clang++ -std=c++23 -g -fsanitize=address,undefined -I. tests/public_tests.cpp TreeExplorer.cpp -o public_tests_asan.exe
```

## How to Run

From the repository root, use **MSYS2 UCRT64** to run the programs:

```bash
./tree_explorer.exe
./student_tests.exe
./public_tests.exe
```

To generate the normal test reports:

```bash
./tree_explorer.exe > tests/demo_results.txt 2>&1
./student_tests.exe > tests/student_results.txt 2>&1
./public_tests.exe > tests/public_results.txt 2>&1
```

In **MSYS2 CLANG64**, generate the memory-sanitizer reports:

```bash
./tree_explorer_asan.exe > tests/demo_sanitizer_results.txt 2>&1
./student_tests_asan.exe > tests/sanitizer_results.txt 2>&1
./public_tests_asan.exe > tests/public_sanitizer_results.txt 2>&1
```

The reports contain program output and any detected errors. Check the exit statuses and review the files to verify successful execution.

To run the official assignment validator, use MSYS2 UCRT64:

```bash
python validate.py
```

If necessary, use `python3 validate.py` instead.

## Tests and Results

The traversal results for Preorder, Inorder, Postorder, and Level order on Tree A and Tree B are identical to the values provided in the assignment. It also correctly counts the number of nodes (7 for Tree A, 5 for Tree B), leaves (4 for Tree A, 2 for Tree B),
the height in levels (3 for both Trees), and the Sum of all Nodes (350 for Tree A, 80 for Tree B). `tests/student_tests.cpp` has its own `main` and tests each of the four traversals plus all four measurements for each tree. The results are recorded in `tests/student_results.txt`.
`tests/demo_results.txt` records the console output from compiling and running main.cpp. I also recompiled the demo and test program with GCC 14.2.0 and `-fsanitize=address,undefined`, then ran each with leak detection enabled and halt-on-error settings. 
Both runs exited successfully without a reported AddressSanitizer or UndefinedBehaviorSanitizer error. Logs are in `tests/sanitizer_results.txt` and `tests/demo_sanitizer_results.txt`. The results of `tests/public_tests` are saved in `tests/public_results.txt`, the program passes all 96 checks.
`validate.py` runs correctly and passes all checks.

- **Pre:** Preorder
- **In:** Inorder
- **Post:** Postorder
- **Level:** Level-order
- **N/L/H/S:** Nodes / Leaves / Height / Sum

| Test / Input | Expected Results | Actual Results | Corrections Made |
|---|---|---|---|
| **1. Empty Tree:** `root = nullptr`, followed by two destruction calls | All four traversals empty. N/L/H/S: `0/0/0/0`. Root remains null. | Passed | None |
| **2. Singleton:** One node containing `-4` | All traversals: `-4`. N/L/H/S: `1/1/1/-4`. Root becomes null after destruction. | Passed | None |
| **3. Left-Only Chain:** `8 →L 6 →L 4 →L 2` | Pre: `8 6 4 2`; In: `2 4 6 8`; Post: `2 4 6 8`; Level: `8 6 4 2`. N/L/H/S: `4/1/4/20`. | Passed | None |
| **4. Right-Only Chain:** `8 →R 6 →R 4 →R 2` | Pre: `8 6 4 2`; In: `8 6 4 2`; Post: `2 4 6 8`; Level: `8 6 4 2`. N/L/H/S: `4/1/4/20`. | Passed | None|
| **5. Zero, Negative, and Duplicate Values:** Root `0`; left child `-3` with left child `-3`; right child `0` with left child `2` | Pre: `0 -3 -3 0 2`; In: `-3 -3 0 2 0`; Post: `-3 -3 2 0 0`; Level: `0 -3 0 -3 2`. N/L/H/S: `5/2/3/-4`. | Passed | None |
| **6. Traversal Independence:** Use Test 5's tree, run preorder twice, then change `snapshot[0]` to `999` | The next preorder remains `0 -3 -3 0 2`. Root value remains `0`, inorder remains unchanged, and node count remains `5`. | Passed | None |
| **7. Rebuild After Destruction:** New root `7` with right child `-2`; destroy twice | Pre: `7 -2`; In: `7 -2`; Post: `-2 7`; Level: `7 -2`. N/L/H/S: `2/1/2/5`. Root becomes null after both destruction calls. | Passed | None |
| **8. Additional Test — Uneven Zigzag Tree:** Root `12`; left branch `4 →R 9 →L 7 →R -2`; right branch `18 →L 16` | Pre: `12 4 9 7 -2 18 16`; In: `4 7 -2 9 12 16 18`; Post: `-2 7 9 4 16 18 12`; Level: `12 4 18 9 16 7 -2`. N/L/H/S: `7/2/5/64`. | Passed | None |

## Analysis Questions

1. Preorder: 30 -> 20 -> 40. Inorder: 20 -> 30 -> 40. Postorder: 20 -> 40 -> 30. The sequences change beccause preorder visits the root, then left, then right, inorder visits left, root, then right, and postorder visits left, right, then root. Despite the changing sequences, each traversal contains the same nodes, so the sum is identical.
2. Each node has 2 or fewer children, so B is a valid binary tree. However 30 is the left child of the root 10, but a BST would require a smaller value to be the left child, so it violates left-child ordering. B also violates right-child ordering, as 5 is the right child of 30, when the right child must be greater than the parent. This is also violated by 15 being the right child of 20. The inorder sequence is not sorted because it follows the parent-child relationships, not the values of the nodes. Inorder will only sort the sequence if it is a BST.  
3. Node 40 has a depth of 2, with the edges 50 -> 30 -> 40. When determining depth the root node is considered a depth of 0, while when determining the number of levels the root node is 1. Therefore the tree has 3 levels, despite Node 40 having a depth of 2.
4. You enqueue the root, dequeue the front node, append its value, and then enqueue both its left and right child. The nodes enqueued first are processed first, so earlier levels finish before later levels, from left to right.
5. For 'n' nodes, height 'h', and maximum width 'w', each DFS traversal takes O(n) time and O(h) auxiliary-space in the stack. Level order takes O(n) time and O(w) auxiliary-space in the queue. Recursive counting, recursive destruction, height, and sum all take O(n) time and O(h) auxiliary-space on the stack.
6. Postorder deletion guarantees that the children are destroyed before the parent, which contains their pointers. 'TreeNode*&' is a reference to the caller's pointer, so 'root = nullptr' clears the caller's root aswell. Deleting a parent before its child pointers and then accessing those pointers afterword would cause undefined behavior, as the memory containing the parent was already freed.


## AI Use

I used Chatgpt to generate all of the files in the project individually, and then checked to verify that they worked as described in the assignment. My biggest struggle with the AI was trying to get the student_tests.cpp file to work correctly in Visual Studio.
The code for the tests was solid, but no matter what I did, I was unable to get it to compile correctly within visual studio, as both student_tests.cpp and main.cpp had a main function, and student_tests was located in a subfolder, which it would not allow me to include in my project.
I realized that I should compile student_tests.cpp seperately along with TreeExplorer.cpp, which avoided the issues with Visual Studio's file structure. I learned that while Chatgpt is quite reliable with producing code, it will not always account for differences between IDEs, 
which I need to be careful about when taking it's code.