#include "TXLib.h"

#define DEBUG
#include "general/debug.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include <immintrin.h>

const size_t WIDTH_WINDOW         = 800;
const size_t HEIGHT_WINDOW        = 600;

const float  SCALE_NUM            = 450.0;

const float  dx                   = 1 / SCALE_NUM;
const float  dy                   = 1 / SCALE_NUM;

const float  MAX_COMPLEX_NUM_SIZE = 4.0;
const size_t AMOUNT_ITERATIONS    = 256;

const size_t UNWRAP_NUMBER        = 16;

inline void countMondelbrot(float* curComplex, float* curShift, int* exitIndexes, int curIter, char* flag);
inline void drawMondelbrot(RGBQUAD** videoMemBuffer, int curIter, int counterX, int counterY);

void debugPrintV512(__m512 v, const char* name);
void debugPrintV512i(__m512i v, const char* name);
void debugPrintMask16(__mmask16 mask, const char* name);

int main(void){
    txCreateWindow (WIDTH_WINDOW, HEIGHT_WINDOW);

    RGBQUAD* videoMemBuffer = txVideoMemory();

    float time   = 0;
    float fps    = 0;
    float curXPosScaled   = 0;
    float curYPosScaled   = 0;

    debugPrintMask16(0x0f0f, "test");
    while(time < 1){
        txLock();
        for(int counterX = 0; counterX < (int) WIDTH_WINDOW; counterX++){
            curXPosScaled = (counterX - 1000.0f) / SCALE_NUM;
            for(int counterY = 0; counterY < (int) HEIGHT_WINDOW; counterY += UNWRAP_NUMBER){
                curYPosScaled = (counterY - 300.0f)  / SCALE_NUM;

                __m512 curShiftRe = _mm512_set1_ps(curXPosScaled);                         
                __m512 curShiftIm = _mm512_set_ps(curYPosScaled + 15 * dy, curYPosScaled + 14 * dy, curYPosScaled + 13 * dy, curYPosScaled + 12 * dy,
                                                curYPosScaled + 11 * dy, curYPosScaled + 10 * dy, curYPosScaled + 9 * dy,  curYPosScaled + 8 * dy,
                                                curYPosScaled + 7 * dy,  curYPosScaled + 6 * dy,  curYPosScaled + 5 * dy,  curYPosScaled + 4 * dy,
                                                curYPosScaled + 3 * dy,  curYPosScaled + 2 * dy,  curYPosScaled + dy,      curYPosScaled);

                __m512 curComplexRe = curShiftRe;
                __m512 curComplexIm = curShiftIm;

                __m512i exitIndexes = _mm512_set1_epi32(0);

                int curIter = 0;
                if(HEIGHT_WINDOW - counterY > 16){
                    while(curIter < AMOUNT_ITERATIONS){

                        __m512 curComplexReSquare = _mm512_mul_ps(curComplexRe, curComplexRe);
                        __m512 curComplexImSquare = _mm512_mul_ps(curComplexIm, curComplexIm);
                        __m512 curComplexImRe     = _mm512_mul_ps(curComplexRe, curComplexIm);

                        __m512 curComplexSquare   = _mm512_add_ps(curComplexReSquare, curComplexImSquare);


                        __m512 unwNumber        =  _mm512_set1_ps(MAX_COMPLEX_NUM_SIZE);
                        __mmask16 mask          = _mm512_mask_cmp_ps_mask(0xFFFF, curComplexSquare, unwNumber, _CMP_LT_OS);
                        exitIndexes             = _mm512_mask_add_epi32(exitIndexes, mask, exitIndexes, _mm512_set1_epi32(1));
                        
                        debugPrintMask16(mask, "Mask");

                        curComplexRe            = _mm512_mask_sub_ps(curComplexRe, mask, curComplexReSquare, curComplexImSquare);
                        curComplexRe            = _mm512_mask_add_ps(curComplexRe, mask, curComplexRe, curShiftRe);

                        curComplexIm            = _mm512_mask_add_ps(curComplexIm, mask, curComplexImRe, curComplexImRe);
                        curComplexIm            = _mm512_mask_add_ps(curComplexIm, mask, curComplexIm, curShiftIm);

                        curIter++;
                    }
                    
                    __m512i v_red   = _mm512_and_epi32(exitIndexes, _mm512_set1_epi32(113)); 
                    __m512i v_green = _mm512_and_epi32(exitIndexes, _mm512_set1_epi32(127)); 
                    __m512i v_blue  = _mm512_and_epi32(exitIndexes, _mm512_set1_epi32(10));  

                    __m512i v_pixel = _mm512_or_si512(_mm512_slli_epi32(v_red, 16), _mm512_or_si512(_mm512_slli_epi32(v_green, 8), v_blue));

                    void* dest_addr = &videoMemBuffer[counterX + (-(counterY) + HEIGHT_WINDOW - 1) * WIDTH_WINDOW];

                    _mm512_storeu_si512((__m512i*)dest_addr, v_pixel);
                }
            }
        }
        txUnlock();
        
        fps = txGetFPS();

        char printStr[15];
        sprintf(printStr, "fps: %.2f", fps);
        txSetColor (TX_YELLOW); 
        txTextOut(0, 0, printStr);
        txRedrawWindow();
        time += 1;
    }

    return 0;
}

void debugPrintV512(__m512 v, const char* name){
    float temp[16];
    _mm512_storeu_ps(temp, v);
    lprintf("%s: ", name);
    for (int i = 0; i < 16; i++) lprintf("%.2f ", temp[i]);
    lprintf("\n");
}

void debugPrintV512i(__m512i v, const char* name){
    int temp[16];
    _mm512_storeu_si512((__m512i*)temp, v);
    lprintf("%s: ", name);
    for (int i = 0; i < 16; i++) lprintf("%d ", temp[i]);
    lprintf("\n");
}

void debugPrintMask16(__mmask16 mask, const char* name){
    lprintf("%s: ", name);
    for (int i = 0; i < 16; i++) lprintf("%d ", (mask >> i) & 1);
    lprintf("\n");
}