#ifndef RECOMMENDATION_H
#define RECOMMENDATION_H

#include "configuration.h"
#include "sorting.h"

typedef struct {
    int productId;
    char name[100];
    double price;
    double compatibilityScore;
    double rating;
    double score; // Overall score calculated by engine
} RecommendationCandidate;

typedef struct {
    RecommendationCandidate candidates[20];
    int count;
    char sortingAlgorithmUsed[32];
} RecommendationResult;

RecommendationResult getRecommendations(CPQEngine *engine, const char *sortAlgoName);
char* serializeRecommendationsJSON(RecommendationResult recs);

#endif // RECOMMENDATION_H
