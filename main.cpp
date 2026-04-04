#include "TXLib.h"

#define DEBUG
#include "general/debug.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>


const size_t WIDTH_WINDOW  = 800;
const size_t HEIGHT_WINDOW = 600;

const size_t AMOUNT_ITERATIONS = 256;

const size_t UNWRAP_NUMBER     = 8;
const size_t SIZE_OF_ARRAY     = UNWRAP_NUMBER * 2;   // for 8 complex numbers

inline void countMondelbrot(float* curComplex, float* curShift, int* exitIndexes, int curIter, char* flag);
inline void drawMondelbrot(RGBQUAD** videoMemBuffer, int curIter, int curX, int curY);

int main(void){
    txCreateWindow (WIDTH_WINDOW, HEIGHT_WINDOW);

    RGBQUAD* videoMemBuffer = txVideoMemory();

    LPRINTF("MEOW");

    float time = 0;
    float fps  = 0;
    while(true){
        txLock();
        for(int curX = 0; curX < (int) WIDTH_WINDOW; curX++){
            for(int curY = 0; curY < (int) HEIGHT_WINDOW; curY += 8){
                float curComplex[SIZE_OF_ARRAY] = {0.0}; 
                int   exitIndexes[UNWRAP_NUMBER] = {0};

                if(HEIGHT_WINDOW - curY > 8){

                    float curShift[SIZE_OF_ARRAY] = {0.0};

                    for(size_t i = 0; i < SIZE_OF_ARRAY / 2; i++){
                        curShift[i]     = (curX - 1000 - time) / (450.0);
                        curShift[i + 1] = (curY - 300 + i) / (450.0);
                    }

                    char countFlag[UNWRAP_NUMBER] = {0};

                    int curIter = 0;
                    while(curIter < AMOUNT_ITERATIONS){
                        countMondelbrot(curComplex + 0 , curShift + 0 , exitIndexes + 0, curIter, countFlag + 0);
                        countMondelbrot(curComplex + 2 , curShift + 2 , exitIndexes + 1, curIter, countFlag + 1);
                        countMondelbrot(curComplex + 4 , curShift + 4 , exitIndexes + 2, curIter, countFlag + 2);
                        countMondelbrot(curComplex + 6 , curShift + 6 , exitIndexes + 3, curIter, countFlag + 3);
                        countMondelbrot(curComplex + 8 , curShift + 8 , exitIndexes + 4, curIter, countFlag + 4);
                        countMondelbrot(curComplex + 10, curShift + 10, exitIndexes + 5, curIter, countFlag + 5);
                        countMondelbrot(curComplex + 12, curShift + 12, exitIndexes + 6, curIter, countFlag + 6);
                        countMondelbrot(curComplex + 14, curShift + 14, exitIndexes + 7, curIter, countFlag + 7);
                        
                        curIter++;
                    }
                
                    for(size_t i = 0; i < SIZE_OF_ARRAY / 2; i++){
                        drawMondelbrot(&videoMemBuffer, exitIndexes[i], curX, curY + i);
                    }

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
        // time += 1;
    }

    return 0;
}


inline void countMondelbrot(float* curComplex, float* curShift, int* exitIndexes, int curIter, char* flag){
    assert(curComplex);
    assert(curShift);
    assert(exitIndexes);
    assert(flag);

    if(*flag) return;
    
    float curComplexReSquare = (*curComplex)       * (*curComplex);
    float curComplexImSquare = (*(curComplex + 1)) * (*(curComplex + 1));
    float curComplexImRe     = (*(curComplex + 1)) * (*curComplex);
    
    if(curComplexReSquare + curComplexImSquare > 4){
        *exitIndexes = curIter;
        *flag = 1;
        return;
    } 

    *curComplex       = (curComplexReSquare - curComplexImSquare + *curShift);
    *(curComplex + 1) = 2 * curComplexImRe + *(curShift + 1);

}

inline void drawMondelbrot(RGBQUAD** videoMemBuffer, int curIter, int curX, int curY){
    assert(videoMemBuffer);

    (*videoMemBuffer)[curX + (-curY + HEIGHT_WINDOW - 1) * WIDTH_WINDOW].rgbRed   = curIter % 113;
    (*videoMemBuffer)[curX + (-curY + HEIGHT_WINDOW - 1) * WIDTH_WINDOW].rgbGreen = curIter % 128;
    (*videoMemBuffer)[curX + (-curY + HEIGHT_WINDOW - 1) * WIDTH_WINDOW].rgbBlue  = curIter % 10;
}