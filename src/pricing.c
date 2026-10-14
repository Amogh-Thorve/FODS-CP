#include "pricing.h"

PriceBreakdown calculatePrice(ConfigLinkedList *config, double taxRate, double discountRate) {
    PriceBreakdown pb;
    memset(&pb, 0, sizeof(pb));

    if (!config) return pb;

    ConfigNode *curr = config->head;
    while (curr) {
        if (!curr->isOption) {
            pb.baseProductPrice += curr->price;
        } else {
            pb.optionTotalPrice += curr->price;
        }
        curr = curr->next;
    }

    double subtotal = pb.baseProductPrice + pb.optionTotalPrice + pb.requiredComponentsPrice + pb.surcharge;
    pb.discountAmount = subtotal * discountRate;
    double taxable = subtotal - pb.discountAmount;
    pb.taxAmount = taxable * taxRate;
    pb.finalTotalPrice = taxable + pb.taxAmount;

    return pb;
}

char* serializePriceJSON(PriceBreakdown pb) {
    static char jsonBuf[1024];
    snprintf(jsonBuf, sizeof(jsonBuf),
             "{\"baseProductPrice\":%.2f,\"optionTotalPrice\":%.2f,\"requiredComponentsPrice\":%.2f,\"surcharge\":%.2f,\"discountAmount\":%.2f,\"taxAmount\":%.2f,\"finalTotalPrice\":%.2f}",
             pb.baseProductPrice, pb.optionTotalPrice, pb.requiredComponentsPrice, pb.surcharge, pb.discountAmount, pb.taxAmount, pb.finalTotalPrice);
    return jsonBuf;
}
