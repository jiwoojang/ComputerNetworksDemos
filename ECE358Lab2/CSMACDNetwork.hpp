#pragma once
#include "NodeEventQueue.hpp"

#include <vector>

enum class PersistenceType
{
    Persistent,
    NonPersistent
};

class CSMACDNetwork
{
public:
    struct Packet {
        double startTime;
        double endTime;
        int startIndex;
    };

    CSMACDNetwork(PersistenceType newPersistenceType, int newN, int newR, int newL, double newA, double newD, double newS);
    ~CSMACDNetwork();

    void InitializeNetwork();
    void RunSimulation();
    bool GetNextPacket();

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

    double propDelay;
    Packet lastPacket;
};