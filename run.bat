@echo off
setlocal enableextensions
title CONFIGCORE CPQ Engine Launcher

echo =======================================================
echo          CONFIGCORE - CPQ Engine Build and Run          
echo =======================================================

echo [1/3] Compiling C Backend (configcore.exe)...
gcc -Iinclude src/arrays.c src/linked_list.c src/doubly_list.c src/circular_list.c src/josephus.c src/generalized_list.c src/stack.c src/expression.c src/queue.c src/circular_queue.c src/deque.c src/tree.c src/tree_conversion.c src/binary_tree.c src/bst.c src/threaded_tree.c src/graph.c src/bfs.c src/dfs.c src/cycle_detection.c src/connected_graph.c src/bipartite.c src/dijkstra.c src/prim.c src/union_find.c src/kruskal.c src/search.c src/sorting.c src/hashing.c src/perfect_hash.c src/extendible_hash.c src/sparse_matrix.c src/transpose.c src/ordered_list.c src/pricing.c src/rules.c src/configuration.c src/recommendation.c src/quote.c src/benchmark.c src/json.c src/api.c src/server.c src/main.c -lws2_32 -o configcore.exe

if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Compilation of configcore.exe failed!
    pause
    exit /b %errorlevel%
)

echo [2/3] Compiling and running automated unit tests...
gcc -Iinclude src/arrays.c src/linked_list.c src/doubly_list.c src/circular_list.c src/josephus.c src/generalized_list.c src/stack.c src/expression.c src/queue.c src/circular_queue.c src/deque.c src/tree.c src/tree_conversion.c src/binary_tree.c src/bst.c src/threaded_tree.c src/graph.c src/bfs.c src/dfs.c src/cycle_detection.c src/connected_graph.c src/bipartite.c src/dijkstra.c src/prim.c src/union_find.c src/kruskal.c src/search.c src/sorting.c src/hashing.c src/perfect_hash.c src/extendible_hash.c src/sparse_matrix.c src/transpose.c src/ordered_list.c src/pricing.c src/rules.c src/configuration.c src/recommendation.c src/quote.c src/benchmark.c src/json.c src/api.c tests/test_main.c -o test_runner.exe

if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Compilation of test_runner.exe failed!
    pause
    exit /b %errorlevel%
)

.\test_runner.exe

if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Unit tests failed!
    pause
    exit /b %errorlevel%
)

echo.
echo [3/3] Launching web browser at http://localhost:8080...
start "" "http://localhost:8080"

echo.
echo Starting CONFIGCORE C HTTP Server...
.\configcore.exe 8080

pause
