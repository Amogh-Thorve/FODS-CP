# CONFIGCORE REST API Reference

All requests and responses use JSON. Backend server runs by default on `http://localhost:8080`.

| Method | Endpoint | Description |
|--------|----------|-------------|
| `GET` | `/api/products` | Returns array of all products from C ProductArray. |
| `GET` | `/api/options` | Returns array of all product options. |
| `GET` | `/api/configuration` | Returns active build components from C Singly Linked List. |
| `POST` | `/api/configuration/select` | Adds product (`productId`) or option (`optionId`) to C config list. |
| `POST` | `/api/configuration/remove` | Removes option (`optionId`) from C config list. |
| `POST` | `/api/configuration/undo` | Rolls back C history to previous Doubly Linked List state. |
| `POST` | `/api/configuration/redo` | Advances C history to next Doubly Linked List state. |
| `POST` | `/api/configuration/validate` | Evaluates C rules and graph compatibility, returning JSON status. |
| `GET` | `/api/configuration/price` | Calculates exact base, tax, discount, total price in C. |
| `GET` | `/api/recommendations` | Returns candidates sorted by C sorting algorithms. |
| `GET` | `/api/compatibility` | Returns graph vertices and edges for rendering. |
| `GET` | `/api/rules` | Returns active rule definitions. |
| `POST` | `/api/rules/evaluate` | Evaluates infix rule expression using C Stack converter. |
| `GET` | `/api/history` | Returns timeline from C Doubly Linked List. |
| `GET` | `/api/queue` | Returns pending quote jobs from C ArrayQueue. |
| `POST` | `/api/queue` | Enqueues quote generation job and returns generated quote. |
| `GET` | `/api/tree` | Returns General Product Hierarchy tree. |
| `GET` | `/api/hash-table` | Returns SKU Separate Chaining Hash Table. |
| `GET` | `/api/sparse-matrix` | Returns compatibility sparse matrix in triplet form. |
| `POST` | `/api/benchmark/sort` | Runs full benchmark across dataset size $N$ in C. |
