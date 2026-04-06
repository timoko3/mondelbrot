#include <stddef.h>

double countAverage(double* measures, int accuracy, size_t amountMeasures);
double roundTo(double toRoundNum, int accuracy);
double* countInfelicity(double avg, double* measures, size_t amountMeasures, double* avgInfelicity);
void createCsvWithResults(double* measures, double* infelicities,
         double avgMeasures, double avgInfelicity, size_t amountMeasures);