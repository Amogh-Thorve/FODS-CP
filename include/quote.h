#ifndef QUOTE_H
#define QUOTE_H

#include "configuration.h"
#include "queue.h"

typedef struct {
    int quoteNumber;
    char date[32];
    char customerName[100];
    char productName[100];
    int optionCount;
    char optionNames[10][100];
    double basePrice;
    double optionTotal;
    double tax;
    double discount;
    double finalPrice;
    char validity[32];
    char status[32];
} QuoteOutput;

typedef struct {
    ArrayQueue jobQueue;
    QuoteOutput generatedQuotes[50];
    int quoteCount;
} QuoteEngine;

void initQuoteEngine(QuoteEngine *qe);
int createQuoteJob(QuoteEngine *qe, CPQEngine *engine, const char *customerName);
bool processNextQuoteJob(QuoteEngine *qe, CPQEngine *engine);
QuoteOutput* getQuoteByNumber(QuoteEngine *qe, int quoteNumber);
char* serializeQuoteJSON(QuoteOutput *quote);
char* serializeQueueStatusJSON(QuoteEngine *qe);

#endif // QUOTE_H
