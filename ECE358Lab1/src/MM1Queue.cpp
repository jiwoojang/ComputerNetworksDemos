#include <iostream>

#include "MM1Queue.hpp"
#include "Event.hpp"

MM1Queue::MM1Queue(double newLambda, int newL, double newAlpha, int newC)
{
    lambda = newLambda;
    L = newL;
    alpha = newAlpha;
    C = newC;
}

MM1Queue::~MM1Queue()
{
    eventList.clear();
}

bool locSortEventByTime(const Event& firstEvent, const Event& secondEvent)
{
    return firstEvent.GetProcessTime() < secondEvent.GetProcessTime();
}

void MM1Queue::InitalizeQueue(double simulationTime)
{
    double time = 0.0f;

    // Populate observer events
    while(time < simulationTime)
    {
        // Increase simulation time by random interarrival time
        double randomInterArrivalTime = numGen.GenerateRandomValue(alpha);
        time += randomInterArrivalTime;

        // Create and push an observer event
        Event observerEvent(Event::EventType::Observer, time);

        eventList.push_back(observerEvent);
    }

    time = 0.0f;

    while(time < simulationTime)   
    {
        // Increase simulation time by random interarrival time
        double randomInterArrivalTime = numGen.GenerateRandomValue(lambda);
        time += randomInterArrivalTime;

        // Create and push an arrival event
        Event arrivalEvent(Event::EventType::Arrival, time);
        eventList.push_back(arrivalEvent);

        // Calculate time at which this arrival will be processed

        double packetLength = numGen.GenerateRandomValue(1.0f/L);
        double serviceTime = (packetLength / C);

        // Create and push corresponding departure event
        Event departureEvent(Event::EventType::Departure, time + serviceTime);
        eventList.push_back(departureEvent);
    }

    // Sort all events by time stamp
    eventList.sort(locSortEventByTime);
}

void MM1Queue::ProcessQueue()
{
    int numArrivals = 0; 
    int numDepartures = 0;
    int numObservations = 0;
    int idleCount = 0;
    int numPackets = 0;

    for (Event event : eventList)
    {
        switch(event.GetEventType())
        {
            case Event::EventType::Arrival:
            {
                ++numArrivals;
                break;
            }
            case Event::EventType::Departure:
            {
                ++numDepartures;
                break;
            }
            case Event::EventType::Observer:
            {
                ++numObservations;
                
                if (numArrivals > numDepartures)
                {
                    numPackets += numArrivals - numDepartures;
                }
                else 
                {
                    ++idleCount;
                }

                break;
            }
            default:
                break;
        }
    }

    cout << "E[N]: " << numPackets / (float)numObservations << endl;
    cout << "P idle: " << idleCount / (float)numObservations << endl;
}