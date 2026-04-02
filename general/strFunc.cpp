#include "strFunc.h"
#include "hash.h"
#include "poison.h"
#define DEBUG
#include "debug.h"

#include <assert.h>
#include <stdio.h>

size_t myStrLen(const char* start, char endStr){
    assert(start);

    size_t curSymInd = 0;
    while(start[curSymInd] != endStr) curSymInd++;
    
    return curSymInd;
}

char* myStrCpy(char* dest, const char* src){
    assert(dest);
    assert(src);

    poisonMemory(dest, myStrLen(dest) + 1);

    int i = 0;

    while(src[i] != '\0'){
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';

    return dest;
}

char* myFGets(char* str, int count, FILE* stream){
    assert(str);
    assert(stream);
    if(count < 1)return NULL;

    int i = 0;
    while(i < count) {
        str[i] = (char)fgetc(stream);
        LPRINTF("Код текущего символа: %d\n", str[i]);
        if(str[i] == '\n'){
            break;
        }

        i++;
    }
    str[i] = '\0';

    return str;
}

bool isEqualStrings(const char* str1, const char* str2){
    LPRINTF("hash answer %s, длина %lu: %ld, hash %s, длина %lu: %lu\n", str1, myStrLen(str1) + 1, hash(str1, myStrLen(str1) + 1), str2, myStrLen(str2) + 1, hash(str2, myStrLen(str2) + 1));
    return hash(str1, myStrLen(str1) + 1) == hash(str2, myStrLen(str2) + 1);
}

size_t countStrings(char* buf, size_t fileSize, char endStr){
    assert(buf);

    size_t nStrings = 1;

    size_t curSym = 0;
    while(curSym < fileSize){
        if(buf[curSym] == endStr){
            nStrings++;
        }
        curSym++;
    }

    return nStrings;
}

void clearBuffer(){
    while(getchar() != '\n'){
        continue;
    }
}

bool isYes(char* answer){
    assert(answer);

    return isEqualStrings(answer, "да");
}

bool isNo(char* answer){
    assert(answer);

    return isEqualStrings(answer, "нет");
}