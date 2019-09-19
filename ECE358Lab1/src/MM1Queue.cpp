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

void MM1Queue::ProcessQueue()
{
    // TODO break this out into a common struct in the EventQueue class
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