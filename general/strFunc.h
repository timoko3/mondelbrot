#ifndef STR_FUNC_H
#define STR_FUNC_H

#include <stddef.h>
#include <stdio.h>

const size_t         MAX_ANSWER_SIZE = 256;

struct myString {
    char* ptr;
    size_t len;
};

struct buffer_t{
    int* ptr;
    size_t size;
};

const int UPPER_SYM_MIN        = (int) 'A'; 
const int UPPER_SYM_MAX        = 'Z'; 
const int UPPER_TO_LOWER_SHIFT = 'A' - 'a'; 
const int SIZE_OF_END_FILE     = 1; 

const char END_STR             = '\n';

size_t countStrings(char* buf, size_t fileSize, char endStr);
bool isEqualStrings(const char* str1, const char* str2);
size_t myStrLen(const char* start, char endStr ='\0');
char* myStrCpy(char* dest, const char* src);
char* myFGets(char* str, int count, FILE* stream);
void clearBuffer();

bool isYes(char* answer);
bool isNo(char* answer);

#endif /* STR_FUNC_H */