#include "Event.hpp"

Event::Event(EventType type, double newProcessTime)
{
    eventType = type;
    processTime = newProcessTime;
}