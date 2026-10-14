#include "quote.h"
#include <time.h>

static int g_quoteNumberCounter = 1001;

void initQuoteEngine(QuoteEngine *qe) {
    if (!qe) return;
    initArrayQueue(&qe->jobQueue);
    qe->quoteCount = 0;
}

int createQuoteJob(QuoteEngine *qe, CPQEngine *engine, const char *customerName) {
    if (!qe || !engine) return -1;
    QuoteJob job;
    job.jobId = g_quoteNumberCounter;
    strcpy(job.jobType, "QUOTE_GENERATION");
    snprintf(job.details, sizeof(job.details), "Customer: %s", customerName ? customerName : "Generic Client");
    job.configId = engine->selectedProductId;
    strcpy(job.status, "PENDING");

    enqueueArrayQueue(&qe->jobQueue, job);

    // Immediately generate quote record
    QuoteOutput *q = &qe->generatedQuotes[qe->quoteCount++];
    q->quoteNumber = g_quoteNumberCounter++;
    
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    if (timeinfo) strftime(q->date, sizeof(q->date), "%Y-%m-%d", timeinfo);
    else strcpy(q->date, "2026-10-07");

    strncpy(q->customerName, customerName ? customerName : "ACME Corporation", sizeof(q->customerName) - 1);
    strcpy(q->validity, "30 Days");
    strcpy(q->status, "GENERATED");

    ValidationResult vr = validateCurrentConfiguration(engine);
    PriceBreakdown pb = vr.pricing;

    q->basePrice = pb.baseProductPrice;
    q->optionTotal = pb.optionTotalPrice;
    q->tax = pb.taxAmount;
    q->discount = pb.discountAmount;
    q->finalPrice = pb.finalTotalPrice;
    q->optionCount = 0;

    ConfigNode *curr = engine->currentConfig.head;
    while (curr) {
        if (!curr->isOption) {
            strncpy(q->productName, curr->name, sizeof(q->productName) - 1);
        } else if (q->optionCount < 10) {
            strncpy(q->optionNames[q->optionCount++], curr->name, 100);
        }
        curr = curr->next;
    }

    return q->quoteNumber;
}

bool processNextQuoteJob(QuoteEngine *qe, CPQEngine *engine) {
    if (!qe || isArrayQueueEmpty(&qe->jobQueue)) return false;
    QuoteJob job;
    dequeueArrayQueue(&qe->jobQueue, &job);
    return true;
}

QuoteOutput* getQuoteByNumber(QuoteEngine *qe, int quoteNumber) {
    if (!qe) return NULL;
    for (int i = 0; i < qe->quoteCount; i++) {
        if (qe->generatedQuotes[i].quoteNumber == quoteNumber) {
            return &qe->generatedQuotes[i];
        }
    }
    return NULL;
}

char* serializeQuoteJSON(QuoteOutput *q) {
    static char jsonBuf[4096];
    jsonBuf[0] = '\0';
    if (!q) return "{}";

    snprintf(jsonBuf, sizeof(jsonBuf),
             "{\"quoteNumber\":%d,\"date\":\"%s\",\"customerName\":\"%s\",\"productName\":\"%s\",\"basePrice\":%.2f,\"optionTotal\":%.2f,\"tax\":%.2f,\"discount\":%.2f,\"finalPrice\":%.2f,\"validity\":\"%s\",\"status\":\"%s\",\"options\":[",
             q->quoteNumber, q->date, q->customerName, q->productName, q->basePrice, q->optionTotal, q->tax, q->discount, q->finalPrice, q->validity, q->status);

    for (int i = 0; i < q->optionCount; i++) {
        char oBuf[128];
        snprintf(oBuf, sizeof(oBuf), "\"%s\"", q->optionNames[i]);
        strcat(jsonBuf, oBuf);
        if (i < q->optionCount - 1) strcat(jsonBuf, ",");
    }
    strcat(jsonBuf, "]}");
    return jsonBuf;
}

char* serializeQueueStatusJSON(QuoteEngine *qe) {
    static char jsonBuf[4096];
    jsonBuf[0] = '\0';
    if (!qe) return "{}";

    snprintf(jsonBuf, sizeof(jsonBuf),
             "{\"pendingCount\":%d,\"frontIndex\":%d,\"rearIndex\":%d,\"jobs\":[",
             qe->jobQueue.count, qe->jobQueue.front, qe->jobQueue.rear);

    if (qe->jobQueue.count > 0) {
        int idx = qe->jobQueue.front;
        for (int i = 0; i < qe->jobQueue.count; i++) {
            char jBuf[256];
            QuoteJob *j = &qe->jobQueue.jobs[idx];
            snprintf(jBuf, sizeof(jBuf),
                     "{\"jobId\":%d,\"jobType\":\"%s\",\"details\":\"%s\",\"status\":\"%s\"}",
                     j->jobId, j->jobType, j->details, j->status);
            strcat(jsonBuf, jBuf);
            if (i < qe->jobQueue.count - 1) strcat(jsonBuf, ",");
            idx = (idx + 1) % MAX_QUEUE_SIZE;
        }
    }
    strcat(jsonBuf, "]}");
    return jsonBuf;
}
