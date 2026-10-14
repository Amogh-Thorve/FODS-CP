# CONFIGCORE Algorithms & Complexity Documentation

## 1. Searching Algorithms
- **Linear Search**: $O(n)$ time, $O(1)$ space. Scans elements sequentially.
- **Binary Search**: $O(\log n)$ time, $O(1)$ space. Requires sorted input array.
- **Indexed Sequential Search**: $O(\sqrt{n})$ time. Uses block indexing to skip ranges.
- **Interpolation Search**: $O(\log \log n)$ average, $O(n)$ worst. Estimates target position.
- **Fibonacci Search**: $O(\log n)$ time. Uses Fibonacci numbers to divide array ranges.

## 2. Sorting Algorithms (10 Implemented in Pure C)
- **Bubble Sort**: $O(n^2)$ time, $O(1)$ space.
- **Selection Sort**: $O(n^2)$ time, $O(1)$ space.
- **Insertion Sort**: $O(n^2)$ time, $O(1)$ space.
- **Merge Sort**: $O(n \log n)$ time, $O(n)$ space. Stable divide-and-conquer.
- **Quick Sort**: $O(n \log n)$ average, $O(n^2)$ worst time.
- **Heap Sort**: $O(n \log n)$ time, $O(1)$ space. Uses max-heap structure.
- **Radix Sort**: $O(d \cdot (n + k))$ time. Non-comparative digit sorting.
- **Shell Sort**: $O(n^{1.3})$ time. Gap-based insertion sort.
- **Counting Sort**: $O(n + k)$ time. Frequency array based sorting.
- **Bucket Sort**: $O(n + k)$ average time.

## 3. Graph Algorithms
- **Dijkstra's Algorithm**: $O(V^2)$ shortest path for non-negative weighted graphs.
- **Prim's MST**: $O(V^2)$ minimum spanning tree.
- **Kruskal's MST**: $O(E \log E)$ minimum spanning tree using Union-Find.
- **Cycle Detection**: $O(V + E)$ directed graph DFS recursion stack check.
