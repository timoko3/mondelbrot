#include "TXLib.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

const size_t WIDTH_WINDOW  = 800;
const size_t HEIGHT_WINDOW = 600;

const size_t AMOUNT_ITERATIONS = 256;

int main(void){
    txCreateWindow (WIDTH_WINDOW, HEIGHT_WINDOW);

    RGBQUAD* videoMemBuffer = txVideoMemory();

    float time = 0;
    float fps  = 0;
    while(true){
        txLock();
        for(int curX = 0; curX < (int) WIDTH_WINDOW; curX++){
            for(int curY = 0; curY < (int) HEIGHT_WINDOW; curY++){
                float curComplexRe         = 0;
                float curComplexIm         = 0;

                double curComplexRe2       = 0;     // overflow if float        
                double curComplexIm2       = 0;     // overflow if float
                float curComplexImRe       = 0;

                float curShiftRe           = (curX - 1000 - time) / (450.0); 
                float curShiftIm           = (curY - 300) / (450.0); 

                int curIter = 0;
                while(curComplexRe2 + curComplexIm2 < 4 && curIter < AMOUNT_ITERATIONS){
                    curComplexRe2        = curComplexRe * curComplexRe;
                    curComplexIm2        = curComplexIm * curComplexIm;
                    curComplexImRe       = curComplexIm * curComplexRe;

                    curComplexRe = (curComplexRe2 - curComplexIm2 + curShiftRe);
                    curComplexIm = 2 * curComplexImRe + curShiftIm;

                    curIter++;
                }
                
                
                videoMemBuffer[curX + (-curY + HEIGHT_WINDOW - 1) * WIDTH_WINDOW].rgbRed   = curIter % 113;
                videoMemBuffer[curX + (-curY + HEIGHT_WINDOW - 1) * WIDTH_WINDOW].rgbGreen = curIter % 128;
                videoMemBuffer[curX + (-curY + HEIGHT_WINDOW - 1) * WIDTH_WINDOW].rgbBlue  = curIter % 10;
                
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


void