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
    double GenerateRandomValue(double lambda);

private:
    random_device rd;
    mt19937 generator;
    uniform_real_distribution<double> dis;
};