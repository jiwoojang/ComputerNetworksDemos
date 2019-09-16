#include <stdio.h> 
#include <stdlib.h>
#include <time.h> 
#include <math.h>

// Generates and prints 1000 random numbers in an exponential distribution with lambda=75
int main(void) {
    int lambda = 75;

    // Initialize/seed the random number generator with the system time
    // Not a good idea from a security perspective but acceptable for simulations
    srand(time(0));

    for (int i=0; i<1000; i++) {
        // Generate and print the random value
        // rand() generates uniformly from 0 to RAND_MAX
        double uniformRandomVariable = 1.0f * rand() / RAND_MAX;

        // Transform to exponential distribution
        double randVal = -(1.0f / lambda) * log(1.0 - uniformRandomVariable);
        printf("%f\n", randVal);
    }
    return 0;
}