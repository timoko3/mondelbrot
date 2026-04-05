#include "debug.h"
#include "file.h"

#include <stdarg.h>
#include <assert.h>
#include <stdarg.h>

const char* LOG_FILE_NAME = "logMb.txt";

void lprintf(const char* text, ...){
    assert(text);
    
    static size_t amountCalls = 0;
    amountCalls++;

    fileDescription logFile;
    if(amountCalls == 1){
        logFile = {
            LOG_FILE_NAME,
            "wb+"
        };  
    }
    else{
        logFile = {
            LOG_FILE_NAME,
            "ab+"
        };    
    }
    
    FILE* logFilePtr = myOpenFile(&logFile);
    if(!logFilePtr) return;

    va_list args;
    va_start(args, text);
    vfprintf(logFilePtr, text, args);
    va_end(args);

    fclose(logFilePtr);
}
