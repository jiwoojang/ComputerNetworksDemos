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
    EventQueue::ProcessResults results;

    for (Event event : eventList)
    {
        switch(event.GetEventType())
        {
            case Event::EventType::Arrival:
            {
                ++results.numArrivals;
                break;
            }
            case Event::EventType::Departure:
            {
                ++results.numDepartures;
                break;
            }
            case Event::EventType::Observer:
            {
                ++results.numObservations;
                
                if (results.numArrivals > results.numDepartures)
                {
                    results.queueSum += results.numArrivals - results.numDepartures;
                }
                else 
                {
                    ++results.idleCount;
                }

                break;
            }
            default:
                break;
        }
    }

    cout << "E[N]: " << results.GetAveragePacketsInQueue() << endl;
    cout << "P idle: " << results.GetIdleTimePercent() << endl;
}