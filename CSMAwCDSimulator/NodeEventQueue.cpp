#include <iostream>
#include <cmath>
#include <iomanip>

#include "NodeEventQueue.hpp"
#include "Event.hpp"

// Initialize the random number generator once for all instances of the class
RandomNumberGenerator NodeEventQueue::numGen;

NodeEventQueue::NodeEventQueue() {}

NodeEventQueue::NodeEventQueue(double newLambda, double newR)
{
    lambda = newLambda;
    R = newR;

    collisionCounter = 0;
    busyBackOffCounter = 0;
    totalCollisions = 0;
    successfulTransmissions = 0;
    numPackets = 0;
    numDropped = 0;
}

NodeEventQueue::~NodeEventQueue() {
    eventList.clear();
}

// Utility function used to sort events by time
bool locSortEventByTime(const Event& firstEvent, const Event& secondEvent)
{
    return firstEvent.GetProcessTime() < secondEvent.GetProcessTime();
}

void NodeEventQueue::InitializeQueue(double simulationTime, double newPropDelay, double newTransDelay) {
    double time = 0.0f;

    propDelay = newPropDelay;
    transDelay = newTransDelay;

    while(time + transDelay < simulationTime)   
    {
        // Increase simulation time by random interarrival time
        double randomInterArrivalTime = numGen.GenerateRandomPoissonValue(lambda);
        time += randomInterArrivalTime;

        // Create and push an arrival event
        Event arrivalEvent(Event::EventType::Arrival, time);
        eventList.push_back(arrivalEvent);
        numPackets++;
    }
}

void NodeEventQueue::InitializeQueue(double newPropDelay, double newTransDelay, std::list<double> arrivals) {
    propDelay = newPropDelay;
    transDelay = newTransDelay;

    collisionCounter = 0;
    busyBackOffCounter = 0;
    totalCollisions = 0;
    successfulTransmissions = 0;
    numPackets = 0;
    numDropped = 0;
    R = 1e6;

    for (double t: arrivals) {
        Event arrivalEvent(Event::EventType::Arrival, t);
        eventList.push_back(arrivalEvent);
        numPackets++;
    }
}

int NodeEventQueue::GetQueueSize() {
    return eventList.size();
}

double NodeEventQueue::GetNextEventTime() {
    if (eventList.size() > 0) {
        return eventList.front().GetProcessTime();
    }
    else {
        return -1;
    }
}

void NodeEventQueue::PopEvent() {
    if (eventList.size() > 0) {
        eventList.pop_front();
    }
    else 
    {
        return;
    }
}

void NodeEventQueue::ResetBusyBackOffCounter()
{
    busyBackOffCounter = 0;
}

void NodeEventQueue::ResetCollisionCounter()
{
    collisionCounter = 0;
}

bool NodeEventQueue::WillCollideWithTransmission(double transTime, int distance)
{
    // The next packet up for transmission
    double nextPacketTime = eventList.front().GetProcessTime();

    // Check for collision
    return (nextPacketTime < (transTime + (distance*propDelay)));
}

bool NodeEventQueue::WillDetectBusBusy(double transTime, int distance)
{
    // The next packet up for transmission
    double nextPacketTime = eventList.front().GetProcessTime();
    double signalArivalTime = transTime + (distance*propDelay);
    
    // We consider a packet that arrives EXACTLY at the time the bus is free
    // as a packet that will see the bus busy, not as a collision
    return (nextPacketTime >= signalArivalTime) && (nextPacketTime <= (signalArivalTime + transDelay));
}

void NodeEventQueue::BufferPacketsToTime(double time)
{
    eventList.front().SetProcessTime(time);
}

// transTime is collision time is using greedy method
void NodeEventQueue::ApplyExponentialBackOff(double timeStamp)
{
    ++collisionCounter;
    
    if (collisionCounter > 10)
    {
        double transmissionTime = eventList.front().GetProcessTime();

        // Drop packet
        PopEvent();
        ++numDropped;
        ResetCollisionCounter();

        if (eventList.front().GetProcessTime() < transmissionTime)
        {
            BufferPacketsToTime(transmissionTime);
        }

        return;
    }

    double randomMultiplier = round(numGen.GenerateRandomNumberInRange(0, pow(2,collisionCounter) - 1.0f));
    double waitTime = (randomMultiplier * 512.0f/R) + timeStamp + transDelay;

    BufferPacketsToTime(waitTime);
}

void NodeEventQueue::ApplyBusyWait(double timeStamp)
{
    BufferPacketsToTime(timeStamp);
}

void NodeEventQueue::ApplyBusyExponentialBackOff(double timeStamp)
{
    ++busyBackOffCounter;

    if (busyBackOffCounter > 10)
    {
        double transmissionTime = eventList.front().GetProcessTime();
        
        // Drop packet
        PopEvent();
        ++numDropped;
        ResetBusyBackOffCounter();

        if (eventList.front().GetProcessTime() < transmissionTime)
        {
            BufferPacketsToTime(transmissionTime);
        }

        return;
    }

    double randomMultiplier = round(numGen.GenerateRandomNumberInRange(0, pow(2,busyBackOffCounter) - 1.0f));
    double waitTime = (randomMultiplier * 512.0f/R) + timeStamp + transDelay;
    
    // Backoff for all relevant nodes in the queue
    BufferPacketsToTime(waitTime);
}

void NodeEventQueue::TransmitPacketSuccessfully(double ditherCeiling)
{
    if (!eventList.empty())
    {
        // Success!
        ++successfulTransmissions;

        double transmissionTime = eventList.front().GetProcessTime();

        PopEvent();

        if (eventList.front().GetProcessTime() < (transmissionTime + transDelay))
        {
            // Add random dither here so that this node doesnt always win the priority race 
            eventList.front().SetProcessTime(transmissionTime + transDelay + numGen.GenerateRandomNumberInRange(0, ditherCeiling));
        }
    }
}

void NodeEventQueue::TransmitPacketWithCollision()
{
    if (!eventList.empty())
    {
        // Record the collision
        ++totalCollisions;
    }
}

NodeEventQueue::NodeResult NodeEventQueue::GetPerformanceStats() {
    return {numPackets, successfulTransmissions, totalCollisions, numDropped};
}
