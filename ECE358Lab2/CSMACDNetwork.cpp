#include "CSMACDNetwork.hpp"

#include <cfloat>
#include <iostream>

CSMACDNetwork::CSMACDNetwork(PersistenceType newPersistenceType, int newN, double newA) {
    persistenceType = newPersistenceType;
    N = newN;
    A = newA;

    simulationTime = 1000;
    propDelay = D/S;
    // All packets have same length here
    transDelay = L/R;
}

CSMACDNetwork::~CSMACDNetwork() {}

void CSMACDNetwork::InitializeNetwork() {
    for (int i=0; i<N; i++) {
        NodeEventQueue newNode(A, R);
        newNode.InitializeQueue(simulationTime, propDelay, transDelay);
        nodes.push_back(newNode);
    }
}

// Find next packet after startTime
int CSMACDNetwork::GetNextPacketIndex(double startTime) {
    double packetTime = DBL_MAX;
    uint index = -1;

    for (uint i=0; i < nodes.size(); i++) {
        double currPacketTime = nodes[i].GetNextEventTime();
        if (nodes[i].GetQueueSize() > 0 
            && nodes[i].GetNextEventTime() < packetTime
            && nodes[i].GetNextEventTime() > startTime) 
        {
            index = i;
            packetTime = currPacketTime;
        }
    }

    return index;
}

CSMACDNetwork::SimulationResult CSMACDNetwork::CalculatePerformance() {
    double totalTransmissions = 0;
    double totalCollisions = 0;
    double totalSucesses = 0;

    for (NodeEventQueue node: nodes) {
        NodeEventQueue::NodeResult nodeResult = node.GetPerformanceStats();

        totalTransmissions += nodeResult.transmissions;
        totalCollisions += nodeResult.collisions;
        totalSucesses += nodeResult.successes;
    }
    std::cout << "Transmissions, Successes, Collisions" << endl;
    std::cout << totalTransmissions << "," << totalSucesses << "," << totalCollisions << std::endl;
    
    double throughput = (totalSucesses * L) / simulationTime;
    double efficiency = totalSucesses / totalTransmissions;

    return {throughput, efficiency};
}

CSMACDNetwork::SimulationResult CSMACDNetwork::RunSimulation() {
    int index=0;

    while (true) {
        // Scan for next packet
        index = GetNextPacketIndex();
        if (index < 0) break;

        double packetTransTime = nodes[index].GetNextEventTime();

        // Find the 1st collision that will occur
        double lowestCollisionTime = DBL_MAX;
        int collisionIndex = -1;

        for (int i=0; i<N; i++) {
            if (i==index) continue;

            if (nodes[i].WillCollideWithTransmission(packetTransTime, abs(index-i))) {
                if (nodes[i].GetNextEventTime() < lowestCollisionTime && nodes[i].GetNextEventTime() > 0) {
                    lowestCollisionTime = nodes[i].GetNextEventTime();
                    collisionIndex = i;
                };
            }
        }

        // Process effects of collision
        if (collisionIndex > 0) {
            nodes[index].TransmitPacketWithCollision();
            nodes[collisionIndex].TransmitPacketWithCollision();
            nodes[index].ApplyExponentialBackOff(lowestCollisionTime);
            nodes[collisionIndex].ApplyExponentialBackOff(lowestCollisionTime);
            continue;
        }
        // Apply busy wait where applicable
        else {
            for (int i=0; i<N; i++) {
                if (nodes[i].WillBusyWait(packetTransTime, abs(index-i))) {
                    nodes[i].ApplyBusyWait(packetTransTime, abs(index-i));
                }
            }
        }

        nodes[index].TransmitPacketSuccessfully();
    }

    return CalculatePerformance();
}