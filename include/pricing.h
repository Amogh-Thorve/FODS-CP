#ifndef PRICING_H
#define PRICING_H

#include "linked_list.h"

typedef struct {
    double baseProductPrice;
    double optionTotalPrice;
    double requiredComponentsPrice;
    double surcharge;
    double discountAmount;
    double taxAmount;
    double finalTotalPrice;
} PriceBreakdown;

PriceBreakdown calculatePrice(ConfigLinkedList *config, double taxRate, double discountRate);
char* serializePriceJSON(PriceBreakdown pb);

#endif // PRICING_H
