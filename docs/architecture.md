# CONFIGCORE Architecture Document

## System Architecture

CONFIGCORE is a product-quality, academic-grade Product Configuration Engine for CPQ (Configure, Price, Quote) built with a pure C core and a modern web front end.

```
+-------------------------------------------------------+
|                    Browser Frontend                   |
| (HTML5 / Vanilla CSS3 / JavaScript SaaS Dashboard UI) |
+-------------------------------------------------------+
                           │
                 HTTP REST API (Port 8080)
                           │
+-------------------------------------------------------+
|                   C Winsock HTTP Server               |
|                      (server.c / api.c)               |
+-------------------------------------------------------+
                           │
+-------------------------------------------------------+
|                  CPQ Core Engine (C11)                |
|  - Pricing Formula Engine      - Rule Stack Evaluator |
|  - Recommendation Ranker       - Quote Job Queue      |
+-------------------------------------------------------+
                           │
+-------------------------------------------------------+
|                 Data Structures Layer (C)             |
|  - Singly/Doubly/Circular/Generalized Linked Lists    |
|  - Array & List Stacks / Queues / Deques              |
|  - General Trees, Binary Trees, BSTs, Threaded Trees   |
|  - Adjacency Matrix & List Graphs                     |
|  - Hash Tables (Chaining, Open Addr, Perfect, Ext)    |
|  - Sparse Matrix & Transpositions (Simple/Fast)       |
|  - 5 Searching & 10 Manual Sorting Algorithms         |
+-------------------------------------------------------+
```

## Key Architectural Principles
1. **Zero Simulation**: Every CPQ decision (validation, pricing, candidate ranking, dependency checking) executes inside C Data Structure memory.
2. **Modular C Architecture**: Headers (`include/*.h`) and implementations (`src/*.c`) strictly isolate domain logic from raw data structures.
3. **Defensive Dynamic Memory**: Explicit `malloc`, `realloc`, and `free` routines prevent memory leaks and dangling pointers.
