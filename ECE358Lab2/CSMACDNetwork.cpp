#include "CSMACDNetwork.hpp"

#include <cfloat>
#include <iostream>

CSMACDNetwork::CSMACDNetwork(PersistenceType newPersistenceType, int newN, double newA) {
    persistenceType = newPersistenceType;
    N = newN;
    A = newA;

    simulationTime = 1000; // TODO move this to main.cpp and tune
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

// Find node that contains the next packet after startTime
int CSMACDNetwork::GetNextPacketIndex(double startTime) {
    double packetTime = DBL_MAX;
    uint index = -1;

    for (uint i=0; i < nodes.size(); i++) {
        // If no data currPacketTime will be negative
        double currPacketTime = nodes[i].GetNextEventTime();
        if (currPacketTime > startTime
            && currPacketTime < packetTime) 
        {
            index = i;
            packetTime = currPacketTime;
        }
    }

    return index;
}

CSMACDNetwork::SimulationResult CSMACDNetwork::CalculatePerformance() {
    double totalPackets = 0;
    double totalCollisions = 0;
    double totalSucesses = 0;

    for (NodeEventQueue node: nodes) {
        NodeEventQueue::NodeResult nodeResult = node.GetPerformanceStats();

        totalPackets += nodeResult.packets;
        totalCollisions += nodeResult.collisions;
        totalSucesses += nodeResult.successes;
    }
    std::cout << "Transmissions, Successes, Collisions" << endl;
    std::cout << totalPackets << "," << totalSucesses << "," << totalCollisions << std::endl;
    
    double throughput = (totalSucesses * L) / simulationTime;
    double efficiency = totalSucesses / totalPackets;

    return {throughput, efficiency};
}

CSMACDNetwork::SimulationResult CSMACDNetwork::RunSimulation() {
    int index=0;

    while (true) {
        // Scan for next packet
        index = GetNextPacketIndex();
        if (index < 0) break;

        double packetTransTime = nodes[index].GetNextEventTime();
        
        // If a node is ready to transmit, then reset its busy backoff counter if being used
        if (persistenceType == PersistenceType::NonPersistent)
        {
            nodes[index].ResetBusyBackOffCounter();
        }

        // Find the 1st collision that will occur
        double lowestCollisionTime = DBL_MAX;
        int collisionIndex = -1;

        for (int i=0; i<N; i++) {
            if (i==index) continue;

            // More than 1 collision can occur on one transmission
            if (nodes[i].WillCollideWithTransmission(packetTransTime, abs(index-i))) {
                nodes[i].TransmitPacketWithCollision();
                // Collision occurs when signal from transmitted reaches node
                nodes[i].ApplyExponentialBackOff(packetTransTime + abs(index-i) * propDelay);

                if (nodes[i].GetNextEventTime() < lowestCollisionTime && nodes[i].GetNextEventTime() > 0) {
                    lowestCollisionTime = nodes[i].GetNextEventTime();
                    collisionIndex = i;
                };
            }
            else {
                // Even if there is a collision going on, other nodes will still see the bus as busy
                if (nodes[i].WillDetectBusBusy(packetTransTime, abs(index-i))) {
                    switch(persistenceType)
                    {
                        case PersistenceType::Persistent:
                        {
                            nodes[i].ApplyBusyWait(packetTransTime, abs(index-i));
                            break;
                        }
                        case PersistenceType::NonPersistent:
                        {
                            nodes[i].ApplyBusyExponentialBackOff();
                            break;
                        }
                        default:
                            break;
                    }
                }
            }
        }

        // Process effects of collision
        if (collisionIndex > 0) {
            nodes[index].TransmitPacketWithCollision();
            
            // If a collision occured the transmitting node will see it at the time of the first collision
            nodes[index].ApplyExponentialBackOff(packetTransTime + abs(index-collisionIndex) * propDelay);
        }
        else
        {
            nodes[index].TransmitPacketSuccessfully();
        }    
    }

    return CalculatePerformance();
}