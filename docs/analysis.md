# Lab 23 Analysis — AVL Trees and Splay Trees

## Required Analysis

1. Explain why an AVL tree maintains `O(log n)` height.
An AVL tree maintains O(log n) height ecause it keeps the height difference between the left and right subtrees of every node at most 1. This difference is called the balance factor. When an insertion or deletion makes a node unbalanced, the tree uses rotations to restore its balance. Because the tree stays balanced, its height grows logarithmically instead of becoming a long chain.

2. Identify the four AVL imbalance cases:
   - LL: Left-Left, this is a node where it becomes unbalanced becasue a new node was added to the left subtree of its left child. It is fixed with a right rotation. 
   - RR: Right-Right, this is a node where it becomes unbalanced becasue a new node was added to the right subtree of its right child. It is fixed with a left rotation.
   - LR: Left-Right, A new node was added to the right subtree of the left child. It is fixed with a left rotation followed by a right rotation. 
   - RL: Right-Left, A new node was added to the left subtree of the right child. It is fixed with a right rotation follwed by a left rotation.

3. Explain how insertion and deletion differ in their rebalancing behavior.
When inserting, the tree first follows the nrmal BST insertion process. Then it updates the heights while moving back towards the root. If a node becomes. unbalanced, a rotation fixes it. After the first unbalanced ansector is fixed, then the height of that subtree returns to its previous height, so no addtional ansectors need rebalancing. 

When deleting, the tree removes the node using the BST deletion. It then updates heights ad chekcs balance factors while moving back towards the root. Deletion can make the height of a subtree smaller, which can cause additional ansector to become unbalanced. Because of this, multiple rotations may be needed.

4. Explain why each AVL `find`, `insert`, and `erase` operation is `O(log n)`.
Becasue it's operations only need to travel along a path whose length is proportional to the tree's height. 
   - Find searches down one path from root, so it takes O(log n). 
   - Insert finds the correct position and then updates heights and performs any needed rotations along the path back to the root. This also take O(log n).
   - Erase finds and removes the node, then updates heights and rebalances while moving back toward the root, which take O(log n).
The AVL tree itself uses O(n) space because it stores n nodes with a small amount of additional infromation for each node, such as its height. 

5. Explain the purpose of a splay operation.
A splay operation moves an accessed node to the root of the tree using rotations. The purpose is to make recently accessed nodes easier to find again. It also allows the tree to adjust based on which keys are accessed most often.

6. Describe:
   - zig: The accessed node is a direct child of the root. One rotation moves it to the root.
   - zig-zig: The accessed node and its parent are both left children or both right children. Two rotations in the same direction move the accessed node closer to the root.
   - zig-zag: The accessed node is a left child while its parent is a right child, or the opposite. Two rotations in opposite directions move the accessed node closer to the root.

7. Explain why a single splay operation can take `O(n)` in the worst case even though splay trees provide logarithmic amortized performance.
A splay tree does not always stay balanced. It can become a long chain, especially after certain insertion sequences. If the accessed node is at the bottom of a chain, moving it to the root may require many rotations. Therefore, one individual splay operation can take O(n) time in the worst case. Splay trees provide O(log n) amortized performance. This means that over a sequence of operations, the average cost per operation is O(log n), even though some individual operations can be more expensive.

8. Compare AVL and splay trees for:
| Feature | AVL Tree | Splay Tree |
|---|---|---|
| Guaranteed height | O(log n) | O(n) worst case |
| Amortized access cost | O(log n) | O(log n) |
| Repeated access to hot keys | Searches remain O(log n) | Frequently accessed keys move near the root |
| Update overhead | Updates heights and performs rotations | Performs rotations to move accessed or inserted nodes |
| Memory metadata | Stores height information | Uses parent pointers; no height metadata | 

9. Construct an access sequence where a splay tree benefits substantially from locality of reference.
Consider a splay tree containing the keys 10, 20, 30, 40, and 50. Suppose the access sequence is:

30, 30, 30, 30, 30

The first access to 30 may require several rotations to move it to the root. After that, the next accesses to 30 are very fast because it is already at the root.This shows locality of reference because the same key is accessed repeatedly. A splay tree benefits from this pattern by keeping recently accessed keys near the root.

10. Explain why a splay tree cannot be described as having `O(log n)` worst-case time for every individual operation.
Because it does not maintain a strict height balance like an AVL tree. Its structure depends on the order of insertions and accesses. Sometimes a node can be far from the root and require O(n) rotations to reach it. The O(log n) guarantee applies to amortized performance over a sequence of operations, not to every individual operation. This is why a splay tree has O(n) worst-case time for one operation but O(log n) amortized time.

## Expected Complexity

AVL:
- search: O(log n)
- insertion: O(log n)
- deletion: O(log n)
- space: O(n)

Splay:
- individual operation: O(n) worst case
- amortized operation: O(log n)
- space: O(n)
