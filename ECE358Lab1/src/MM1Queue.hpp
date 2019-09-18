#pragma once
#include <list>

#include "Event.hpp"
#include "RandomNumberGenerator.hpp"

using namespace std;

class MM1Queue
{
public:
    MM1Queue(double newLambda, int newL, double newAlpha, int newC);
    ~MM1Queue();

    void InitalizeQueue(double simulationTime);
    const list<Event>& GetEventList() { return eventList; }
    void ProcessQueue();

private:
    double lambda;
    int L;
    double alpha;
    int C;

    RandomNumberGenerator numGen;
    list<Event> eventList;
};