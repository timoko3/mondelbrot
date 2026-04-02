#include "file.h"

#include <sys/stat.h>
#include <stdlib.h>
#include <assert.h>

#define $ fprintf(stderr, "MEOW in %s:%d\n", __FILE__, __LINE__);

// #define DEBUG

static char* getTextToBuffer(FILE* inputFile, size_t fileSize);
static myString* divideBufferToStringsStructure(char* buffer, size_t nStrings);

#ifdef DEBUG
static void printBuffer(char* buffer);
#endif /* DEBUG */

int parseStringsFile(data_t* data, const char* fileName){
    assert(data);
    assert(fileName);

    if((data->fileSize = getFileSize(fileName)) == EXIT_FAILURE) return EXIT_FAILURE;

    fileDescription inputFile = {
        fileName, 
        "rb"
    };

    data->inputFile = myOpenFile(&inputFile);
    if(!data->inputFile) return EXIT_FAILURE;
    assert(data->inputFile);

    data->buffer = getTextToBuffer(data->inputFile, data->fileSize);
    assert(data->buffer);

    fclose(data->inputFile);

    data->nStrings = countStrings(data->buffer, data->fileSize, END_STR);
    data->bufferSize = data->fileSize + SIZE_OF_END_FILE;
    
    data->strings = divideBufferToStringsStructure(data->buffer, data->nStrings);
    assert(data->strings);

    return 0;
}

size_t getFileSize(const char* fileName){
    assert(fileName);
    
    struct stat file_info = {};
    if(stat(fileName, &file_info) != 0){
        fprintf(stderr, ALERT_GET_INFO_FAILURE);
        perror(FUNCTION_FAILURE_ALERT);
        return EXIT_FAILURE;
    }

    return (size_t) file_info.st_size;
}

FILE* myOpenFile(fileDescription* file){
    assert(file);
    
    FILE* filePtr = fopen(file->name, file->openMode);

    if(!filePtr){
        fprintf(stderr, ALERT_FILE_OPEN_FAILURE, file->name);
        perror(FUNCTION_FAILURE_ALERT);
        return NULL;
    }
    
    return filePtr;
}

static char* getTextToBuffer(FILE* inputFIle, size_t fileSize){
    assert(inputFIle);

    char* buffer = (char*) calloc(fileSize + SIZE_OF_END_FILE, sizeof(char)); 
    assert(buffer);
    buffer[fileSize] = '\0';

    fread(buffer, sizeof(char), fileSize, inputFIle);
    
    return buffer;
}

static myString* divideBufferToStringsStructure(char* buffer, size_t nStrings){
    // struct 
    assert(buffer);
    
    myString* strings = (myString*) calloc(nStrings, sizeof(myString));
    assert(strings);

    strings[0].ptr = buffer;

    #ifdef DEBUG
    printBuffer(buffer);
    #endif /* DEBUG */

    int curStr = 1;
    int curSym = 0;
    for(curSym = 0; buffer[curSym + 1] != '\0'; curSym++){
        #ifdef DEBUG
        fprintf(stderr, "curSym = %d\n", curSym);
        fprintf(stderr, "buffer[curSym] = %d\n", buffer[curSym]);
        fprintf(stderr, "buffer[curSym + 1] = %d\n", buffer[curSym + 1]);
        #endif /* DEBUG */
        if(buffer[curSym] == END_STR){
            strings[curStr].ptr = buffer + curSym + 1;
            strings[curStr - 1].len = (size_t) strings[curStr].ptr - 
                                      (size_t) strings[curStr - 1].ptr;
            curStr++;
        }
        
    }
    
    strings[curStr - 1].len = (size_t) ((buffer + curSym) - strings[curStr - 1].ptr);

    return strings;
}

void writeOpcode(buffer_t* buffer, const char* fileName){
    assert(buffer);
    assert(fileName);

    fileDescription byteCode = {
        fileName,
        "wb"
    };

    FILE* byteCodeFile = myOpenFile(&byteCode);
    assert(byteCodeFile);

    size_t written = fwrite(buffer->ptr, sizeof(int), buffer->size, byteCodeFile);
    if(written != buffer->size){
        perror("fwrite error");
    }

    fclose(byteCodeFile);
}

bool getIntNumsToBuffer(fileDescription file, size_t fileSize, int** buffer){
    assert(buffer);

    FILE* byteCodeFile = myOpenFile(&file);
    assert(byteCodeFile);

    fread(*buffer, 1, fileSize, byteCodeFile);
    fclose(byteCodeFile);
    
    return true;
}

#ifdef DEBUG
static void printBuffer(char* buffer){
    assert(buffer);

    for(size_t curSym = 0; curSym < 31; curSym++){
        printf("Символ %ld: %d\n", curSym, buffer[curSym]);
    }   
}
#endif /* DEBUG */