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
}

NodeEventQueue::~NodeEventQueue() {
    eventList.clear();
}

// Utility function used to sort events by time
bool locSortEventByTime(const Event& firstEvent, const Event& secondEvent)
{
    return firstEvent.GetProcessTime() < secondEvent.GetProcessTime();
}

void NodeEventQueue::InitializeQueue(double simulationTime) {
    double time = 0.0f;

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

double NodeEventQueue::GetNextEventTime() {
    return eventList.front().GetProcessTime();
}

bool NodeEventQueue::WillCollideWithTransmission(double transTime, double propDelay, double transDelay)
{
    // The next packet up for transmission
    double nextPacketTime = eventList.front().GetProcessTime();

    // Check for collision
    return nextPacketTime < (transTime + propDelay);
}

bool NodeEventQueue::WillBusyWait(double transTime, double propDelay, double transDelay)
{
    // The next packet up for transmission
    double nextPacketTime = eventList.front().GetProcessTime();

    return (nextPacketTime > transTime + propDelay) && (nextPacketTime < transTime + propDelay + transDelay);
}

void NodeEventQueue::ApplyExponentialBackOff(double transTime, double propDelay, double transDelay)
{
    // TODO: Keep an eye on the performance of pow here, if its really gross we should just pre multiply it
    double randomMultiplier = numGen.GenerateRandomNumberInRange(0, pow(2,collisions) - 1.0f);
    
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

void NodeEventQueue::ApplyBusyWait(double transTime, double propDelay, double transDelay)
{
    double waitTime = transTime + propDelay + transDelay;

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
    ++collisions;

    if (collisions > 10)
    {
        // Drop the packet
        eventList.pop_front();

        // Reset counter
        collisions = 0;
    }
}
