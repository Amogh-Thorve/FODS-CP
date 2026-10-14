#include "arrays.h"

void initProductArray(ProductArray *pa) {
    if (!pa) return;
    pa->count = 0;
}

void addProduct(ProductArray *pa, Product p) {
    if (!pa || pa->count >= MAX_PRODUCTS) return;
    pa->items[pa->count++] = p;
}

Product* findProductById(ProductArray *pa, int productId) {
    if (!pa) return NULL;
    for (int i = 0; i < pa->count; i++) {
        if (pa->items[i].productId == productId) {
            return &pa->items[i];
        }
    }
    return NULL;
}

Product* findProductBySKU(ProductArray *pa, const char *sku) {
    if (!pa || !sku) return NULL;
    for (int i = 0; i < pa->count; i++) {
        if (strcmp(pa->items[i].sku, sku) == 0) {
            return &pa->items[i];
        }
    }
    return NULL;
}

void initOptionArray(OptionArray *oa) {
    if (!oa) return;
    oa->count = 0;
}

void addOption(OptionArray *oa, Option opt) {
    if (!oa || oa->count >= MAX_OPTIONS) return;
    oa->items[oa->count++] = opt;
}

Option* findOptionById(OptionArray *oa, int optionId) {
    if (!oa) return NULL;
    for (int i = 0; i < oa->count; i++) {
        if (oa->items[i].optionId == optionId) {
            return &oa->items[i];
        }
    }
    return NULL;
}
