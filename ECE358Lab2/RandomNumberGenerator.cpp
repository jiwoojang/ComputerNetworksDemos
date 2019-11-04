#include "RandomNumberGenerator.hpp"

RandomNumberGenerator::RandomNumberGenerator()
{
    // Seed the generator
    generator.seed(1);

    // Initialize the uniform distribution
    dis = std::uniform_real_distribution<double>(0, 1.0f);
}

double RandomNumberGenerator::GenerateRandomPoissonValue(double lambda)
{
    // Generate a uniform random variable
    double uniformRandomVariable = dis(generator);

    // Crunch the inverse function for a poisson distribution
    return -(1.0f / lambda) * log(1.0 - uniformRandomVariable);
}

double RandomNumberGenerator::GenerateRandomNumberInRange(double min, double max)
{
    // Generate a uniform random variable
    double uniformRandomVariable = dis(generator);

    return min + uniformRandomVariable*(max - min);
}
