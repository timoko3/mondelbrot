#include <assert.h>

#include "hash.h"

unsigned long hash(const void* ptr, size_t size){
    assert(ptr);

    unsigned long hash = 5381;
    for(size_t curByteInd = 0; curByteInd < size; curByteInd++){
        hash =((hash << 5) + hash) + (unsigned char) (*((const char*)ptr + curByteInd));
    }

    return hash;
}