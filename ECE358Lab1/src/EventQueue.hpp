#pragma once
#include <list>

#include "Event.hpp"
#include "RandomNumberGenerator.hpp"

class EventQueue {
public:
    virtual ~EventQueue();

    void InitalizeQueue(double simulationTime, bool generateDepartures=true);
    const list<Event>& GetEventList() { return eventList; }
    virtual void ProcessQueue();

protected:
    double lambda;
    int L;
    double alpha;
    int C;
    int queueSize;

    RandomNumberGenerator numGen;
    list<Event> eventList;
};