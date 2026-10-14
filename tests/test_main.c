#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "arrays.h"
#include "linked_list.h"
#include "doubly_list.h"
#include "circular_list.h"
#include "josephus.h"
#include "generalized_list.h"
#include "stack.h"
#include "expression.h"
#include "queue.h"
#include "circular_queue.h"
#include "deque.h"
#include "tree.h"
#include "tree_conversion.h"
#include "binary_tree.h"
#include "bst.h"
#include "threaded_tree.h"
#include "graph.h"
#include "bfs.h"
#include "dfs.h"
#include "cycle_detection.h"
#include "connected_graph.h"
#include "bipartite.h"
#include "dijkstra.h"
#include "prim.h"
#include "union_find.h"
#include "kruskal.h"
#include "search.h"
#include "sorting.h"
#include "hashing.h"
#include "perfect_hash.h"
#include "extendible_hash.h"
#include "sparse_matrix.h"
#include "transpose.h"
#include "ordered_list.h"
#include "pricing.h"
#include "rules.h"
#include "configuration.h"

static int g_passCount = 0;

#define RUN_TEST(fn) do { \
    printf("Running %s...", #fn); \
    fn(); \
    printf(" PASSED!\n"); \
    g_passCount++; \
} while(0)

// Test cases
void test_array_operations() {
    ProductArray pa;
    initProductArray(&pa);
    assert(pa.count == 0);
    addProduct(&pa, (Product){1, "P1", "Pump", "Category", "Desc", 100.0, 5, 4.5, 1});
    assert(pa.count == 1);
    assert(findProductById(&pa, 1) != NULL);
    assert(findProductById(&pa, 99) == NULL);
}

void test_singly_linked_list() {
    ConfigLinkedList list;
    initLinkedList(&list);
    assert(countNodes(&list) == 0);
    insertFront(&list, 10, "Base Pump", "Pump", 5000.0, 0);
    insertEnd(&list, 20, "15HP Motor", "Motor", 2000.0, 1);
    assert(countNodes(&list) == 2);
    assert(searchNode(&list, 20) != NULL);
    assert(deleteByValue(&list, 20) == 1);
    assert(countNodes(&list) == 1);
    clearList(&list);
    assert(countNodes(&list) == 0);
}

void test_doubly_linked_list_history() {
    DoublyHistoryList hist;
    initHistoryList(&hist);
    insertHistory(&hist, "SELECT_PRODUCT", 101, "Pump X", 50000.0);
    insertHistory(&hist, "ADD_OPTION", 201, "5HP Motor", 62000.0);
    assert(hist.count == 2);
    assert(previousState(&hist) != NULL);
    clearHistory(&hist);
    assert(hist.count == 0);
}

void test_circular_linked_list() {
    CircularList cl;
    initCircularList(&cl);
    insertWorker(&cl, 1, "Worker 1");
    insertWorker(&cl, 2, "Worker 2");
    assert(cl.count == 2);
    CircularNode *w1 = getNextWorker(&cl);
    assert(w1->workerId == 1);
    CircularNode *w2 = getNextWorker(&cl);
    assert(w2->workerId == 2);
    CircularNode *w3 = getNextWorker(&cl);
    assert(w3->workerId == 1); // Round robin wrap around
    clearCircularList(&cl);
}

void test_josephus_problem() {
    JosephusResult jRes = solveJosephus(7, 3);
    assert(jRes.totalCount == 7);
    assert(jRes.stepSize == 3);
    assert(jRes.survivorId == 4);
}

void test_generalized_linked_list() {
    GListNode *sub = createAtomNode(1, "5 HP Motor", 12000.0);
    GListNode *cat = createListNode("Motor Options", sub);
    assert(cat != NULL);
    freeGList(cat);
}

void test_stack_array_and_list() {
    ArrayStack as;
    initArrayStack(&as);
    assert(isArrayStackEmpty(&as));
    pushArrayStack(&as, "AND");
    pushArrayStack(&as, "OR");
    assert(!isArrayStackEmpty(&as));
    char buf[64];
    popArrayStack(&as, buf);
    assert(strcmp(buf, "OR") == 0);

    ListStack ls;
    initListStack(&ls);
    assert(isListStackEmpty(&ls));
    pushListStack(&ls, "Voltage415");
    assert(!isListStackEmpty(&ls));
    popListStack(&ls, buf);
    assert(strcmp(buf, "Voltage415") == 0);
    freeListStack(&ls);
}

void test_infix_to_postfix_evaluation() {
    ExpressionResult res = infixToPostfix("1 AND 1 OR 0");
    assert(res.evaluationResult == 1);
}

void test_queue_array_and_list() {
    ArrayQueue aq;
    initArrayQueue(&aq);
    assert(isArrayQueueEmpty(&aq));
    QuoteJob j = {101, "PRICING", "Details", 1, "PENDING"};
    enqueueArrayQueue(&aq, j);
    assert(!isArrayQueueEmpty(&aq));
    QuoteJob outJ;
    dequeueArrayQueue(&aq, &outJ);
    assert(outJ.jobId == 101);
}

void test_circular_queue() {
    CircularQueue cq;
    initCircularQueue(&cq);
    assert(isCircularQueueEmpty(&cq));
    CircularTask t = {1, "Task 1", 101};
    enqueueCircularQueue(&cq, t);
    assert(!isCircularQueueEmpty(&cq));
    CircularTask outT;
    dequeueCircularQueue(&cq, &outT);
    assert(outT.taskId == 1);
}

void test_deque_operations() {
    Deque dq;
    initDeque(&dq);
    assert(isDequeEmpty(&dq));
    DequeItem item = {1, "High Priority Job"};
    insertFrontDeque(&dq, item);
    assert(!isDequeEmpty(&dq));
    DequeItem outItem;
    deleteFrontDeque(&dq, &outItem);
    assert(outItem.priority == 1);
}

void test_general_tree() {
    TreeNode *root = createTreeNode(1, "Root", "PRODUCT", 100.0);
    TreeNode *child = createTreeNode(2, "Child", "OPTION", 50.0);
    addChild(root, child);
    assert(searchTree(root, 2) != NULL);
    freeTree(root);
}

void test_general_to_binary_tree_conversion() {
    TreeNode *genRoot = createTreeNode(1, "Root", "PRODUCT", 100.0);
    addChild(genRoot, createTreeNode(2, "Child 1", "OPTION", 50.0));
    addChild(genRoot, createTreeNode(3, "Child 2", "OPTION", 60.0));
    LCRSBinaryNode *binRoot = convertGeneralToBinary(genRoot);
    assert(binRoot != NULL);
    assert(binRoot->leftChild != NULL);
    assert(binRoot->leftChild->rightSibling != NULL);
    freeTree(genRoot);
    freeLCRSTree(binRoot);
}

void test_binary_tree_traversals() {
    BinaryNode *root = createBinaryNode(1, "Root", 10.0);
    root->left = createBinaryNode(2, "L", 5.0);
    root->right = createBinaryNode(3, "R", 15.0);

    TraversalResult r1 = preorderRecursive(root);
    assert(r1.count == 3);
    TraversalResult r2 = preorderNonRecursive(root);
    assert(r2.count == 3);
    TraversalResult r3 = inorderNonRecursive(root);
    assert(r3.count == 3);
    TraversalResult r4 = levelOrderTraversal(root);
    assert(r4.count == 3);

    freeBinaryTree(root);
}

void test_bst_operations() {
    BSTNode *root = NULL;
    root = insertBST(root, 50, "P50", 100.0);
    root = insertBST(root, 30, "P30", 50.0);
    root = insertBST(root, 70, "P70", 150.0);

    assert(searchBST(root, 30) != NULL);
    assert(findMinBST(root)->productId == 30);
    assert(findMaxBST(root)->productId == 70);
    assert(getBSTHeight(root) == 2);
    assert(countBSTNodes(root) == 3);

    root = deleteBST(root, 30);
    assert(countBSTNodes(root) == 2);
    freeBST(root);
}

void test_threaded_binary_tree() {
    ThreadedNode *root = NULL;
    root = insertThreaded(root, 20, "Node 20");
    root = insertThreaded(root, 10, "Node 10");
    root = insertThreaded(root, 30, "Node 30");

    int arr[10];
    char names[10][64];
    int count = 0;
    inorderThreadedTraversal(root, arr, names, &count);
    assert(count == 3);
    assert(arr[0] == 10 && arr[1] == 20 && arr[2] == 30);
}

void test_graph_and_traversals() {
    Graph g;
    initGraph(&g);
    addVertex(&g, 1, "V1", "Cat");
    addVertex(&g, 2, "V2", "Cat");
    addEdge(&g, 1, 2, REL_COMPATIBLE, 1.0);

    BFSResult bfsRes = runBFS(&g, 1);
    assert(bfsRes.count == 2);

    DFSResult dfsRes = runDFS(&g, 1);
    assert(dfsRes.count == 2);

    freeGraph(&g);
}

void test_cycle_detection() {
    Graph g;
    initGraph(&g);
    addVertex(&g, 1, "V1", "Cat");
    addVertex(&g, 2, "V2", "Cat");
    addVertex(&g, 3, "V3", "Cat");

    addEdge(&g, 1, 2, REL_REQUIRES, 1.0);
    addEdge(&g, 2, 3, REL_REQUIRES, 1.0);
    addEdge(&g, 3, 1, REL_REQUIRES, 1.0); // Cycle!

    CycleResult res = detectCycle(&g);
    assert(res.hasCycle == true);
    freeGraph(&g);
}

void test_connected_components() {
    Graph g;
    initGraph(&g);
    addVertex(&g, 1, "V1", "Cat");
    addVertex(&g, 2, "V2", "Cat");
    addVertex(&g, 3, "V3", "Cat");

    addEdge(&g, 1, 2, REL_COMPATIBLE, 1.0);
    ComponentResult res = findConnectedComponents(&g);
    assert(res.numComponents == 2);
    freeGraph(&g);
}

void test_bipartite_graph() {
    Graph g;
    initGraph(&g);
    addVertex(&g, 1, "V1", "Cat");
    addVertex(&g, 2, "V2", "Cat");
    addEdge(&g, 1, 2, REL_COMPATIBLE, 1.0);

    BipartiteResult res = checkBipartite(&g);
    assert(res.isBipartite == true);
    freeGraph(&g);
}

void test_dijkstra_shortest_path() {
    Graph g;
    initGraph(&g);
    addVertex(&g, 1, "V1", "Cat");
    addVertex(&g, 2, "V2", "Cat");
    addVertex(&g, 3, "V3", "Cat");
    addEdge(&g, 1, 2, REL_COMPATIBLE, 2.0);
    addEdge(&g, 2, 3, REL_COMPATIBLE, 3.0);

    DijkstraResult res = runDijkstra(&g, 1, 3);
    assert(res.totalCost == 5.0);
    freeGraph(&g);
}

void test_mst_prim_and_kruskal() {
    Graph g;
    initGraph(&g);
    addVertex(&g, 1, "V1", "Cat");
    addVertex(&g, 2, "V2", "Cat");
    addEdge(&g, 1, 2, REL_COMPATIBLE, 4.0);

    MSTResult primRes = runPrim(&g);
    assert(primRes.totalWeight == 4.0);

    KruskalResult kruskRes = runKruskal(&g);
    assert(kruskRes.mst.totalWeight == 4.0);

    freeGraph(&g);
}

void test_union_find() {
    DisjointSet ds;
    makeSet(&ds, 5);
    unionSet(&ds, 0, 1);
    unionSet(&ds, 1, 2);
    assert(findSet(&ds, 0) == findSet(&ds, 2));
    assert(findSet(&ds, 0) != findSet(&ds, 4));
}

void test_searching_algorithms() {
    int arr[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
    assert(linearSearch(arr, 10, 14).foundIndex == 6);
    assert(binarySearch(arr, 10, 14).foundIndex == 6);
    assert(indexedSequentialSearch(arr, 10, 14).foundIndex == 6);
    assert(interpolationSearch(arr, 10, 14).foundIndex == 6);
    assert(fibonacciSearch(arr, 10, 14).foundIndex == 6);
}

void test_all_sorting_algorithms() {
    int arr[6] = {64, 34, 25, 12, 22, 11};
    assert(bubbleSort(arr, 6).sortedArray[0] == 11);
    assert(selectionSort(arr, 6).sortedArray[0] == 11);
    assert(insertionSort(arr, 6).sortedArray[0] == 11);
    assert(mergeSort(arr, 6).sortedArray[0] == 11);
    assert(quickSort(arr, 6).sortedArray[0] == 11);
    assert(heapSort(arr, 6).sortedArray[0] == 11);
    assert(radixSort(arr, 6).sortedArray[0] == 11);
    assert(shellSort(arr, 6).sortedArray[0] == 11);
    assert(countingSort(arr, 6).sortedArray[0] == 11);
    assert(bucketSort(arr, 6).sortedArray[0] == 11);
}

void test_hash_tables() {
    ChainingHashTable cht;
    initChainingHashTable(&cht);
    Product p = {101, "PMP-1001", "Pump X", "Pumps", "Desc", 50000.0, 10, 4.8, 1};
    insertChaining(&cht, "PMP-1001", p);
    assert(searchChaining(&cht, "PMP-1001") != NULL);
    assert(deleteChaining(&cht, "PMP-1001") == true);
    freeChainingHashTable(&cht);

    OpenAddressingHashTable oht;
    initOpenAddressingHashTable(&oht);
    insertLinearProbing(&oht, "PMP-1001", p);
    assert(searchOpenAddressing(&oht, "PMP-1001", false) != NULL);

    ExtendibleHashTable *eh = createExtendibleHash(2);
    insertExtendibleHash(eh, 5);
    assert(eh != NULL);
    freeExtendibleHash(eh);
}

void test_sparse_matrix_and_transpose() {
    SparseMatrix sm = createSparseMatrix(4, 4);
    insertSparseElement(&sm, 0, 2, 5.0);
    insertSparseElement(&sm, 1, 3, 10.0);
    assert(sm.numElements == 2);

    TransposeResult simpleRes = simpleTranspose(&sm);
    assert(simpleRes.transposedMatrix.numElements == 2);

    TransposeResult fastRes = fastTranspose(&sm);
    assert(fastRes.transposedMatrix.numElements == 2);
}

void test_ordered_list() {
    OrderedList ol;
    initOrderedList(&ol);
    insertSorted(&ol, 101, "Item A", 500.0);
    insertSorted(&ol, 102, "Item B", 200.0);
    assert(ol.head->id == 102); // Lowest key first
    freeOrderedList(&ol);
}

void test_cpq_pricing_and_rules() {
    CPQEngine engine;
    initCPQEngine(&engine);
    addProduct(&engine.products, (Product){101, "PMP-1001", "Pump X", "Pumps", "Desc", 50000.0, 10, 4.8, 1});
    addOption(&engine.options, (Option){203, "15 HP Motor", "Motor", 25000.0, 104});
    addOption(&engine.options, (Option){204, "230 V Power", "Voltage", 3000.0, 109});

    selectProduct(&engine, 101);
    selectOption(&engine, 203);
    selectOption(&engine, 204);

    ValidationResult vr = validateCurrentConfiguration(&engine);
    assert(vr.isValid == false); // 15HP + 230V is invalid!
}

int main() {
    printf("=======================================================\n");
    printf("        CONFIGCORE Automated Unit Test Suite          \n");
    printf("=======================================================\n");

    RUN_TEST(test_array_operations);
    RUN_TEST(test_singly_linked_list);
    RUN_TEST(test_doubly_linked_list_history);
    RUN_TEST(test_circular_linked_list);
    RUN_TEST(test_josephus_problem);
    RUN_TEST(test_generalized_linked_list);
    RUN_TEST(test_stack_array_and_list);
    RUN_TEST(test_infix_to_postfix_evaluation);
    RUN_TEST(test_queue_array_and_list);
    RUN_TEST(test_circular_queue);
    RUN_TEST(test_deque_operations);
    RUN_TEST(test_general_tree);
    RUN_TEST(test_general_to_binary_tree_conversion);
    RUN_TEST(test_binary_tree_traversals);
    RUN_TEST(test_bst_operations);
    RUN_TEST(test_threaded_binary_tree);
    RUN_TEST(test_graph_and_traversals);
    RUN_TEST(test_cycle_detection);
    RUN_TEST(test_connected_components);
    RUN_TEST(test_bipartite_graph);
    RUN_TEST(test_dijkstra_shortest_path);
    RUN_TEST(test_mst_prim_and_kruskal);
    RUN_TEST(test_union_find);
    RUN_TEST(test_searching_algorithms);
    RUN_TEST(test_all_sorting_algorithms);
    RUN_TEST(test_hash_tables);
    RUN_TEST(test_sparse_matrix_and_transpose);
    RUN_TEST(test_ordered_list);
    RUN_TEST(test_cpq_pricing_and_rules);

    printf("\n[TEST SUMMARY] All %d unit tests executed successfully!\n", g_passCount);
    return 0;
}
