#include "TXLib.h"

// #define DEBUG
// #include "general/debug.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>


const size_t WIDTH_WINDOW  = 800;
const size_t HEIGHT_WINDOW = 600;

const float  dx            = 1 / WIDTH_WINDOW;
const float  dY            = 1 / HEIGHT_WINDOW;

const size_t AMOUNT_ITERATIONS = 256;

const size_t UNWRAP_NUMBER     = 4;
const size_t SIZE_OF_ARRAY     = UNWRAP_NUMBER * 2;   // for 8 complex numbers

inline void countMondelbrot(float* curComplex, float* curShift, int* exitIndexes, int curIter, char* flag);
inline void drawMondelbrot(RGBQUAD** videoMemBuffer, int curIter, int counterX, int counterY);

int main(void){
    txCreateWindow (WIDTH_WINDOW, HEIGHT_WINDOW);

    RGBQUAD* videoMemBuffer = txVideoMemory();

    float time   = 0;
    float fps    = 0;
    float curX   = 0;
    float curY   = 0;
    while(true){
        txLock();
        for(int counterX = 0; counterX < (int) WIDTH_WINDOW; counterX += UNWRAP_NUMBER, curX += UNWRAP_NUMBER * dx){
            float x0[UNWRAP_NUMBER] = {curX, curX + 1, curX + 2, curX + 3};
            float y0[UNWRAP_NUMBER] = {curY, curY, curY, curY};

            float curShiftX[UNWRAP_NUMBER] = {}; for(int i = 0; i < UNWRAP_NUMBER; i++) curShiftX[i] = x0[i];
            float curShiftY[UNWRAP_NUMBER] = {}; for(int i = 0; i < UNWRAP_NUMBER; i++) curShiftX[i] = y0[i];

            for(int counterY = 0; counterY < (int) HEIGHT_WINDOW; counterY += 8){
                float curComplex[SIZE_OF_ARRAY] = {0.0}; 
                int   exitIndexes[UNWRAP_NUMBER] = {0};
                // for(int i = 0; i < UNWRAP_NUMBER; i++) exitIndexes[i] = AMOUNT_ITERATIONS;

                if(HEIGHT_WINDOW - counterY > 8){

                    float curShift[SIZE_OF_ARRAY] = {0.0};

                    for(int i = 0; i < UNWRAP_NUMBER; i++){
                        curShift[2 * i]     = (counterX - 1000.0f - time) / 450.0f;
                        curShift[2 * i + 1] = (counterY + i - 300.0f) / 450.0f;
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
                        if (counterY + i < HEIGHT_WINDOW) { 
                            drawMondelbrot(&videoMemBuffer, exitIndexes[i], counterX, counterY + i);
                        }
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
    // assert(curComplex);
    // assert(curShift);
    // assert(exitIndexes);
    // assert(flag);
    
    float curComplexReSquare = (*curComplex)       * (*curComplex);
    float curComplexImSquare = (*(curComplex + 1)) * (*(curComplex + 1));
    float curComplexImRe     = (*(curComplex + 1)) * (*curComplex);
    
    int belongsSet           = (curComplexReSquare + curComplexImSquare <= 4.0f); 
    *exitIndexes += belongsSet;

    *curComplex       = (curComplexReSquare - curComplexImSquare + *curShift);
    *(curComplex + 1) = 2 * curComplexImRe + *(curShift + 1);

}

inline void drawMondelbrot(RGBQUAD** videoMemBuffer, int curIter, int counterX, int counterY){
    assert(videoMemBuffer);

    (*videoMemBuffer)[counterX + (-counterY + HEIGHT_WINDOW - 1) * WIDTH_WINDOW].rgbRed   = curIter % 113;
    (*videoMemBuffer)[counterX + (-counterY + HEIGHT_WINDOW - 1) * WIDTH_WINDOW].rgbGreen = curIter % 128;
    (*videoMemBuffer)[counterX + (-counterY + HEIGHT_WINDOW - 1) * WIDTH_WINDOW].rgbBlue  = curIter % 10;
}