#pragma once

#include <list>

#include "Event.hpp"
#include "EventQueue.hpp"
#include "RandomNumberGenerator.hpp"

class MM1KQueue : public EventQueue 
{
public:
    MM1KQueue(double newLambda, int newL, double newAlpha, int newC, int newK);
    void ProcessQueue() override;
};