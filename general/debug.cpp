#include "debug.h"
#include "file.h"

#include <stdarg.h>
#include <assert.h>
#include <stdarg.h>

static const char* LOG_FILE_NAME            = "logAkinator.txt";

void lprintf(const char* text, ...){
    assert(text);
    
    fileDescription logFile = {
        LOG_FILE_NAME,
        "ab+"
    };    

    FILE* logFilePtr = myOpenFile(&logFile);
    if(!logFilePtr) return;

    va_list args;
    va_start(args, text);
    vfprintf(logFilePtr, text, args);
    va_end(args);

    fclose(logFilePtr);
}