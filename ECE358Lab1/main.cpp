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

    cout << "//////////////////////////////////////////////////" << endl;
    cout << "//                                              //" << endl;
    cout << "//                  QUESTION 1                  //" << endl;
    cout << "//                                              //" << endl;
    cout << "//////////////////////////////////////////////////" << endl;
    cout << endl;
    
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
    
    cout << "//////////////////////////////////////////////////" << endl;
    cout << "//                                              //" << endl;
    cout << "//                  QUESTION 3                  //" << endl;
    cout << "//                                              //" << endl;
    cout << "//////////////////////////////////////////////////" << endl;
    cout << endl;
    
    for (float i = 0.25f; i < 1.05f; i += 0.1f)
    {
        cout << "Rho: " << i << endl;
        double lambda = i * LINKRATE / MEANPACKETLENGTH;

        MM1Queue queue(lambda, MEANPACKETLENGTH, lambda * 5, LINKRATE);
        queue.InitalizeQueue(1000.0f);
        queue.ProcessQueue();
    }
}

void Question4() {

    cout << "//////////////////////////////////////////////////" << endl;
    cout << "//                                              //" << endl;
    cout << "//                  QUESTION 4                  //" << endl;
    cout << "//                                              //" << endl;
    cout << "//////////////////////////////////////////////////" << endl;
    cout << endl;
    
    double lambda =  1.2f * LINKRATE / MEANPACKETLENGTH;

    MM1Queue queue(lambda, MEANPACKETLENGTH, lambda * 5, LINKRATE);
    queue.InitalizeQueue(1000.0f);
    queue.ProcessQueue();
}

void Question6GraphAnalysis(int K, float startRho, float endRho, float stepSize)
{
    for (float i = startRho; i < (endRho + stepSize); i += stepSize)
    {
        cout << "Rho: " << i << endl;
        double lambda = i * LINKRATE / MEANPACKETLENGTH;

        MM1KQueue queue(lambda, MEANPACKETLENGTH, lambda * 5, LINKRATE, K);
        queue.InitalizeQueue(1000.0f, false);
        queue.ProcessQueue();
    }
}

void Question6()
{
    cout << "//////////////////////////////////////////////////" << endl;
    cout << "//                                              //" << endl;
    cout << "//                  QUESTION 6                  //" << endl;
    cout << "//                                              //" << endl;
    cout << "//////////////////////////////////////////////////" << endl;
    cout << endl;

    cout << "//////////////////////////////////////////////////" << endl;
    cout << "//                E[N] Analysis                 //" << endl;
    cout << "//////////////////////////////////////////////////" << endl;
    cout << endl;

    Question6GraphAnalysis(10, 0.5f, 1.5f, 0.1f);
    Question6GraphAnalysis(25, 0.5f, 1.5f, 0.1f);
    Question6GraphAnalysis(50, 0.5f, 1.5f, 0.1f);

    cout << "//////////////////////////////////////////////////" << endl;
    cout << "//                PLoss Analysis                //" << endl;
    cout << "//////////////////////////////////////////////////" << endl;
    cout << endl;

    Question6GraphAnalysis(10, 0.4f, 2.0f, 0.1f);
    Question6GraphAnalysis(25, 0.4f, 2.0f, 0.1f);
    Question6GraphAnalysis(50, 0.4f, 2.0f, 0.1f);

    Question6GraphAnalysis(10, 2.0f, 5.0f, 0.2f);
    Question6GraphAnalysis(25, 2.0f, 5.0f, 0.2f);
    Question6GraphAnalysis(50, 2.0f, 5.0f, 0.2f);

    Question6GraphAnalysis(10, 5.0f, 10.0f, 0.4f);
    Question6GraphAnalysis(25, 5.0f, 10.0f, 0.4f);
    Question6GraphAnalysis(50, 5.0f, 10.0f, 0.4f);
}

int main() {
    Question1();
    Question3();
    Question4();
    Question6();
}