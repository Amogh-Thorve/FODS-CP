#ifndef API_H
#define API_H

#include "configuration.h"
#include "quote.h"

void initAPI(CPQEngine *engine, QuoteEngine *quoteEng);
char* handleAPIRequest(const char *method, const char *path, const char *body);

#endif // API_H
