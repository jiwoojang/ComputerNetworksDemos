#include "Event.hpp"

Event::Event(EventType type, double processTime)
{
    eventType = type;
    SetProcessTime(processTime);
}

void Event::SetProcessTime(float newProcessTime) 
{ 
    newProcessTime = newProcessTime; 
}