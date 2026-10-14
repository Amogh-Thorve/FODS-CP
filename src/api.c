#include "api.h"
#include "json.h"
#include "expression.h"
#include "search.h"
#include "sorting.h"
#include "hashing.h"
#include "perfect_hash.h"
#include "extendible_hash.h"
#include "tree.h"
#include "tree_conversion.h"
#include "binary_tree.h"
#include "bst.h"
#include "threaded_tree.h"
#include "bfs.h"
#include "dfs.h"
#include "cycle_detection.h"
#include "connected_graph.h"
#include "bipartite.h"
#include "dijkstra.h"
#include "prim.h"
#include "kruskal.h"
#include "sparse_matrix.h"
#include "transpose.h"
#include "ordered_list.h"
#include "josephus.h"
#include "circular_queue.h"
#include "deque.h"
#include "generalized_list.h"
#include "benchmark.h"
#include "recommendation.h"

static CPQEngine *g_engine = NULL;
static QuoteEngine *g_quoteEng = NULL;
static ChainingHashTable g_skuHash;

void initAPI(CPQEngine *engine, QuoteEngine *quoteEng) {
    g_engine = engine;
    g_quoteEng = quoteEng;

    // Load Hash Table for SKU Lookups
    initChainingHashTable(&g_skuHash);
    for (int i = 0; i < engine->products.count; i++) {
        insertChaining(&g_skuHash, engine->products.items[i].sku, engine->products.items[i]);
    }
}

char* handleAPIRequest(const char *method, const char *path, const char *body) {
    static char responseBuf[32768];
    responseBuf[0] = '\0';

    if (!g_engine) {
        strcpy(responseBuf, "{\"error\":\"CPQ Engine not initialized\"}");
        return responseBuf;
    }

    // 1. GET /api/products
    if (strcmp(method, "GET") == 0 && strcmp(path, "/api/products") == 0) {
        strcat(responseBuf, "[");
        for (int i = 0; i < g_engine->products.count; i++) {
            char pBuf[512];
            Product *p = &g_engine->products.items[i];
            snprintf(pBuf, sizeof(pBuf),
                     "{\"productId\":%d,\"sku\":\"%s\",\"name\":\"%s\",\"category\":\"%s\",\"description\":\"%s\",\"basePrice\":%.2f,\"availability\":%d,\"rating\":%.1f}",
                     p->productId, p->sku, p->name, p->category, p->description, p->basePrice, p->availability, p->rating);
            strcat(responseBuf, pBuf);
            if (i < g_engine->products.count - 1) strcat(responseBuf, ",");
        }
        strcat(responseBuf, "]");
        return responseBuf;
    }

    // 2. GET /api/options
    if (strcmp(method, "GET") == 0 && strcmp(path, "/api/options") == 0) {
        strcat(responseBuf, "[");
        for (int i = 0; i < g_engine->options.count; i++) {
            char oBuf[256];
            Option *o = &g_engine->options.items[i];
            snprintf(oBuf, sizeof(oBuf),
                     "{\"optionId\":%d,\"name\":\"%s\",\"category\":\"%s\",\"price\":%.2f,\"productId\":%d}",
                     o->optionId, o->name, o->category, o->price, o->productId);
            strcat(responseBuf, oBuf);
            if (i < g_engine->options.count - 1) strcat(responseBuf, ",");
        }
        strcat(responseBuf, "]");
        return responseBuf;
    }

    // 3. GET /api/configuration
    if (strcmp(method, "GET") == 0 && strcmp(path, "/api/configuration") == 0) {
        strcpy(responseBuf, serializeLinkedListJSON(&g_engine->currentConfig));
        return responseBuf;
    }

    // 4. POST /api/configuration/select
    if (strcmp(method, "POST") == 0 && strcmp(path, "/api/configuration/select") == 0) {
        int productId = jsonGetInt(body, "productId", -1);
        int optionId = jsonGetInt(body, "optionId", -1);
        bool success = false;
        if (productId != -1) {
            success = selectProduct(g_engine, productId);
        } else if (optionId != -1) {
            success = selectOption(g_engine, optionId);
        }
        ValidationResult vr = validateCurrentConfiguration(g_engine);
        snprintf(responseBuf, sizeof(responseBuf),
                 "{\"success\":%s,\"validation\":%s}",
                 success ? "true" : "false", serializeValidationJSON(vr));
        return responseBuf;
    }

    // 5. POST /api/configuration/remove
    if (strcmp(method, "POST") == 0 && strcmp(path, "/api/configuration/remove") == 0) {
        int optionId = jsonGetInt(body, "optionId", -1);
        bool success = removeOption(g_engine, optionId);
        ValidationResult vr = validateCurrentConfiguration(g_engine);
        snprintf(responseBuf, sizeof(responseBuf),
                 "{\"success\":%s,\"validation\":%s}",
                 success ? "true" : "false", serializeValidationJSON(vr));
        return responseBuf;
    }

    // 6. POST /api/configuration/undo
    if (strcmp(method, "POST") == 0 && strcmp(path, "/api/configuration/undo") == 0) {
        bool success = undoConfiguration(g_engine);
        strcpy(responseBuf, serializeHistoryJSON(&g_engine->history));
        return responseBuf;
    }

    // 7. POST /api/configuration/redo
    if (strcmp(method, "POST") == 0 && strcmp(path, "/api/configuration/redo") == 0) {
        bool success = redoConfiguration(g_engine);
        strcpy(responseBuf, serializeHistoryJSON(&g_engine->history));
        return responseBuf;
    }

    // 8. POST /api/configuration/validate
    if (strcmp(method, "POST") == 0 && strcmp(path, "/api/configuration/validate") == 0) {
        ValidationResult vr = validateCurrentConfiguration(g_engine);
        strcpy(responseBuf, serializeValidationJSON(vr));
        return responseBuf;
    }

    // 9. GET /api/configuration/price
    if (strcmp(method, "GET") == 0 && strcmp(path, "/api/configuration/price") == 0) {
        PriceBreakdown pb = calculatePrice(&g_engine->currentConfig, g_engine->taxRate, g_engine->discountRate);
        strcpy(responseBuf, serializePriceJSON(pb));
        return responseBuf;
    }

    // 10. GET /api/recommendations
    if (strcmp(method, "GET") == 0 && strcmp(path, "/api/recommendations") == 0) {
        RecommendationResult recs = getRecommendations(g_engine, "Quick Sort");
        strcpy(responseBuf, serializeRecommendationsJSON(recs));
        return responseBuf;
    }

    // 11. GET /api/compatibility
    if (strcmp(method, "GET") == 0 && strcmp(path, "/api/compatibility") == 0) {
        strcpy(responseBuf, serializeGraphJSON(&g_engine->compatibilityGraph));
        return responseBuf;
    }

    // 12. GET /api/rules
    if (strcmp(method, "GET") == 0 && strcmp(path, "/api/rules") == 0) {
        strcpy(responseBuf, serializeRuleSetJSON(&g_engine->ruleSet));
        return responseBuf;
    }

    // 13. POST /api/rules/evaluate
    if (strcmp(method, "POST") == 0 && strcmp(path, "/api/rules/evaluate") == 0) {
        char expression[128] = "Motor15HP AND Voltage415";
        jsonGetString(body, "expression", expression, sizeof(expression));
        ExpressionResult exprRes = infixToPostfix(expression);
        strcpy(responseBuf, serializeExpressionResultJSON(exprRes));
        return responseBuf;
    }

    // 14. GET /api/history
    if (strcmp(method, "GET") == 0 && strcmp(path, "/api/history") == 0) {
        strcpy(responseBuf, serializeHistoryJSON(&g_engine->history));
        return responseBuf;
    }

    // 15. GET /api/queue
    if (strcmp(method, "GET") == 0 && strcmp(path, "/api/queue") == 0) {
        strcpy(responseBuf, serializeQueueStatusJSON(g_quoteEng));
        return responseBuf;
    }

    // 16. POST /api/queue
    if (strcmp(method, "POST") == 0 && strcmp(path, "/api/queue") == 0) {
        char clientName[64] = "ACME CPQ Client";
        jsonGetString(body, "customerName", clientName, sizeof(clientName));
        int qNum = createQuoteJob(g_quoteEng, g_engine, clientName);
        QuoteOutput *q = getQuoteByNumber(g_quoteEng, qNum);
        strcpy(responseBuf, serializeQuoteJSON(q));
        return responseBuf;
    }

    // 17. GET /api/tree
    if (strcmp(method, "GET") == 0 && strcmp(path, "/api/tree") == 0) {
        TreeNode *root = createTreeNode(101, "Industrial Pump X", "PRODUCT", 50000.0);
        TreeNode *cMotor = createTreeNode(200, "Motor Options", "CATEGORY", 0.0);
        TreeNode *cVolt = createTreeNode(201, "Voltage Options", "CATEGORY", 0.0);
        TreeNode *cMat = createTreeNode(202, "Material Options", "CATEGORY", 0.0);
        addChild(root, cMotor);
        addChild(root, cVolt);
        addChild(root, cMat);

        addChild(cMotor, createTreeNode(201, "5 HP Motor", "OPTION", 12000.0));
        addChild(cMotor, createTreeNode(202, "10 HP Motor", "OPTION", 18000.0));
        addChild(cMotor, createTreeNode(203, "15 HP Motor", "OPTION", 25000.0));

        addChild(cVolt, createTreeNode(204, "230 V Power", "OPTION", 3000.0));
        addChild(cVolt, createTreeNode(205, "415 V Power", "OPTION", 6000.0));

        addChild(cMat, createTreeNode(206, "Cast Iron Body", "OPTION", 4000.0));
        addChild(cMat, createTreeNode(207, "Stainless Steel Body", "OPTION", 12000.0));

        strcpy(responseBuf, serializeGeneralTreeJSON(root));
        freeTree(root);
        return responseBuf;
    }

    // 18. GET /api/graph
    if (strcmp(method, "GET") == 0 && strcmp(path, "/api/graph") == 0) {
        strcpy(responseBuf, serializeGraphJSON(&g_engine->compatibilityGraph));
        return responseBuf;
    }

    // 19. GET /api/hash-table
    if (strcmp(method, "GET") == 0 && strcmp(path, "/api/hash-table") == 0) {
        strcpy(responseBuf, serializeChainingHashTableJSON(&g_skuHash));
        return responseBuf;
    }

    // 20. GET /api/sparse-matrix
    if (strcmp(method, "GET") == 0 && strcmp(path, "/api/sparse-matrix") == 0) {
        SparseMatrix sm = createSparseMatrix(10, 10);
        insertSparseElement(&sm, 0, 2, 1.0);
        insertSparseElement(&sm, 0, 3, 2.0);
        insertSparseElement(&sm, 1, 5, 1.5);
        insertSparseElement(&sm, 2, 7, 3.0);
        insertSparseElement(&sm, 4, 1, 0.8);
        strcpy(responseBuf, serializeSparseMatrixJSON(&sm));
        return responseBuf;
    }

    // 21. GET /api/josephus
    if (strcmp(method, "GET") == 0 && strcmp(path, "/api/josephus") == 0) {
        int n = jsonGetInt(body, "n", 7);
        int k = jsonGetInt(body, "k", 3);
        JosephusResult jRes = solveJosephus(n, k);
        strcpy(responseBuf, serializeJosephusJSON(jRes));
        return responseBuf;
    }

    // 22. POST /api/benchmark/sort
    if (strcmp(method, "POST") == 0 && strcmp(path, "/api/benchmark/sort") == 0) {
        int size = jsonGetInt(body, "size", 1000);
        BenchmarkSuite suite = runFullBenchmark(size);
        strcpy(responseBuf, serializeBenchmarkJSON(suite));
        return responseBuf;
    }

    // Default 404
    strcpy(responseBuf, "{\"error\":\"API Endpoint not found\"}");
    return responseBuf;
}
