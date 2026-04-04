#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

extern const char* LOG_FILE_NAME;

void lprintf(const char* text, ...);

#ifdef DEBUG

    #define LPRINTF(text, ...) lprintf("[DEBUG] %s:%d:%s(): " text "\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#else
    #define LPRINTF(text, ...) ((void)0)
#endif

#define $ LPRINTF("MEOW");

#endif /* DEBUG_H */