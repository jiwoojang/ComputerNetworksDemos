#pragma once
#include <cmath>
#include <random>

class RandomNumberGenerator
{
public:
    RandomNumberGenerator();
    ~RandomNumberGenerator() {};

    // Generates a random value according to the poisson distribution
    double GenerateRandomValue(double lambda);

private:
    std::random_device rd;
    std::mt19937 generator;
    std::uniform_real_distribution<double> dis;
};