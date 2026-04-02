#include "TXLib.h"

#include <stdio.h>
#include <math.h>

const size_t WIDTH_WINDOW  = 800;
const size_t HEIGHT_WINDOW = 600;

int main(void){
    txCreateWindow (WIDTH_WINDOW, HEIGHT_WINDOW);

    double powX = 0;
    double powY = 0;
    for(int curX = 0; curX < (int) WIDTH_WINDOW; curX++){
        for(int curY = 0; curY < (int) HEIGHT_WINDOW; curY++){
            powX = fabs(pow(curX - 400, 2.0));
            powY= fabs(pow(curY - 300, 2.0));
            // fprintf(stderr, "powX: %lf, powY: %lf\n", powX, powY);
            if(powX + powY < 100*100){
                txSetPixel(curX, curY, TX_LIGHTRED);
            }
        }
    }

    return 0;
}