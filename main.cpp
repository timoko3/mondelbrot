#include "TXLib.h"

#include <stdio.h>
#include <math.h>

const size_t WIDTH_WINDOW  = 800;
const size_t HEIGHT_WINDOW = 600;

int main(void){
    txCreateWindow (WIDTH_WINDOW, HEIGHT_WINDOW);

    double powTwoX              = 0;
    double powTwoY              = 0;
    double curComplexRe         = 0;
    double curComplexIm         = 0;
    double nextComplexRe        = 0;
    double nextComplexIm        = 0;

    for(int curX = 0; curX < (int) WIDTH_WINDOW; curX++){
        for(int curY = 0; curY < (int) HEIGHT_WINDOW; curY++){
            powTwoX = curX * curX;
            powTwoX = curY * curY;
            
            nextComplexRe = (curComplexRe * curComplexRe - curComplexIm * curComplexIm + curX);
            nextComplexIm = 2 * curComplexRe *curComplexIm + curY;

            curComplexRe = nextComplexRe;
            curComplexIm - nextComplexIm;

            if(curComplexRe * curComplexRe - curComplexIm * curComplexIm < 4){
                txSetPixel(curX, curY, TX_LIGHTRED);
            }


        }
    }

    return 0;
}