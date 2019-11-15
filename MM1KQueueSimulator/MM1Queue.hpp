#pragma once
#include <list>

#include "Event.hpp"
#include "EventQueue.hpp"
#include "RandomNumberGenerator.hpp"

using namespace std;

class MM1Queue : public EventQueue
{
public:
    MM1Queue(double newLambda, int newL, double newAlpha, int newC);
    void ProcessQueue() override;
};