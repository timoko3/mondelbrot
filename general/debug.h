#include <stdio.h>

void lprintf(const char* text, ...);

#ifdef DEBUG

    #define LPRINTF(text, ...) lprintf("[DEBUG] %s:%d:%s(): " text "\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#else
    #define LPRINTF(text, ...) ((void)0)
#endif

// #define $ LPRINTF("MEOW");