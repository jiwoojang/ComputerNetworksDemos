#include "RandomNumberGenerator.hpp"

RandomNumberGenerator::RandomNumberGenerator()
{
    // Seed the generator
    generator.seed(rd());

    // Initialize the uniform distribution
    dis = std::uniform_real_distribution<double>(0, 1.0f);
}

double RandomNumberGenerator::GenerateRandomValue(double lambda)
{
    // Generate a uniform random variable
    double uniformRandomVariable = dis(generator);

    // Crunch the inverse functionf for a poisson distribution
    return -(1.0f / lambda) * log(1.0 - uniformRandomVariable);
}