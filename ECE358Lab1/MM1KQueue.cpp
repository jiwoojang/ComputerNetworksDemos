#include <iostream>
#include <queue>

#include "MM1KQueue.hpp"
#include "Event.hpp"


MM1KQueue::MM1KQueue(double newLambda, int newL, double newAlpha, int newC, int newK) 
{
    lambda = newLambda;
    L = newL;
    alpha = newAlpha;
    C = newC;
    K = newK;
}

void MM1KQueue::ProcessQueue()
{
    EventQueue::ProcessResults results;

    // Add departure events to both the list and here if applicable
    // if event in queue, base departure time off last event in queue
    std::queue<Event> packetQueue;

    for (auto it=eventList.begin(); it != eventList.end(); ++it) 
    {  
        Event event = *it;
        switch (event.GetEventType()) {
            case Event::EventType::Arrival:
            {
                ++results.numArrivals;

                if (packetQueue.size() >= K) 
                {
                    // Drop packet
                    ++results.numDropped;
                    break;
                }
                else 
                {
                    // Determine departure timme based on random size and the last element in the packet queu
                    double packetLength = numGen.GenerateRandomValue(1.0f/L);
                    double serviceTime = (packetLength / C);
                    double departureTime = 0;

                    if (packetQueue.size() == 0) 
                    {
                        departureTime = event.GetProcessTime() + serviceTime;
                    }
                    else {
                        departureTime = packetQueue.back().GetProcessTime() + serviceTime;
                    }

                    // Store new departure event in packet queue
                    Event departureEvent(Event::EventType::Departure, departureTime);
                    packetQueue.push(departureEvent);

                    // Insert departure event into current list at correct time
                    auto insertIt = std::next(it);
                    while (insertIt->GetProcessTime() < departureTime && insertIt != eventList.end())
                    {
                        insertIt = std::next(insertIt);
                    }
                    eventList.insert(insertIt, departureEvent);
                }
                break;
            }
            case Event::EventType::Departure:
            {
                packetQueue.pop();
                ++results.numDepartures;
                break;
            }
            case Event::EventType::Observer:
            {
                ++results.numObservations;

                if (packetQueue.size() == 0) 
                {
                    ++results.idleCount;
                }
                else 
                {
                    results.queueSum += packetQueue.size();
                }
            }
            default:
                break;
        }
    }


    // Print CSV output
    cout << K << ",";
    cout << (alpha/5.0)*2000/1000000 << ",";
    cout << results.GetAveragePacketsInQueue() << ",";
    cout << results.GetIdleTimePercent() << ",";
    cout << results.GetDroppedPacketPercent() << endl;
}