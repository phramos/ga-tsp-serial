#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#define TOTAL_CITIES 11
#define MAX_DISTANCE 15


struct timespec begin, end;

long distances[TOTAL_CITIES][TOTAL_CITIES];

//Generate random number
long randomAtMost(long max) {
    unsigned long
    // max <= RAND_MAX < ULONG_MAX, so this is okay.
            num_bins = (unsigned long) max + 1,
            num_rand = (unsigned long) RAND_MAX + 1,
            bin_size = num_rand / num_bins,
            defect   = num_rand % num_bins;

    long x;
    do {
        x = random();
    }
        // This is carefully written not to overflow
    while (num_rand - defect <= (unsigned long)x);

    // Truncated division is intentional
    return x/bin_size;
}

void initializaDistances() {
    for (int i = 0; i < TOTAL_CITIES; ++i) {
        distances[i][i] = 0;
        for (int j = i+1; j < TOTAL_CITIES ; ++j) {
            long distance = randomAtMost(MAX_DISTANCE);
            distances[i][j] = distance;
            distances[j][i] = distance;
        }
    }
}

void printMatrix(long matrix[]) {
    printf("\n");
    for (int k = 0; k < TOTAL_CITIES; ++k) {
//        printf("\t\t");
        printf("|%d|",k);
        printf("\t\t");
    }
    for (int i = 0; i < TOTAL_CITIES; ++i) {
//        printf("\n|%d|",i);
//        printf("\t\t");
        printf("\n");
        for (int j = 0; j < TOTAL_CITIES; ++j) {
            printf("%d",distances[i][j]);
            printf("\t\t");
        }
        printf("|%d|",i);


    }
}

int main() {
    srand(time(NULL));
    clock_gettime(CLOCK_MONOTONIC, &begin);
    //Codigo vai aqui

    initializaDistances();
    printMatrix(distances);
    //TEMPO eh calculado daqui pra baixo
    clock_gettime(CLOCK_MONOTONIC, &end);

    double timeSpent = end.tv_sec - begin.tv_sec;
    timeSpent += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
    printf("\nTime spent: %.10lf seconds.\n", timeSpent);
    return 0;
}


