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
        //std::cout << t << std::endl;
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
    return (nextPacketTime > transTime) && (nextPacketTime < (transTime + (distance*propDelay)));
}

bool NodeEventQueue::WillDetectBusBusy(double transTime, int distance)
{
    // The next packet up for transmission
    double nextPacketTime = eventList.front().GetProcessTime();
    double signalArivalTime = transTime + (distance*propDelay);
    
    return (nextPacketTime >= signalArivalTime) && (nextPacketTime < (signalArivalTime + transDelay));
}

void NodeEventQueue::DiscardPacket() {
        PopEvent();

        // Reset counters
        ResetCollisionCounter();
        ResetBusyBackOffCounter();
}

// transtime is collision time is using greedy method
void NodeEventQueue::ApplyExponentialBackOff(double transTime)
{
    ++collisionCounter;
    
    if (collisionCounter > 10)
    {
        DiscardPacket();
        ++numDropped;
        return;
    }

    //std::cout << "Collision at " << std::setprecision(10) << eventList.front().GetProcessTime() << std::endl; ;

    double randomMultiplier = round(numGen.GenerateRandomNumberInRange(0, pow(2,collisionCounter) - 1.0f));
    double waitTime = (randomMultiplier * 512.0f/R) + transTime; // + transDelay; //J: don't think we use transdelay here?

    //std::cout << "Backing off to " << waitTime << std::endl;

     for (Event& packetArrival : eventList)
    {
        if (packetArrival.GetProcessTime() < waitTime)
        {
            packetArrival.SetProcessTime(waitTime);
        }
        else 
        {
            break;
        }
    }
}

void NodeEventQueue::ApplyBusyWait(double transTime, int distance)
{
    double waitTime = (transTime + (distance*propDelay));

    //std::cout << "Busy waiting all packets in this node to " << waitTime << endl; 

    for (Event& packetArrival : eventList)
    {   
        // Events are stored in order
        if (packetArrival.GetProcessTime() < waitTime)
        {
            packetArrival.SetProcessTime(waitTime);
        }
        else {
            break;
        }
    }
}

void NodeEventQueue::ApplyBusyExponentialBackOff()
{
    ++busyBackOffCounter;

    if (busyBackOffCounter > 10)
    {
        DiscardPacket();
        ++numDropped;
        return;
    }

    double randomMultiplier = numGen.GenerateRandomNumberInRange(0, pow(2,busyBackOffCounter) - 1.0f);
    double waitTime = (randomMultiplier * 512.0f/R) + eventList.front().GetProcessTime();

    // 2nd and latest packets can at ealiest be sent Ttrans after 1st
    bool setFirst = false;
    for (Event& packetArrival : eventList)
    {
        if (packetArrival.GetProcessTime() < waitTime && !setFirst)
        {
            packetArrival.SetProcessTime(waitTime);
            setFirst = true;
        }
        else if (packetArrival.GetProcessTime() < waitTime + transDelay) {
            packetArrival.SetProcessTime(waitTime + transDelay);
        } 
        else 
        {
            break;
        }
    }
}

void NodeEventQueue::TransmitPacketSuccessfully()
{
    if (!eventList.empty())
    {
        // Success!
        ++successfulTransmissions;

        double transTime = eventList.front().GetProcessTime();
        //std::cout << "Sent packet at " << std::setprecision(10) << eventList.front().GetProcessTime() << "," << collisionCounter << "," << GetQueueSize() <<  std::endl;
        DiscardPacket();

        // update other packets on node to not send until current node is done
        for (Event& packetArrival : eventList) 
        {
            if (packetArrival.GetProcessTime() < transTime + transDelay)
            {
                packetArrival.SetProcessTime(transTime + transDelay);
            }
            else 
            {
                break;
            }
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
