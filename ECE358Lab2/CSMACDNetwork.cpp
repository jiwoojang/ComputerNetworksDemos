#include "CSMACDNetwork.hpp"

#include <cfloat>
#include <iostream>
#include <iomanip>
#include <list>


CSMACDNetwork::CSMACDNetwork(PersistenceType newPersistenceType, int newN, double newA) {
    persistenceType = newPersistenceType;
    N = newN;
    A = newA;

    // All packets have same length here, and distance is the same between all nodes
    propDelay = D/S;
    transDelay = L/R;
}

// Constructor that takes a manually given list of lists of arrival times
CSMACDNetwork::CSMACDNetwork(PersistenceType newPersistenceType, std::list<std::list<double>> arrivals) {
    persistenceType = newPersistenceType;
    N = arrivals.size();

    propDelay = D/S;
    transDelay = L/R;

    // Create new node for each list of arrivals and initialize node's arrivals to that arrival list
    for (auto nodeArrivals: arrivals) {
        NodeEventQueue newNode;
        newNode.InitializeQueue(propDelay, transDelay, nodeArrivals);
        nodes.push_back(newNode);
    }
}

CSMACDNetwork::~CSMACDNetwork() {}

// For given number of nodes and arrival characteristics, create nodes and generate node arrival times
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
        if (currPacketTime > startTime && currPacketTime < packetTime) 
        {
            index = i;
            packetTime = currPacketTime;
        }
    }

    return index;
}

// Sum performance metrics from each node and print total network performance stats
CSMACDNetwork::SimulationResult CSMACDNetwork::CalculatePerformance() {
    double totalPackets = 0;
    double totalCollisions = 0;
    double totalSucesses = 0;
    double totalDropped = 0;

    for (NodeEventQueue node: nodes) 
    {
        NodeEventQueue::NodeResult nodeResult = node.GetPerformanceStats();

        totalPackets += nodeResult.packets;
        totalCollisions += nodeResult.collisions;
        totalSucesses += nodeResult.successes;
        totalDropped += nodeResult.dropped;
    }
    
    double throughput = (totalSucesses * L) / simulationTime;
    double efficiency = totalSucesses / (totalSucesses+totalCollisions);

    return {throughput, efficiency};
}

// Main function to run the simulation after initialization
CSMACDNetwork::SimulationResult CSMACDNetwork::RunSimulation() {
    // Index of node transmitting current packet
    int index=0;
    double packetTransTime;

    while (true) {
        // Scan for next packet
        index = GetNextPacketIndex();
        if (index < 0) break; // no packet found, all packets have been processed

        packetTransTime = nodes[index].GetNextEventTime();
        if (packetTransTime > simulationTime) break; // cutoff at the end of simulation time

        // If a node is ready to transmit, then reset its busy backoff counter if being used
        if (persistenceType == PersistenceType::NonPersistent)
        {
            nodes[index].ResetBusyBackOffCounter();
        }

        // Check for collisions
        bool collision = false;
        for (int i=0; i<N; i++) {
            if (i==index) continue;

            // More than 1 collision can occur on one transmission
            if (nodes[i].WillCollideWithTransmission(packetTransTime, abs(index-i))) {

                // Collision occurs right away, because nodes detect them right away
                nodes[i].TransmitPacketWithCollision();
                nodes[i].ApplyExponentialBackOff(nodes[i].GetNextEventTime()+transDelay);
                collision = true;
            }
        }
        
        // Process effects of collision or not
        if (collision)
        {
            // If a collision occured the transmitting node sees it immediately as per lab manual
            nodes[index].TransmitPacketWithCollision();
            nodes[index].ApplyExponentialBackOff(packetTransTime + transDelay);
        }
        else
        {
            nodes[index].TransmitPacketSuccessfully();
        }

        // Backoff on all nodes *after* packet is sent or collision is processed
        for (int i=0; i<N; i++) 
        {   
            if (nodes[i].WillDetectBusBusy(packetTransTime, abs(index-i))) 
            {
                switch(persistenceType)
                {
                    case PersistenceType::Persistent:
                    {
                        nodes[i].ApplyBusyWait(packetTransTime+transDelay, abs(index-i));
                        break;
                    }
                    case PersistenceType::NonPersistent:
                    {
                        nodes[i].ApplyBusyExponentialBackOff(packetTransTime+transDelay, abs(index-i));
                        break;
                    }
                }
            }
        }
    }

    return CalculatePerformance();
}