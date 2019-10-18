#pragma once
#include <list>

#include "Event.hpp"
#include "RandomNumberGenerator.hpp"

class NodeEventQueue 
{
public:

    NodeEventQueue(double newLambda, double newR);
    ~NodeEventQueue();

    struct NodeResult {
        int transmissions;
        int successes;
        int collisions;
    };
    
    void InitializeQueue(double simulationTime, double newPropDelay, double newTransDelay);
    bool WillCollideWithTransmission(double transTime, int distance);
    bool WillBusyWait(double transTime, int distance);

    double GetNextEventTime();
    int GetQueueSize();
    void PopEvent();
    
    void ApplyExponentialBackOff(double transTime);
    void ApplyBusyWait(double transTime, int distance);

    void TransmitPacketSuccessfully();
    void TransmitPacketWithCollision();

    NodeResult GetPerformanceStats();
    
    static RandomNumberGenerator numGen;
private:
    double lambda;
    double R; 

    double propDelay;
    double transDelay;

    int collisionCounter;

    int totalCollisions;
    int successfulTransmissions; 
    int totalTransmissions;

    list<Event> eventList;
};