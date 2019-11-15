#pragma once
#include <cstdint>

class Event
{
public:
    enum EventType
    {
        Invalid,
        Arrival,
        Observer,
        Departure
    };

    Event(EventType type, double newProcessTime);
    ~Event(){};

    double GetProcessTime() const { return processTime; }
    void SetProcessTime(double newProcessTime) { processTime = newProcessTime; }
    EventType GetEventType() const { return eventType; }

private:
    EventType eventType = Invalid;
    double processTime = 0.0f;
};