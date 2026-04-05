#include "../general/file.h"
#include "../general/debug.h"
#include "measuresHandler.h"

#include <math.h>
#include <assert.h>

const char* TEST_RESULT_FILE_NAME = "test.txt";

// count

int main(void){
    fileDescription testResult{
        TEST_RESULT_FILE_NAME,
        "rb"
    };

    data_t results = {};

    parseStringsFile(&results, TEST_RESULT_FILE_NAME);

    size_t fileSize = getFileSize(TEST_RESULT_FILE_NAME);

    double* buffer = (double*) calloc(1, fileSize);

    for(size_t curString = 0; curString < results.nStrings; curString++){
        printf("%d %lu\n", results.nStrings, curString);
        printf(" %s\n", results.strings[curString].ptr);
        if(sscanf(results.strings[curString].ptr, "%*lf %lf", &buffer[curString]) == 0){
            break;
        }
    }

    size_t amountMeasures = results.nStrings;

    printf("amountMeasures: %lu\n", amountMeasures);

    for(size_t curInd = 0; curInd < amountMeasures; curInd++){
        printf("%lu %.2lf\n", curInd + 1, buffer[curInd]);
    }

    double avgMeasures = countAverage(buffer, 2, amountMeasures);

    printf("Среднее: %.2lf\n", avgMeasures);

    double avgInfelicity = 0;

    double* infelicities = countInfelicity(avgMeasures, buffer, amountMeasures, &avgInfelicity);
    createCsvWithResults(buffer, infelicities, avgMeasures, avgInfelicity, amountMeasures);

    free(buffer);
    free(infelicities);
}