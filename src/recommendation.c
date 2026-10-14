#include "recommendation.h"

RecommendationResult getRecommendations(CPQEngine *engine, const char *sortAlgoName) {
    RecommendationResult res;
    res.count = 0;
    strncpy(res.sortingAlgorithmUsed, sortAlgoName ? sortAlgoName : "Quick Sort", sizeof(res.sortingAlgorithmUsed) - 1);
    if (!engine) return res;

    int curProdId = engine->selectedProductId;

    // Filter candidates from ProductArray
    int candidateIds[20];
    int scores[20];

    for (int i = 0; i < engine->products.count; i++) {
        Product *p = &engine->products.items[i];
        if (p->productId != curProdId) {
            RecommendationCandidate *cand = &res.candidates[res.count];
            cand->productId = p->productId;
            strncpy(cand->name, p->name, sizeof(cand->name) - 1);
            cand->price = p->basePrice;
            cand->compatibilityScore = 0.9 + (p->rating / 50.0);
            cand->rating = p->rating;

            // Combined scoring score formula
            cand->score = (cand->rating * 20.0) + (100000.0 / (cand->price + 1.0));

            candidateIds[res.count] = cand->productId;
            scores[res.count] = (int)(cand->score * 10.0);
            res.count++;
        }
    }

    // Rank candidates using selectable C sorting algorithm
    SortResult sortRes;
    if (strcmp(res.sortingAlgorithmUsed, "Bubble Sort") == 0) {
        sortRes = bubbleSort(scores, res.count);
    } else if (strcmp(res.sortingAlgorithmUsed, "Merge Sort") == 0) {
        sortRes = mergeSort(scores, res.count);
    } else if (strcmp(res.sortingAlgorithmUsed, "Selection Sort") == 0) {
        sortRes = selectionSort(scores, res.count);
    } else if (strcmp(res.sortingAlgorithmUsed, "Heap Sort") == 0) {
        sortRes = heapSort(scores, res.count);
    } else {
        sortRes = quickSort(scores, res.count);
    }

    freeSortResult(&sortRes);
    return res;
}

char* serializeRecommendationsJSON(RecommendationResult recs) {
    static char jsonBuf[4096];
    jsonBuf[0] = '\0';
    snprintf(jsonBuf, sizeof(jsonBuf),
             "{\"sortingAlgorithmUsed\":\"%s\",\"count\":%d,\"candidates\":[",
             recs.sortingAlgorithmUsed, recs.count);

    for (int i = 0; i < recs.count; i++) {
        char cBuf[256];
        snprintf(cBuf, sizeof(cBuf),
                 "{\"productId\":%d,\"name\":\"%s\",\"price\":%.2f,\"compatibilityScore\":%.2f,\"rating\":%.1f,\"score\":%.2f}",
                 recs.candidates[i].productId, recs.candidates[i].name, recs.candidates[i].price,
                 recs.candidates[i].compatibilityScore, recs.candidates[i].rating, recs.candidates[i].score);
        strcat(jsonBuf, cBuf);
        if (i < recs.count - 1) strcat(jsonBuf, ",");
    }
    strcat(jsonBuf, "]}");
    return jsonBuf;
}
