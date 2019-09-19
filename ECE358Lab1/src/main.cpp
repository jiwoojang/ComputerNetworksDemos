#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#include "MM1Queue.hpp"
#include "MM1KQueue.hpp"

using namespace std;


void MM1Test() {
    for (float i = 0.25f; i < 1.05f; i += 0.1f)
    {
        cout << "rho: " << i << endl;
        double lambda = i * 1000000 / 2000;

        MM1Queue queue(lambda, 2000, lambda * 5, 1000000);
        queue.InitalizeQueue(1000.0f);
        queue.ProcessQueue();
    }
}


void MM1KTest() {
    int K = 10;
    for (float i = 0.25f; i < 1.05f; i += 0.1f)
    {
        cout << "rho: " << i << endl;
        double lambda = i * 1000000 / 2000;

        MM1KQueue queue(lambda, 2000, lambda * 5, 1000000, K);
        queue.InitalizeQueue(1000.0f, false);
        queue.ProcessQueue();
    }
}

int main()
{
    MM1KTest();
    
    // For precise logging
/*     cout << fixed;
    cout << setprecision(10);

    for (Event event : queue.GetEventList())
    {
        string type = "";
        switch(event.GetEventType())
        {
            case Event::EventType::Arrival:
            {
                type = "Arrival";
                break;
            }
            case Event::EventType::Departure:
            {
                type = "Departure";
                break;
            }
            case Event::EventType::Observer:
            {
                type = "Observer";
                break;
            }
            default:
                break;
        }

        cout << type <<  ": Time: " << event.GetProcessTime() << endl;
    } */
}