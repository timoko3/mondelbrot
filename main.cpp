#include "TXLib.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

const size_t WIDTH_WINDOW  = 800;
const size_t HEIGHT_WINDOW = 600;

const size_t AMOUNT_ITERATIONS = 256;

int main(void){
    txCreateWindow (WIDTH_WINDOW, HEIGHT_WINDOW);

    double curComplexRe         = 0;
    double curComplexIm         = 0;
    double nextComplexRe        = 0;
    double nextComplexIm        = 0;

    RGBQUAD* videoMemBuffer = txVideoMemory();

    RGBQUAD curColor;

    double time = 0;
    double fps  = 0;
    while(true){
        txLock();
        for(int curX = 0; curX < (int) WIDTH_WINDOW; curX++){
            for(int curY = 0; curY < (int) HEIGHT_WINDOW; curY++){
                double curComplexRe         = 0;
                double curComplexIm         = 0;

                double curShiftRe           = (curX - 1000) / (450.0 + time); 
                double curShiftIm           = (curY - 300) / (450.0 + time); 

                int curIter = 0;
                while(curComplexRe * curComplexRe + curComplexIm * curComplexIm < 4 && curIter < AMOUNT_ITERATIONS){
                    nextComplexRe = (curComplexRe * curComplexRe - curComplexIm * curComplexIm + curShiftRe);
                    nextComplexIm = 2 * curComplexRe * curComplexIm + curShiftIm;

                    curComplexRe = nextComplexRe;
                    curComplexIm = nextComplexIm;
                    
                    curIter++;
                }
                
                
                videoMemBuffer[curX + (-curY + HEIGHT_WINDOW - 1) * WIDTH_WINDOW].rgbRed   = curIter % 113;
                videoMemBuffer[curX + (-curY + HEIGHT_WINDOW - 1) * WIDTH_WINDOW].rgbGreen = curIter % 128;
                videoMemBuffer[curX + (-curY + HEIGHT_WINDOW - 1) * WIDTH_WINDOW].rgbBlue  = curIter % 10;
                
            }
        }
        txUnlock();
        // txRedrawWindow();
        
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