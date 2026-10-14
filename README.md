# CONFIGCORE
### Data Structures Based Product Configuration Engine for CPQ

**CONFIGCORE** is an academic-grade + product-quality application designed to demonstrate a **complete Data Structures & Algorithms syllabus** mapped directly onto an enterprise **CPQ (Configure, Price, Quote)** platform.

---

## 🚀 Key Features

- **Pure C Core Backend**: Zero fake simulations. All configuration vectors, undo timelines, rule stack evaluations, graph compatibility, pricing, and quote job scheduling execute strictly in C memory.
- **Complete Syllabus Coverage**:
  - **Arrays & Searching**: Linear, Binary, Indexed Sequential, Interpolation, Fibonacci Search.
  - **Sorting Benchmark**: 10 manual sorting algorithms (Bubble, Selection, Insertion, Merge, Quick, Heap, Radix, Shell, Counting, Bucket Sort).
  - **Linked Lists**: Singly (Active Config), Doubly (History & Undo/Redo), Circular (Worker round-robin & Josephus), Generalized (Nested options).
  - **Stacks & Queues**: Expression conversion (Infix → Postfix), rule evaluator, FIFO Array Queue, Circular Queue, Deque priority queue.
  - **Trees**: General Tree, Binary Tree, BST, Threaded Binary Tree, traversals (Recursive & Non-Recursive).
  - **Graphs**: Adjacency Matrix & List, BFS, DFS, Cycle Detection, Connected Components, Bipartite Graph, Dijkstra's Shortest Path, Prim/Kruskal MST, Union-Find with path compression.
  - **Hashing**: SKU Separate Chaining, Open Addressing (Linear/Quadratic), Perfect Hashing, Extendible Hashing.
  - **Sparse Matrix**: Triplet format with Simple Transpose vs Fast Transpose benchmark.
- **Enterprise SaaS Dashboard Frontend**: Built with Vanilla HTML5, CSS3, and JavaScript featuring dark mode, dynamic widgets, wizard configurator, data structure visualizer, algorithm lab, and interactive benchmark charts.
- **Windows Built-in HTTP Server**: Native Winsock2 HTTP API server (`server.c`) serving static assets and REST API endpoints.

---

## 🛠️ Project Structure

```
CONFIGCORE/
├── data/                  # Pre-loaded JSON catalog, rules, and compatibility datasets
├── docs/                  # Architectural, API, and Syllabus documentation
│   ├── architecture.md
│   ├── syllabus_mapping.md
│   ├── algorithms.md
│   ├── api.md
│   └── setup.md
├── frontend/              # Modern Enterprise SaaS Dashboard UI
│   ├── index.html
│   ├── css/style.css
│   └── js/
│       ├── api.js
│       └── app.js
├── include/               # Modular C Header files for all DS & Algorithms
├── src/                   # Pure C implementations
│   ├── main.c
│   ├── server.c
│   ├── api.c
│   ├── arrays.c, linked_list.c, doubly_list.c, circular_list.c, generalized_list.c
│   ├── stack.c, expression.c, queue.c, circular_queue.c, deque.c, josephus.c
│   ├── tree.c, tree_conversion.c, binary_tree.c, bst.c, threaded_tree.c
│   ├── graph.c, bfs.c, dfs.c, cycle_detection.c, connected_graph.c, bipartite.c
│   ├── dijkstra.c, prim.c, union_find.c, kruskal.c
│   ├── search.c, sorting.c, hashing.c, perfect_hash.c, extendible_hash.c
│   ├── sparse_matrix.c, transpose.c, ordered_list.c
│   ├── pricing.c, rules.c, configuration.c, recommendation.c, quote.c, benchmark.c
├── tests/                 # Automated C Unit Test Suite (30 test cases)
│   └── test_main.c
├── CMakeLists.txt
├── Makefile
├── README.md
└── run.bat                # Windows One-Click Build & Run script
```

---

## 💻 Quick Start & Demonstration

1. Clone or extract the repository.
2. Run `run.bat` in Windows Command Prompt or PowerShell:
   ```cmd
   .\run.bat
   ```
3. The script will compile the C executable `configcore.exe`, run all unit tests `test_runner.exe`, start the HTTP server on port 8080, and open `http://localhost:8080` in your web browser.

---

## 🎓 Academic Defense & Verification

- Every data structure declaration resides in `include/*.h` and implementation in `src/*.c`.
- **No external STL or high-level containers** are used.
- Call `/api/*` endpoints to observe live C data structure serialization in real-time.
- Check `docs/syllabus_mapping.md` for a complete mapping of syllabus topics to source code functions.
