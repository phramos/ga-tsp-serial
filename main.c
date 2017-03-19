#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#define TOTAL_CITIES 10
#define MAX_DISTANCE 15
#define POPULATION_SIZE 5
#define TOTAL_GENERATIONS 50
#define MUTATION_RATE 0.5
#define TRUE 1
#define FALSE 0

struct Sample {
    //The total distance of the path
    long fitness;
    long chromossomes[TOTAL_CITIES];
};

struct Population {
    struct Sample samples[TOTAL_CITIES];
};

struct timespec begin, end;

long distances[TOTAL_CITIES][TOTAL_CITIES];

//Generate random number from 0 to max
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

//Initialize the distances between the cities with random values
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

//Print a matrix of cities and distances
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
            printf("%ld",distances[i][j]);
            printf("\t\t");
        }
        printf("|%d|",i);


    }
}

//Return the fitness of the givem sample. The best fitness is the one closer to 0.
long calculateFitness(struct Sample * sample) {
    long totalFitness = 0;
//    printf("\nFITNESSSSSS");
//    printf("\nfit=%d",totalFitness);
    for (int i = 1; i < TOTAL_CITIES; ++i) {
//        printf("\nfit=%ld+%ld",totalFitness, distances[sample->chromossomes[i]][sample->chromossomes[i-1]]);
        totalFitness += distances[sample->chromossomes[i]][sample->chromossomes[i-1]];
    }
//    printf("\ntotalFit=%d",totalFitness);
    return totalFitness;
}


//Create a Sample with a random chromossome. This function is used to create the first population of th GA.
struct Sample createRandomSample(){
    struct Sample randomSample;
    long chromossome[TOTAL_CITIES];

    for (int i = 0; i < TOTAL_CITIES; ++i) {
        int searchCandidate = TRUE;
        long candidate;
        while (searchCandidate == TRUE) {
            candidate = randomAtMost(TOTAL_CITIES-1);
            if (i == 0) {
                randomSample.chromossomes[i] = candidate;
                searchCandidate = FALSE;
            } else {
                int safeCandidate = TRUE;
                for (int j = 0; j < i; ++j) {
                    if (randomSample.chromossomes[j] == candidate) {
                        safeCandidate = FALSE;
                    };
                }
                if (safeCandidate == TRUE) {
                    randomSample.chromossomes[i] = candidate;
                    searchCandidate = FALSE;
                }
            }
        }
    }
    randomSample.fitness = calculateFitness(&randomSample);
    return randomSample;
}

//Print a chromossome
void printSample(struct Sample *sample) {

    printf("\nPath cost: %ld", sample->fitness);
    printf("\nPath: ");
    for (int i = 0; i < TOTAL_CITIES; ++i) {
        printf("_%ld", sample->chromossomes[i]);
    }
}

struct Sample crossover(struct Sample * parent1, struct Sample * parent2) {
    struct Sample child;
    long cutPosition = randomAtMost(TOTAL_CITIES-1);

    //Ensure that cut position is different from the first and the last position of the chromossome
    while (cutPosition==0 || cutPosition==TOTAL_CITIES-1) {
        cutPosition = randomAtMost(TOTAL_CITIES-1);
    }
//    printf("\ncutPostion: %ld", cutPosition);
    //Get the part before the cutPosition from parent1
    for (int i = 0; i < cutPosition; ++i) {
        child.chromossomes[i] = parent1->chromossomes[i];
    }

    long parent2GenePos = 0;
    //Get the part after cutPosition from parent2
    for (long j = cutPosition; j < TOTAL_CITIES; ++j) {
        int foundGene = FALSE;
        while (foundGene != TRUE) {
            int isSafeParent2Gene = TRUE;
            for (int i = 0; i < cutPosition; ++i) {
                if (parent2->chromossomes[parent2GenePos] == child.chromossomes[i]){
                    isSafeParent2Gene = FALSE;
                }
            }

            if (isSafeParent2Gene == TRUE) {
                foundGene = TRUE;
            } else {
                parent2GenePos += 1;
            }
        }

        child.chromossomes[j] = parent2->chromossomes[parent2GenePos];
        parent2GenePos += 1;
    }
    child.fitness = calculateFitness(&child);
    return child;
}

void printPopulation(struct Population * population) {
    for (int i = 0; i < TOTAL_CITIES; ++i) {
        printSample(&population->samples[i]);
    }
}

void printBestSolution(struct Population *population) {
    struct Sample shortestPath = population->samples[0];

    for (int i = 1; i < POPULATION_SIZE; ++i) {
        if (population->samples[i].fitness < shortestPath.fitness){
            shortestPath = population->samples[i];
        }
    }

    printSample(&shortestPath);

}

int main() {
    srand(time(NULL));
    clock_gettime(CLOCK_MONOTONIC, &begin);
    //Codigo vai aqui

    initializaDistances();
    printMatrix(distances);
    /*//Testing createRandomChromossome, printChromossome, and fitness;
    struct Sample random = createRandomSample();
    printSample(&random);
    fitness(&random);*/

    /*//Testing crossover
     struct Sample parent1 = createRandomChromossome();
     struct Sample parent2 = createRandomSample();
     printf("\n Parent1");
     printChromossome(&parent1);
     printf("\n Parent2");
     printChromossome(&parent2);
     struct Sample child1 = crossover(&parent1, &parent2);
     printf("\n Child");
     printSample(&child1);*/
    //TEMPO eh calculado daqui pra baixo
    clock_gettime(CLOCK_MONOTONIC, &end);

    gaSolve();
    double timeSpent = end.tv_sec - begin.tv_sec;
    timeSpent += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
    printf("\nTime spent: %.10lf seconds.\n", timeSpent);
    return 0;
}

void gaSolve() {
    struct Population generations[TOTAL_GENERATIONS];


    //TODO: modularize
    //Initialize the first generation with random values
    for (int i = 0; i < TOTAL_CITIES; ++i) {
        generations[0].samples[i] = createRandomSample();
    }

    for (int j = 1; j < TOTAL_GENERATIONS; ++j) {
        long parent1Position = randomAtMost(POPULATION_SIZE-1);
        long parent2Position = randomAtMost(POPULATION_SIZE-1);

        //Ensure that were select 2 distint samples from the population
        while (parent1Position == parent2Position) {
            parent2Position = randomAtMost(POPULATION_SIZE-1);
        }
        struct Sample parent1 = generations[j-1].samples[parent1Position];
        struct Sample parent2 = generations[j-1].samples[parent2Position];

        struct Sample child1 = crossover(&parent1, &parent2);
        struct Sample child2 = crossover(&parent2, &parent1);

        struct Sample newGen1 = parent1;
        struct Sample newGen2 = parent2;

        //Elite
        struct Sample candidate[2];
        candidate[0] = child1;
        candidate[1] = child2;

        for (int i = 0; i < 2; ++i) {
            if (candidate[i].fitness < newGen1.fitness){
                if (newGen1.fitness < newGen2.fitness) {
                    newGen2 = newGen1;
                }
                newGen1 = candidate[i];
            } else {
                if (candidate[i].fitness < newGen2.fitness) {
                    newGen2 = candidate[i];
                }
            }
        }

        /*printf("\nParent1: ");
        printSample(&parent1);
        printf("\nParent2: ");
        printSample(&parent2);
        printf("\nChild1: ");
        printSample(&child1);
        printf("\nChild1: ");
        printSample(&child2);*/


        //Create new generation
        for (int k = 0; k < POPULATION_SIZE; ++k) {
            generations[j].samples[k] = generations[j-1].samples[k];
        }
        generations[j].samples[parent1Position] = newGen1;
        generations[j].samples[parent2Position] = newGen2;

//        printPopulation(&generations[j]);
        printf("END");
    }

    for (long l = 0; l < TOTAL_GENERATIONS; ++l) {
        printf("\n\n ----------GENERATION %ld----------", l);
        printBestSolution(&generations[l]);

    }

}


