#pragma once
#include <list>

#include "Event.hpp"
#include "RandomNumberGenerator.hpp"

class NodeEventQueue 
{
public:

    NodeEventQueue(double newLambda, double newR);
    NodeEventQueue();
    ~NodeEventQueue();

    struct NodeResult {
        int packets;
        int successes;
        int collisions;
        int dropped;
    };
    
    void InitializeQueue(double simulationTime, double newPropDelay, double newTransDelay);
    void InitializeQueue(double newPropDelay, double newTransDelay, std::list<double> events);
    bool WillCollideWithTransmission(double transTime, int distance);
    bool WillDetectBusBusy(double transTime, int distance);

    double GetNextEventTime();
    int GetQueueSize();

    void PopEvent();

    void ResetBusyBackOffCounter();
    void ResetCollisionCounter();
    void BufferPacketsToTime(double time);
    
    void ApplyExponentialBackOff(double timeStamp);
    void ApplyBusyWait(double timeStamp);
    void ApplyBusyExponentialBackOff(double timeStamp);

    void TransmitPacketSuccessfully(double ditherCeiling);
    void TransmitPacketWithCollision();

    NodeResult GetPerformanceStats();
    
    static RandomNumberGenerator numGen;
private:
    double lambda;
    double R; 

    double propDelay;
    double transDelay;

    int collisionCounter;
    int busyBackOffCounter;

    int totalCollisions;
    int successfulTransmissions; 
    int numPackets;
    int numDropped;

    list<Event> eventList;
};