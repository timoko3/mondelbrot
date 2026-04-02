#include "encode.h"
#include "strFunc.h"

#include <assert.h>
#include <stdio.h>
#include <malloc.h>

char* encodeRussian1251ToUTF8(char* text){
    assert(text);

    size_t len = myStrLen(text);
    char* utf8 = (char*) calloc(len * 3 + 1, sizeof(char));
    assert(utf8);

    size_t outInd = 0;

    for (size_t curSymInd = 0; curSymInd < len; curSymInd++) {
        unsigned char c = (unsigned char)text[curSymInd];
        int curSymUnicode;

        if (c >= 0xC0 && c <= 0xDF)       // А..Я
            curSymUnicode = 0x0410 + (c - 0xC0);
        else if (c >= 0xE0 && c < 0xFF)  // а..я
            curSymUnicode = 0x0430 + (c - 0xE0);
        else if (c == 0xA8)               // Ё
            curSymUnicode = 0x0401;
        else if (c == 0xB8)               // ё
            curSymUnicode = 0x0451;
        else                              // ASCII
            curSymUnicode = c;

        if (curSymUnicode < 0x80) {
            utf8[outInd++] = (char)curSymUnicode;
        } else if (curSymUnicode < 0x800) {
            utf8[outInd++] = (char)(0b11000000 | ((curSymUnicode >> 6) & 0b00011111));
            utf8[outInd++] = (char)(0b10000000 | (curSymUnicode & 0b00111111));
        } else {
            utf8[outInd++] = (char)(0b11100000 | ((curSymUnicode >> 12) & 0b00001111));
            utf8[outInd++] = (char)(0b10000000 | ((curSymUnicode >> 6) & 0b00111111));
            utf8[outInd++] = (char)(0b10000000 | (curSymUnicode & 0b00111111));
        }
    }

    utf8[outInd] = '\0';
    return utf8;
}