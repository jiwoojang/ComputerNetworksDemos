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

    void DiscardPacket();
    void PopEvent();

    void ResetBusyBackOffCounter();
    void ResetCollisionCounter();
    
    void ApplyExponentialBackOff(double transTime);
    void ApplyBusyWait(double transTime, int distance);
    void ApplyBusyExponentialBackOff(double transTime, int distance);

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
    int busyBackOffCounter;

    int totalCollisions;
    int successfulTransmissions; 
    int numPackets;
    int numDropped;

    list<Event> eventList;
};