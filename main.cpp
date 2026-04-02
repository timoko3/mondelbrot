#include "TXLib.h"

#include <stdio.h>
#include <math.h>

const size_t WIDTH_WINDOW  = 800;
const size_t HEIGHT_WINDOW = 600;

const size_t AMOUNT_ITERATIONS = 1000;

int main(void){
    txCreateWindow (WIDTH_WINDOW, HEIGHT_WINDOW);

    double curComplexRe         = 0;
    double curComplexIm         = 0;
    double nextComplexRe        = 0;
    double nextComplexIm        = 0;

    for(int curX = 0; curX < (int) WIDTH_WINDOW; curX++){
        for(int curY = 0; curY < (int) HEIGHT_WINDOW; curY++){
            double curComplexRe         = 0;
            double curComplexIm         = 0;

            double curShiftRe           = (curX - 600) / 450.0; 
            double curShiftIm           = (curY - 300) / 450.0; 



            int curIter = 0;
            while(curComplexRe * curComplexRe + curComplexIm * curComplexIm < 4 && curIter < AMOUNT_ITERATIONS){
                nextComplexRe = (curComplexRe * curComplexRe - curComplexIm * curComplexIm + curShiftRe);
                nextComplexIm = 2 * curComplexRe * curComplexIm + curShiftIm;

                curComplexRe = nextComplexRe;
                curComplexIm = nextComplexIm;
                
                curIter++;
            }
            


            if(curIter == AMOUNT_ITERATIONS){
                txSetPixel(curX, curY, TX_CYAN);
            }
            else if(AMOUNT_ITERATIONS - curIter < AMOUNT_ITERATIONS * 0.999){
                txSetPixel(curX, curY, TX_RED);
            }
            else if(AMOUNT_ITERATIONS - curIter < AMOUNT_ITERATIONS * 0.75){
                txSetPixel(curX, curY, TX_ORANGE);
            }
            else if(AMOUNT_ITERATIONS - curIter < AMOUNT_ITERATIONS * 0.5){
                txSetPixel(curX, curY, TX_HUE);
            }
            else if(AMOUNT_ITERATIONS - curIter < AMOUNT_ITERATIONS * 0.25){
                txSetPixel(curX, curY, TX_LIGHTGREEN);
            }


        }
    }

    return 0;
}