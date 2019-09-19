#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#include "MM1Queue.hpp"
#include "MM1KQueue.hpp"

using namespace std;

#define MEANPACKETLENGTH 2000
#define LINKRATE 1000000

void Question1() {

    int lambda = 75;

    // Initialize/seed the random number generator with the system time
    // Not a good idea from a security perspective but acceptable for simulations
    srand(time(0));

    for (int i=0; i<1000; i++) {
        // Generate and print the random value
        // rand() generates uniformly from 0 to RAND_MAX
        double uniformRandomVariable = 1.0f * rand() / RAND_MAX;

        // Transform to exponential distribution
        double randVal = -(1.0f / lambda) * log(1.0f - uniformRandomVariable);
        printf("%f\n", randVal);
    }
}

void Question3() {
    for (float i = 0.25f; i < 1.05f; i += 0.1f)
    {
        cout << "rho: " << i << endl;
        double lambda = i * LINKRATE / MEANPACKETLENGTH;

        MM1Queue queue(lambda, MEANPACKETLENGTH, lambda * 5, LINKRATE);
        queue.InitalizeQueue(1000.0f);
        queue.ProcessQueue();
    }
}

void Question4() {
    double lambda =  1.2f * LINKRATE / MEANPACKETLENGTH;

    MM1Queue queue(lambda, MEANPACKETLENGTH, lambda * 5, LINKRATE);
    queue.InitalizeQueue(1000.0f);
    queue.ProcessQueue();
}

void MM1KTest() {
    int K = 10;
    for (float i = 0.5f; i < 1.6f; i += 0.1f)
    {
        cout << "rho: " << i << endl;
        double lambda = i * LINKRATE / MEANPACKETLENGTH;

        MM1KQueue queue(lambda, MEANPACKETLENGTH, lambda * 5, LINKRATE, K);
        queue.InitalizeQueue(1000.0f, false);
        queue.ProcessQueue();
    }
}

int main() {
    MM1KTest();
}