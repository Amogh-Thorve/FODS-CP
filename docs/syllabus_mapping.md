# CONFIGCORE Syllabus Mapping Report

| Unit | Data Structure / Algorithm | Source File | Function / Struct | CPQ Domain Application | Demo Available |
|------|---------------------------|-------------|-------------------|------------------------|----------------|
| **Unit 1** | Dynamic & Static Arrays | `arrays.c`, `arrays.h` | `ProductArray`, `OptionArray` | Storing product catalog and options in C memory | Yes |
| **Unit 1** | Linear Search | `search.c` | `linearSearch()` | Unsorted product catalog lookup | Yes |
| **Unit 1** | Binary Search | `search.c` | `binarySearch()` | Sorted product SKU / ID search | Yes |
| **Unit 1** | Indexed Sequential Search | `search.c` | `indexedSequentialSearch()` | Block-indexed fast product search | Yes |
| **Unit 1** | Interpolation Search | `search.c` | `interpolationSearch()` | Uniform price-indexed search | Yes |
| **Unit 1** | Fibonacci Search | `search.c` | `fibonacciSearch()` | Non-multiplicative search | Yes |
| **Unit 1** | 10 Sorting Algorithms (Bubble, Selection, Insertion, Merge, Quick, Heap, Radix, Shell, Counting, Bucket) | `sorting.c` | `bubbleSort()`, `quickSort()`, etc. | Ranking recommendations by score and price | Yes |
| **Unit 2** | Singly Linked List | `linked_list.c` | `ConfigLinkedList` | Active build configuration vector | Yes |
| **Unit 2** | Doubly Linked List | `doubly_list.c` | `DoublyHistoryList` | Timeline history & Undo/Redo state tracking | Yes |
| **Unit 2** | Circular Linked List | `circular_list.c` | `CircularList` | Round-robin quote generation worker rotation | Yes |
| **Unit 2** | Josephus Problem | `josephus.c` | `solveJosephus()` | Circular list elimination solver | Yes |
| **Unit 2** | Generalized Linked List | `generalized_list.c` | `GListNode` | Nested product option sublists | Yes |
| **Unit 3** | Stack (Array & List) | `stack.c` | `ArrayStack`, `ListStack` | Configuration undo stack & DFS traversals | Yes |
| **Unit 3** | Infix → Postfix / Prefix Conversion | `expression.c` | `infixToPostfix()` | CPQ rule condition parsing | Yes |
| **Unit 3** | Postfix Expression Evaluation | `expression.c` | `evaluatePostfix()` | Automated rule condition evaluation | Yes |
| **Unit 3** | Queue (Array & List) | `queue.c` | `ArrayQueue`, `ListQueue` | FIFO quote processing job queue | Yes |
| **Unit 3** | Circular Queue | `circular_queue.c` | `CircularQueue` | Fixed-capacity task buffer | Yes |
| **Unit 3** | Deque (Double Ended Queue) | `deque.c` | `Deque` | Priority job scheduling | Yes |
| **Unit 4** | General Tree | `tree.c` | `TreeNode` | Category → Product → Option hierarchy | Yes |
| **Unit 4** | General Tree → Binary Tree Conversion | `tree_conversion.c` | `convertGeneralToBinary()` | Left-Child Right-Sibling conversion | Yes |
| **Unit 4** | Binary Tree Traversals (Pre, In, Post, Level) | `binary_tree.c` | `preorderRecursive()`, `inorderNonRecursive()` | Recursive and stack/queue non-recursive traversals | Yes |
| **Unit 4** | Binary Search Tree (BST) | `bst.c` | `BSTNode`, `insertBST()`, `deleteBST()` | Ordered product catalog index | Yes |
| **Unit 4** | Threaded Binary Tree | `threaded_tree.c` | `ThreadedNode` | Inorder traversal without recursion | Yes |
| **Unit 5** | Graph (Adjacency Matrix & List) | `graph.c` | `Graph` | Product compatibility & incompatibility graph | Yes |
| **Unit 5** | BFS & DFS Traversals | `bfs.c`, `dfs.c` | `runBFS()`, `runDFSStack()` | Compatibility level exploration | Yes |
| **Unit 5** | Cycle Detection | `cycle_detection.c` | `detectCycle()` | Circular dependency detection in configuration rules | Yes |
| **Unit 5** | Connected Components | `connected_graph.c` | `findConnectedComponents()` | Grouping compatible product clusters | Yes |
| **Unit 5** | Bipartite Graph | `bipartite.c` | `checkBipartite()` | Products vs Options 2-set mapping | Yes |
| **Unit 5** | Dijkstra's Shortest Path | `dijkstra.c` | `runDijkstra()` | Minimum cost path between components | Yes |
| **Unit 5** | Prim's & Kruskal's MST | `prim.c`, `kruskal.c` | `runPrim()`, `runKruskal()` | Minimum Spanning Tree for network connectivity | Yes |
| **Unit 5** | Disjoint Set Union-Find | `union_find.c` | `makeSet()`, `findSet()`, `unionSet()` | Kruskal MST cycle check with path compression | Yes |
| **Unit 6** | Hash Table (Chaining & Open Addressing) | `hashing.c` | `ChainingHashTable`, `OpenAddressingHashTable` | Fast SKU → Product lookup | Yes |
| **Unit 6** | Perfect Hashing | `perfect_hash.c` | `buildPerfectHashTable()` | Static material and voltage code lookup | Yes |
| **Unit 6** | Extendible Hashing | `extendible_hash.c` | `insertExtendibleHash()` | Dynamic directory & bucket splits | Yes |
| **Unit 6** | Sparse Matrix (Simple vs Fast Transpose) | `sparse_matrix.c`, `transpose.c` | `simpleTranspose()`, `fastTranspose()` | Compatibility matrix transposition benchmark | Yes |
