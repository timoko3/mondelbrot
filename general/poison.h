#ifndef POISON_H
#define POISON_H

#include <stddef.h>

void poisonMemory(void* ptr, size_t sizeToPollute);

#endif /* POISON_H */