#include "measuresHandler.h"
#include "../general/file.h"

#include <assert.h>
#include <stddef.h>
#include <math.h>
#include <stdio.h>

const char* CSV_RESULTS_TABLE_FILE_NAME = "tableRes.csv";

double countAverage(double* measures, int accuracy, size_t amountMeasures){
    assert(measures);

    double sum = 0;
    for(size_t curMeasurement = 0; curMeasurement < amountMeasures; curMeasurement++){
        sum += measures[curMeasurement];
    }

    double avg = sum / amountMeasures;
    avg = roundTo(avg, 2);

    return avg;
}

double roundTo(double toRoundNum, int accuracy){
    return round(toRoundNum * pow(10, accuracy)) / pow(10, accuracy);
}

double* countInfelicity(double avg, double* measures, size_t amountMeasures, double* avgInfelicity){
    assert(measures);

    double* infelicities = (double*) calloc(amountMeasures, sizeof(double));
    assert(infelicities);

    double sumInfelicities = 0;
    for(size_t curMeasurement = 0; curMeasurement < amountMeasures; curMeasurement++){
        infelicities[curMeasurement] = fabs(avg - measures[curMeasurement]);
        sumInfelicities += fabs(avg - measures[curMeasurement]);
    }

    *avgInfelicity = sumInfelicities / amountMeasures;
    
    // for(size_t curInd = 0; curInd < amountMeasures; curInd++){
    //     printf("%lu %.2lf\n", curInd + 1, infelicities[curInd]);
    // }

    // printf("Средняя погрешность: %.2lf\n", *avgInfelicity);

    return infelicities;
}

void createCsvWithResults(double* measures, double* infelicities,
         double avgMeasures, double avgInfelicity, size_t amountMeasures){
    assert(measures);
    assert(infelicities);

    fileDescription CSVresults = {
        CSV_RESULTS_TABLE_FILE_NAME,
        "wb"
    };

    FILE* csvResPtr = myOpenFile(&CSVresults);
    fprintf(csvResPtr, "testN,value,infelicity\n");
    for(size_t curInd = 0; curInd < amountMeasures; curInd++){
        fprintf(csvResPtr, "%lu,%.2lf,%.2lf\n", curInd + 1, measures[curInd], infelicities[curInd]);
    }

    fprintf(csvResPtr, "Average: %lu,%.2lf,%.2lf\n", amountMeasures + 1, avgMeasures, avgInfelicity);

    fclose(csvResPtr);
}