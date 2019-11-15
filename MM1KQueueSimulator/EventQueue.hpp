#pragma once
#include <list>

#include "Event.hpp"
#include "RandomNumberGenerator.hpp"

class EventQueue 
{
public:
    virtual ~EventQueue();

    struct ProcessResults
    {
        int numArrivals = 0; 
        int numDepartures = 0;
        int numObservations = 0;
        int idleCount = 0;
        int numDropped = 0;
        int queueSum = 0;

        float GetAveragePacketsInQueue() const { return queueSum / (float)numObservations; }
        float GetIdleTimePercent() const { return 100.0f * idleCount / (float)numObservations; }
        float GetDroppedPacketPercent() const {return 100.0f * numDropped / (float)numArrivals; }
    };

    void InitalizeQueue(double simulationTime, bool generateDepartures=true);
    const list<Event>& GetEventList() { return eventList; }
    virtual void ProcessQueue();

protected:
    double lambda;
    int L;
    double alpha;
    int C;
    int K; // Queue Size

    RandomNumberGenerator numGen;
    list<Event> eventList;
};