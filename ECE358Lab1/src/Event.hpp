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

    Event(EventType type, double processTime);
    ~Event(){};

private:
    EventType eventType = Invalid;
    // TODO: Figure out if there are better portable versions of floats
    double newProcessTime = 0.0f;

    void SetProcessTime(float newProcessTime);
};