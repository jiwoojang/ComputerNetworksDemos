#pragma once
#include <queue>

#include "Event.hpp"
#include "RandomNumberGenerator.hpp"

class MM1Queue
{
public:
    MM1Queue() {};
    ~MM1Queue() {};

    double GetRandomNumber();
    
private:
    RandomNumberGenerator numGen;
    std::queue<Event> queue;
};