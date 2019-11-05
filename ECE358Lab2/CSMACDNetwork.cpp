#include "CSMACDNetwork.hpp"

#include <cfloat>
#include <iostream>
#include <iomanip>
#include <list>

CSMACDNetwork::CSMACDNetwork(PersistenceType newPersistenceType, int newN, double newA) {
    persistenceType = newPersistenceType;
    N = newN;
    A = newA;

    propDelay = D/S;
    // All packets have same length here
    transDelay = L/R;
}

CSMACDNetwork::~CSMACDNetwork() {}

CSMACDNetwork::CSMACDNetwork(PersistenceType newPersistenceType, std::list<std::list<double>> arrivals) {
    persistenceType = newPersistenceType;
    N = arrivals.size();

    propDelay = D/S;
    // All packets have same length here
    transDelay = L/R;

    for (auto nodeArrivals: arrivals) {
        NodeEventQueue newNode;
        newNode.InitializeQueue(propDelay, transDelay, nodeArrivals);
        nodes.push_back(newNode);
    }
}

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
    double totalDropped = 0;

    for (NodeEventQueue node: nodes) {
        NodeEventQueue::NodeResult nodeResult = node.GetPerformanceStats();

        totalPackets += nodeResult.packets;
        totalCollisions += nodeResult.collisions;
        totalSucesses += nodeResult.successes;
        totalDropped += nodeResult.dropped;
    }
    std::cout << "Total Packets, Successes, Collisions, Dropped" << endl;
    std::cout << totalPackets << "," << totalSucesses << "," << totalCollisions << "," << totalDropped << std::endl;
    
    double throughput = (totalSucesses * L) / simulationTime;
    double efficiency = totalSucesses / (totalSucesses+totalCollisions);

    return {throughput, efficiency};
}

CSMACDNetwork::SimulationResult CSMACDNetwork::RunSimulation() {
    int index=0;

    double packetTransTime;

    while (true) {
        // Scan for next packet
        index = GetNextPacketIndex();
        if (index < 0) break;

        packetTransTime = nodes[index].GetNextEventTime();

        if (packetTransTime > simulationTime) break;

        // If a node is ready to transmit, then reset its busy backoff counter if being used
        if (persistenceType == PersistenceType::NonPersistent)
        {
            nodes[index].ResetBusyBackOffCounter();
        }

        // Find the 1st collision that will occur (1st that transmitter sees)
        double lowestCollisionTime = DBL_MAX;
        int collisionIndex = -1;

        for (int i=0; i<N; i++) {
            if (i==index) continue;

            // More than 1 collision can occur on one transmission
            if (nodes[i].WillCollideWithTransmission(packetTransTime, abs(index-i))) {

                nodes[i].TransmitPacketWithCollision();
                // Collision occurs when signal from transmitted reaches node
                double collisionTime = packetTransTime + abs(index-i)*propDelay;
                nodes[i].ApplyExponentialBackOff(nodes[i].GetNextEventTime()+transDelay);

                if (nodes[i].GetNextEventTime() < lowestCollisionTime && nodes[i].GetNextEventTime() > 0) {
                    lowestCollisionTime = nodes[i].GetNextEventTime();
                    collisionIndex = i;
                };
            }
        }
        
        // Process effects of collision or not
        if (collisionIndex >= 0)
        {
            nodes[index].TransmitPacketWithCollision();

            // If a collision occured the transmitting node will see it at the time of the first collision plus propogation back
            nodes[index].ApplyExponentialBackOff(packetTransTime + transDelay);
        }
        else
        { 
            nodes[index].TransmitPacketSuccessfully();
        }  

        // Backoff on all nodes *after* packet is sent or collision is processed
        for (int i=0; i<N; i++) 
        {   
            if (nodes[i].WillDetectBusBusy(packetTransTime, abs(index-i))) {
                switch(persistenceType)
                {
                    case PersistenceType::Persistent:
                    {
                        nodes[i].ApplyBusyWait(packetTransTime+transDelay, abs(collisionIndex-i));
                        break;
                    }
                    case PersistenceType::NonPersistent:
                    {
                        nodes[i].ApplyBusyExponentialBackOff();
                        break;
                    }
                }
            }
        }
    }

    return CalculatePerformance();
}