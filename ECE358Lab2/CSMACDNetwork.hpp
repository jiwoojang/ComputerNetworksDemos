#pragma once
#include "NodeEventQueue.hpp"

#include <vector>

class CSMACDNetwork
{
public:

    enum PersistenceType
    {
        Persistent,
        NonPersistent
    };

    struct Packet {
        double startTime;
        double endTime;
        uint startPosition;
    };

    CSMACDNetwork(PersistenceType newPersistenceType, int newN, int newR, int newL, double newA, double newD, double newS);
    ~CSMACDNetwork();

    void InitializeNetwork();
    void RunSimulation();

private: 
    // Network Parameters
    int N;
    double A;
    int R;
    int L;
    double D;
    double S;

    std::vector<NodeEventQueue> nodes;

    // Simulation Parameters
    double simulationTime;
    PersistenceType persistenceType;

    Packet lastPacket;
};