#ifndef FILE_PARSE_H
#define FILE_PARSE_H

#include "strFunc.h"

#include <stdio.h>

const char* const ALERT_FILE_OPEN_FAILURE               = "Ошибка при открытии файла %s\n";
const char* const ALERT_GET_INFO_FAILURE                = "Ошибка при попытке получить информацию о файле\n";
const char* const ALERT_STR_FROM_FILE_TO_STRUCT_FAILURE = "Ошибка при передаче информации о файле в структуру\n";

const char* const FUNCTION_FAILURE_ALERT = "failed";

struct data_t {
    size_t fileSize;
    FILE* inputFile;
    size_t bufferSize;
    char* buffer;
    size_t nStrings;
    myString* strings;
};

struct strings_t{
    myString* ptrs; // ptrs
    size_t  count;
};

struct fileDescription {
    const char* name;
    const char* openMode;
};

int parseStringsFile(data_t* data, const char* fileName);

size_t getFileSize(const char* fileName);
FILE* myOpenFile(fileDescription* file);

void writeOpcode(buffer_t* buffer, const char* fileName);
bool getIntNumsToBuffer(fileDescription file, size_t fileSize, int** buffer);

#endif /*FILE_PARSE_H*/
