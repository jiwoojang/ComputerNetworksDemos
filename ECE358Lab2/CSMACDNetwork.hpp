#pragma once
#include "NodeEventQueue.hpp"

#include <vector>

#define TOL 1e-9

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

    struct SimulationResult {
        double throughput;
        double efficiency;
    };

    CSMACDNetwork(PersistenceType newPersistenceType, int newN, double newA);
    CSMACDNetwork(PersistenceType newPersistenceType, std::list<std::list<double>> arrivals);
    ~CSMACDNetwork();

    void InitializeNetwork();
    SimulationResult RunSimulation();
    int GetNextPacketIndex(double startTime=0);

private: 
    SimulationResult CalculatePerformance();

    // Network Parameters for each run
    int N;
    double A;
    PersistenceType persistenceType;

    // Constant values given in lab manual that hold accross all questions
    const double R = 1e6; // 1 Mbps
    const int L = 1500; // 1500 bits/packet
    const double D = 10000.0; // 10 m between nodes
    const double S = 2e8; // 2/3 of speed of light in m/s
    const double simulationTime = 1000; // run simulation for 1000s

    std::vector<NodeEventQueue> nodes;

    double propDelay;
    double transDelay;

    Packet currentPacket;
    Packet nextPacket;

    double prevTransTime;
    bool wasPrevCollision;
    int prevNode;
};