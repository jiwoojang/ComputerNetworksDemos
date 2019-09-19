#include <iostream>

#include "EventQueue.hpp"
#include "Event.hpp"

EventQueue::~EventQueue() {
    eventList.clear();
}

void EventQueue::ProcessQueue() {}

bool locSortEventByTime(const Event& firstEvent, const Event& secondEvent)
{
    return firstEvent.GetProcessTime() < secondEvent.GetProcessTime();
}

void EventQueue::InitalizeQueue(double simulationTime, bool generateDepartures) {
    double time = 0.0f;
    Event lastDeparture(Event::EventType::Arrival, 0);

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

        if (generateDepartures) {

            // Calculate time at which this arrival will be processed
            double packetLength = numGen.GenerateRandomValue(1.0f/L);
            double serviceTime = (packetLength / C);

            // If the last generated departure occurs later than our current time base our new departure off that time
            double departureTime = lastDeparture.GetProcessTime() > time ? lastDeparture.GetProcessTime() : time;

            // Create and push corresponding departure event
            Event departureEvent(Event::EventType::Departure, departureTime + serviceTime);
            lastDeparture = departureEvent;

            eventList.push_back(departureEvent);
        }
    }

    // Sort all events by time stamp
    eventList.sort(locSortEventByTime);
}