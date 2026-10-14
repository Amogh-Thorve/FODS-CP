#ifndef ARRAYS_H
#define ARRAYS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUCTS 100
#define MAX_OPTIONS 200

typedef struct {
    int productId;
    char sku[32];
    char name[100];
    char category[50];
    char description[256];
    double basePrice;
    int availability;
    double rating;
    int priority;
} Product;

typedef struct {
    int optionId;
    char name[100];
    char category[50];
    double price;
    int productId; // Associated base product ID
} Option;

typedef struct {
    Product items[MAX_PRODUCTS];
    int count;
} ProductArray;

typedef struct {
    Option items[MAX_OPTIONS];
    int count;
} OptionArray;

void initProductArray(ProductArray *pa);
void addProduct(ProductArray *pa, Product p);
Product* findProductById(ProductArray *pa, int productId);
Product* findProductBySKU(ProductArray *pa, const char *sku);

void initOptionArray(OptionArray *oa);
void addOption(OptionArray *oa, Option opt);
Option* findOptionById(OptionArray *oa, int optionId);

#endif // ARRAYS_H
