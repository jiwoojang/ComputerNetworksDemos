#include <iostream>
#include <cmath>

#include "NodeEventQueue.hpp"
#include "Event.hpp"

// Initialize the random number generator once for all instances of the class
RandomNumberGenerator NodeEventQueue::numGen;

NodeEventQueue::NodeEventQueue(double newLambda, double newR)
{
    lambda = newLambda;
    R = newR;

    collisionCounter = 0;
    totalCollisions = 0;
    successfulTransmissions = 0; 
    totalTransmissions = 0;
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

    while(time < simulationTime)   
    {
        // Increase simulation time by random interarrival time
        double randomInterArrivalTime = numGen.GenerateRandomPoissonValue(lambda);
        time += randomInterArrivalTime;

        // Create and push an arrival event
        Event arrivalEvent(Event::EventType::Arrival, time);
        eventList.push_back(arrivalEvent);
    }

    // Sort all events by time stamp
    eventList.sort(locSortEventByTime);
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
}

bool NodeEventQueue::WillCollideWithTransmission(double transTime, int distance)
{
    // The next packet up for transmission
    double nextPacketTime = eventList.front().GetProcessTime();

    // Check for collision
    //std::cout << nextPacketTime << "," << (transTime + (distance*propDelay)) << std::endl;
    return (nextPacketTime < (transTime + (distance*propDelay)));
}

bool NodeEventQueue::WillBusyWait(double transTime, int distance)
{
    // The next packet up for transmission
    double nextPacketTime = eventList.front().GetProcessTime();

    return (nextPacketTime > transTime + (distance*propDelay)) && (nextPacketTime < (transTime + (distance*propDelay) + transDelay) );
}

// transtime is collision time is using greedy method
void NodeEventQueue::ApplyExponentialBackOff(double transTime)
{
    // TODO: Keep an eye on the performance of pow here, if its really gross we should just pre multiply it
    double randomMultiplier = numGen.GenerateRandomNumberInRange(0, pow(2,collisionCounter) - 1.0f);
    
    // TODO: Should this include tProp?
    // jruhland: don't think so
    double waitTime = randomMultiplier * 512.0f/R + transTime;

    for (Event packetArrival : eventList)
    {
        if (packetArrival.GetProcessTime() < waitTime)
        {
            packetArrival.SetProcessTime(waitTime);
        }
        else 
        {
            // Otherwise we always iterate through the entire list
            break;
        }
    }
}

void NodeEventQueue::ApplyBusyWait(double transTime, int distance)
{
    double waitTime = transTime + (distance*propDelay) + transDelay;

    for (Event packetArrival : eventList)
    {
        if (packetArrival.GetProcessTime() < waitTime)
        {
            packetArrival.SetProcessTime(waitTime);
        }
    }
}

void NodeEventQueue::TransmitPacketSuccessfully()
{
    ++totalTransmissions;

    // Success!
    ++successfulTransmissions;

    // Transmit the packet
    eventList.pop_front();
}

void NodeEventQueue::TransmitPacketWithCollision()
{
    ++totalTransmissions;
    ++totalCollisions;
    ++collisionCounter;

    if (collisionCounter > 10)
    {
        // Drop the packet
        eventList.pop_front();

        // Reset counter
        collisionCounter = 0;
    }
}

NodeEventQueue::NodeResult NodeEventQueue::GetPerformanceStats() {
    std::cout << "Transmissions, Successes, Collisions" << endl;
    std::cout << totalTransmissions << "," << successfulTransmissions << "," << totalCollisions << std::endl;
    return {totalTransmissions, successfulTransmissions, totalCollisions};
}
