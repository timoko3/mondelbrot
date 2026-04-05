#include "TXLib.h"

// #define DEBUG
// #include "general/debug.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>


const size_t WIDTH_WINDOW         = 800;
const size_t HEIGHT_WINDOW        = 600;

const float  SCALE_NUM            = 450.0;

const float  dx                   = 1 / SCALE_NUM;
const float  dy                   = 1 / SCALE_NUM;

const float  MAX_COMPLEX_NUM_SIZE = 4.0;
const size_t AMOUNT_ITERATIONS    = 256;

const size_t UNWRAP_NUMBER        = 4;

inline void countMondelbrot(float* curComplex, float* curShift, int* exitIndexes, int curIter, char* flag);
inline void drawMondelbrot(RGBQUAD** videoMemBuffer, int curIter, int counterX, int counterY);

int main(void){
    txCreateWindow (WIDTH_WINDOW, HEIGHT_WINDOW);

    RGBQUAD* videoMemBuffer = txVideoMemory();

    float time   = 0;
    float fps    = 0;
    float curXPosScaled   = 0;
    float curYPosScaled   = 0;
    while(true){
        txLock();
        for(int counterX = 0; counterX < (int) WIDTH_WINDOW; counterX++){
            curXPosScaled = (counterX - 1000.0f) / SCALE_NUM;
            for(int counterY = 0; counterY < (int) HEIGHT_WINDOW; counterY += UNWRAP_NUMBER){
                curYPosScaled = (counterY - 300.0f)  / SCALE_NUM;

                float curShiftRe[UNWRAP_NUMBER]         = {curXPosScaled, curXPosScaled, curXPosScaled, curXPosScaled};
                float curShiftIm[UNWRAP_NUMBER]         = {curYPosScaled, curYPosScaled + dy, curYPosScaled + 2 * dy, curYPosScaled + 3 * dy};

                float curComplexRe[UNWRAP_NUMBER]       = {0.0}; for(int i = 0; i < UNWRAP_NUMBER; i++) curComplexRe[i] = curShiftRe[i]; 
                float curComplexIm[UNWRAP_NUMBER]       = {0.0}; for(int i = 0; i < UNWRAP_NUMBER; i++) curComplexIm[i] = curShiftIm[i];

                int   exitIndexes[UNWRAP_NUMBER]        = {0}; 

                int curIter = 0;
                
                if(HEIGHT_WINDOW - counterY > UNWRAP_NUMBER){
                    while(curIter < AMOUNT_ITERATIONS){

                        float curComplexReSquare[UNWRAP_NUMBER] = {0.0}; for(int i = 0; i < UNWRAP_NUMBER; i++) curComplexReSquare[i] = curComplexRe[i] * curComplexRe[i];
                        float curComplexImSquare[UNWRAP_NUMBER] = {0.0}; for(int i = 0; i < UNWRAP_NUMBER; i++) curComplexImSquare[i] = curComplexIm[i] * curComplexIm[i];
                        float curComplexImRe[UNWRAP_NUMBER]     = {0.0}; for(int i = 0; i < UNWRAP_NUMBER; i++) curComplexImRe[i]     = curComplexRe[i] * curComplexIm[i];

                        float curComplexSquare[UNWRAP_NUMBER]   = {0.0}; for(int i = 0; i < UNWRAP_NUMBER; i++) curComplexSquare[i]     = curComplexReSquare[i] + curComplexImSquare[i];

                        int curComplexCmp[UNWRAP_NUMBER]        = {0};   for(int i = 0; i < UNWRAP_NUMBER; i++) if(curComplexSquare[i] < MAX_COMPLEX_NUM_SIZE) curComplexCmp[i] = 1;

                        int mask = 0;
                        for(int i = 0; i < UNWRAP_NUMBER; i++)  mask |= (curComplexCmp[i] << i);
                        if(!mask) break;

                        for(int i = 0; i < UNWRAP_NUMBER; i++)  exitIndexes[i]  = exitIndexes[i] + curComplexCmp[i];
                        
                        for(int i = 0; i < UNWRAP_NUMBER; i++)  if((mask >> i) & 1) curComplexRe[i] = (curComplexReSquare[i] - curComplexImSquare[i] + curShiftRe[i]);
                        for(int i = 0; i < UNWRAP_NUMBER; i++)  if((mask >> i) & 1) curComplexIm[i] = (curComplexImRe[i] + curComplexImRe[i] + curShiftIm[i]);

                        curIter++;
                    }
                    
                    for(int i = 0; i < UNWRAP_NUMBER; i++){
                        videoMemBuffer[counterX + (-(counterY + i) + HEIGHT_WINDOW - 1) * WIDTH_WINDOW].rgbRed   = exitIndexes[i] % 113;
                        videoMemBuffer[counterX + (-(counterY + i) + HEIGHT_WINDOW - 1) * WIDTH_WINDOW].rgbGreen = exitIndexes[i] % 128;
                        videoMemBuffer[counterX + (-(counterY + i) + HEIGHT_WINDOW - 1) * WIDTH_WINDOW].rgbBlue  = exitIndexes[i] % 10;
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