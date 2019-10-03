#pragma once
#include <cmath>
#include <random>

using namespace std;

class RandomNumberGenerator
{
public:
    RandomNumberGenerator();
    ~RandomNumberGenerator() {};

    // Generates a random value according to the poisson distribution
    double GenerateRandomPoissonValue(double lambda);

    // Generates a random value between min and max
    double GenerateRandomNumberInRange(double min, double max);

private:
    random_device rd;
    mt19937 generator;
    uniform_real_distribution<double> dis;
};