#pragma once
#include <list>

#include "Event.hpp"
#include "RandomNumberGenerator.hpp"

class NodeEventQueue 
{
public:

    NodeEventQueue(double newLambda, double newR);
    ~NodeEventQueue();
    
    void InitializeQueue(double simulationTime);
    bool WillCollideWithTransmission(double transTime, double propDelay, double transDelay);
    bool WillBusyWait(double transTime, double propDelay, double transDelay);

    double GetNextEventTime();
    int GetQueueSize();
    void PopEvent();
    
    void ApplyExponentialBackOff(double transTime, double propDelay, double transDelay);
    void ApplyBusyWait(double transTime, double propDelay, double transDelay);

    void TransmitPacketSuccessfully();
    void TransmitPacketWithCollision();
    
    static RandomNumberGenerator numGen;
private:
    double lambda;
    double R; 

    int collisions;
    int successfulTransmissions; 
    int totalTransmissions;

    list<Event> eventList;
};