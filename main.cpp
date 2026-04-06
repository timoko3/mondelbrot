
#define DEBUG
#include "general/debug.h"

#include "measuresHandler/measuresHandler.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <cstdint>
#include <malloc.h>

#include <immintrin.h>
#include <windows.h>

const size_t WIDTH_WINDOW         = 800;
const size_t HEIGHT_WINDOW        = 600;

const float  SCALE_NUM            = 450.0;

const float  dx                   = 1 / SCALE_NUM;
const float  dy                   = 1 / SCALE_NUM;

const float  MAX_COMPLEX_NUM_SIZE = 4.0;
const size_t AMOUNT_ITERATIONS    = 256;

const size_t UNWRAP_NUMBER        = 16;

typedef double perf_time_t;

const size_t AMOUNT_MEASURES      = 10;

perf_time_t calculateFrames(int nFrames);
void inline countMandelbrot();
inline void countDotsVectorMb(__m512* curComplexRe, __m512* curComplexIm, 
                              __m512 curShiftRe, __m512 curShiftIm,
                              __m512i* exitIndexes);
inline void drawMb(__m512i exitIndexes, RGBQUAD* videoMemBuffer, 
                  int counterX, int counterY);

inline __m512i convertDataForDraw(__m512i exitIndexes);
inline void    saveDataForDraw(__m512i vPixel, RGBQUAD* videoMemBuffer,
                               int counterX, int counterY);

inline void showFps(float fps);

static inline int64_t GetTicks();
static inline int64_t GetFrequency();

inline void basicVersionCalculations();

void debugPrintV512(__m512 v, const char* name);
void debugPrintV512i(__m512i v, const char* name);
void debugPrintMask16(__mmask16 mask, const char* name);

int main(void){
    // txCreateWindow (WIDTH_WINDOW, HEIGHT_WINDOW);

    SetProcessAffinityMask(GetCurrentProcess(), 1 << 2);
    SetThreadAffinityMask (GetCurrentThread(),  1 << 2);
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

    perf_time_t* perfromTimes = (perf_time_t*) calloc(AMOUNT_MEASURES, sizeof(perf_time_t));  
    assert(perfromTimes);


    for(size_t curMeasureInd = 0; curMeasureInd < AMOUNT_MEASURES; curMeasureInd++){
        perfromTimes[curMeasureInd] = calculateFrames(1000);
    }

    perf_time_t avg = countAverage(perfromTimes, 2, AMOUNT_MEASURES);
    perf_time_t avgInfelicity = 0;
    perf_time_t* infelicities = countInfelicity(avg, perfromTimes, AMOUNT_MEASURES, &avgInfelicity);

    createCsvWithResults(perfromTimes, infelicities, avg, avgInfelicity, AMOUNT_MEASURES);

    free(perfromTimes);
    free(infelicities);

    return 0;
}

perf_time_t calculateFrames(int nFrames){
    int64_t startTicks = GetTicks();

    int curFrame = 0;
    float fps    = 0;

    while(curFrame < nFrames){
        // countMandelbrot();
        basicVersionCalculations();

        // fps = txGetFPS();
        // showFps(fps);
        // txRedrawWindow();
        curFrame += 1;
    }

    int64_t endTicks = GetTicks();

    int64_t frequency = GetFrequency();

    return (double) (endTicks - startTicks) / (double) frequency;
}

void inline countMandelbrot(){
    float curXPosScaled   = 0;
    float curYPosScaled   = 0;

    // RGBQUAD* videoMemBuffer = txVideoMemory();

    // txLock();
    for(int counterY = 0; counterY < (int) HEIGHT_WINDOW; counterY++){
        curYPosScaled = ((float) counterY - 300.0f)  / SCALE_NUM;
        for(int counterX = 0; counterX < (int) WIDTH_WINDOW; counterX += UNWRAP_NUMBER){
            curXPosScaled = ((float) counterX - 1000.0f) / SCALE_NUM;

            __m512 curShiftRe = _mm512_set_ps(curXPosScaled + 15 * dx, curXPosScaled + 14 * dx, curXPosScaled + 13 * dx, curXPosScaled + 12 * dx,
                curXPosScaled + 11 * dx, curXPosScaled + 10 * dx, curXPosScaled + 9 * dx,  curXPosScaled + 8 * dx,
                curXPosScaled + 7 * dx,  curXPosScaled + 6 * dx,  curXPosScaled + 5 * dx,  curXPosScaled + 4 * dx,
                curXPosScaled + 3 * dx,  curXPosScaled + 2 * dx,  curXPosScaled + dx,      curXPosScaled);
            __m512 curShiftIm = _mm512_set1_ps(curYPosScaled);                         

            __m512 curComplexRe = curShiftRe;
            __m512 curComplexIm = curShiftIm;

            __m512i exitIndexes = _mm512_set1_epi32(0);

            if(WIDTH_WINDOW - counterX > UNWRAP_NUMBER){
                countDotsVectorMb(&curComplexRe, &curComplexIm, curShiftRe, curComplexIm, &exitIndexes);
                
                // drawMb(exitIndexes, videoMemBuffer, counterX, counterY);

            }
        }
    }
    // txUnlock();
}

inline void countDotsVectorMb(__m512*  curComplexRe, __m512* curComplexIm, 
                              __m512   curShiftRe,   __m512 curShiftIm,
                              __m512i* exitIndexes){
    assert(curComplexRe);
    assert(curComplexIm);
    assert(exitIndexes);

    int curIter = 0;
    while(curIter < AMOUNT_ITERATIONS){
        // if(curIter == 0) debugPrintV512(curComplexRe,     "curComplexRe");
        // if(curIter == 0) debugPrintV512(curComplexIm,     "curComplexIm");
        
        __m512 curComplexReSquare = _mm512_mul_ps(*curComplexRe, *curComplexRe);
        __m512 curComplexImSquare = _mm512_mul_ps(*curComplexIm, *curComplexIm);
        __m512 curComplexImRe     = _mm512_mul_ps(*curComplexRe, *curComplexIm);

        __m512 curComplexSquare   = _mm512_add_ps(curComplexReSquare, curComplexImSquare);

        // if(curIter == 0) debugPrintV512(curComplexReSquare, "curComplexReSquare");
        // if(curIter == 0) debugPrintV512(curComplexImSquare, "curComplexImSquare");
        // if(curIter == 0) debugPrintV512(curComplexImRe,     "curComplexImRe");
        // if(curIter == 0) debugPrintV512(curComplexSquare,   "curComplexSquare");

        __m512 unwNumber        =  _mm512_set1_ps(MAX_COMPLEX_NUM_SIZE);
        __mmask16 mask          = _mm512_mask_cmp_ps_mask(0xFFFF, curComplexSquare, unwNumber, _CMP_LT_OS);

        __m512i ones = _mm512_set1_epi32(1);
        *exitIndexes = _mm512_add_epi32(*exitIndexes, _mm512_maskz_mov_epi32(mask, ones));
        
        if (_kortestz_mask16_u8(mask, mask)) break;

        // if(curIter == 0) debugPrintMask16(mask, "Mask");

        __m512 newRe = _mm512_sub_ps(curComplexReSquare, curComplexImSquare);
        newRe = _mm512_add_ps(newRe, curShiftRe);

        __m512 newIm = _mm512_add_ps(curComplexImRe, curComplexImRe);
        newIm = _mm512_add_ps(newIm, curShiftIm);

        *curComplexRe = _mm512_mask_mov_ps(*curComplexRe, mask, newRe);
        *curComplexIm = _mm512_mask_mov_ps(*curComplexIm, mask, newIm);

        // if(curIter == 0) debugPrintV512(curComplexRe,     "curComplexRe");
        // if(curIter == 0) debugPrintV512(curComplexIm,     "curComplexIm");
        
        // if(curIter == 0) lprintf("---------------------------------------------\n");
        curIter++;

        asm volatile("" :: "v"(*exitIndexes));   // to fool compiler and not allow him remove calculations
    }
}

// inline void drawMb(__m512i exitIndexes, RGBQUAD* videoMemBuffer, int counterX, int counterY){
    // __m512i vPixel = convertDataForDraw(exitIndexes);
    // saveDataForDraw(vPixel, videoMemBuffer, counterX, counterY);
// }

// inline __m512i convertDataForDraw(__m512i exitIndexes){
    // __m512i v_red   = _mm512_and_epi32(_mm512_mullo_epi32(exitIndexes, _mm512_set1_epi32(5)),  _mm512_set1_epi32(255));
    // __m512i v_green = _mm512_and_epi32(_mm512_mullo_epi32(exitIndexes, _mm512_set1_epi32(9)),  _mm512_set1_epi32(255));
    // __m512i v_blue  = _mm512_and_epi32(_mm512_mullo_epi32(exitIndexes, _mm512_set1_epi32(13)), _mm512_set1_epi32(255));

    // __m512i v_pixel = _mm512_or_si512(_mm512_slli_epi32(v_red, 16), _mm512_or_si512(_mm512_slli_epi32(v_green, 8), v_blue));

    // return v_pixel;
// }

// inline void saveDataForDraw(__m512i vPixel, RGBQUAD* videoMemBuffer,
//                             int counterX, int counterY){
//     assert(videoMemBuffer);

    // void* destAddr = &videoMemBuffer[counterX + (-(counterY) + HEIGHT_WINDOW - 1) * WIDTH_WINDOW];

    // _mm512_store_si512((__m512i*)destAddr, vPixel);
// }

// void showFps(float fps){

//     char printStr[15];
//     sprintf(printStr, "fps: %.2f", fps);
//     txSetColor(TX_YELLOW); 
//     txTextOut(0, 0, printStr);
// }

inline void basicVersionCalculations(){
    for(int curX = 0; curX < (int) WIDTH_WINDOW; curX++){
        for(int curY = 0; curY < (int) HEIGHT_WINDOW; curY++){
            float curComplexRe         = 0;
            float curComplexIm         = 0;

            double curComplexRe2       = 0;     // overflow if float        
            double curComplexIm2       = 0;     // overflow if float
            float curComplexImRe       = 0;

            float curShiftRe           = (curX - 1000) / (450.0); 
            float curShiftIm           = (curY - 300) / (450.0); 

            int curIter = 0;
            while(curComplexRe2 + curComplexIm2 < 4 && curIter < AMOUNT_ITERATIONS){
                curComplexRe2        = curComplexRe * curComplexRe;
                curComplexIm2        = curComplexIm * curComplexIm;
                curComplexImRe       = curComplexIm * curComplexRe;

                curComplexRe = (curComplexRe2 - curComplexIm2 + curShiftRe);
                curComplexIm = 2 * curComplexImRe + curShiftIm;

                curIter++;

                asm volatile("" :: ""(curIter)); 
            }
                        
        }
    }
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

static inline int64_t GetTicks(){
    LARGE_INTEGER ticks;
    if (!QueryPerformanceCounter(&ticks)){
        printf("error");
    }
    return ticks.QuadPart;
}

static inline int64_t GetFrequency(){
    LARGE_INTEGER frequency;
    if (!QueryPerformanceFrequency(&frequency)){
        printf("error");
    }
    return frequency.QuadPart;
}