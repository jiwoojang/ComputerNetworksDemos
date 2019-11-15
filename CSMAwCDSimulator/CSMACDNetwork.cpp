#include "CSMACDNetwork.hpp"

#include <cfloat>
#include <iostream>
#include <iomanip>
#include <list>
#include <vector> 
#include <algorithm>

CSMACDNetwork::CSMACDNetwork(PersistenceType newPersistenceType, int newN, double newA) {
    persistenceType = newPersistenceType;
    N = newN;
    A = newA;

    // All packets have same length here, and distance is the same between all nodes
    propDelay = D/S;
    transDelay = L/R;
}

CSMACDNetwork::~CSMACDNetwork() {
    nodes.clear();
}

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

        //std::cout << "NODE : Packets: " << nodeResult.packets << " Collisions: " << nodeResult.collisions << " Successes: " << nodeResult.successes << " Dropped: " << nodeResult.dropped << " Packets left: " << node.GetQueueSize() << endl;
    }
    //std::cout << "Packets: " << totalPackets << " Collisions: " << totalCollisions << " Successes: " << totalSucesses << " Dropped: " << totalDropped << endl;
    double throughput = (totalSucesses * L) / simulationTime;
    double efficiency = totalSucesses / (totalSucesses+totalCollisions+totalDropped);

    return {throughput, efficiency};
}

// Main function to run the simulation after initialization
CSMACDNetwork::SimulationResult CSMACDNetwork::RunSimulation() {
    // Index of node transmitting current packet
    int index=0;
    double packetTransTime;

    std::vector<int> collisionIndexCache;
    double prevSuccess = 0;
    int prevSuccessIndex = -1;

    while (true) {
        // Clear cache
        collisionIndexCache.clear();

        // Scan for next packet
        index = GetNextPacketIndex();
        if (index < 0)
            return CalculatePerformance(); // no packet found, all packets have been processed

        packetTransTime = nodes[index].GetNextEventTime();

        if (packetTransTime > simulationTime)
            return CalculatePerformance(); // cutoff at the end of simulation time

        // If a node is ready to transmit, then reset its busy backoff counter if being used
        if (persistenceType == PersistenceType::NonPersistent)
        {
            nodes[index].ResetBusyBackOffCounter();
        }

        // Check for collisions
        bool collision = false;
        double highestCollisionTime = -1;

        for (int i=0; i<N; i++) {
            if (i==index) continue;

            // More than 1 collision can occur on one transmission
            if (nodes[i].WillCollideWithTransmission(packetTransTime, abs(index-i))) 
            {
                // Track this collision
                collisionIndexCache.push_back(i);

                //std::cout << setprecision(20) << "Collision at index: " << i << " with timestamp:" << nodes[i].GetNextEventTime() << " and transmission index: " << index << " at transmission time: " << packetTransTime << endl;

                // Cache the highest collision time for the transmitting node to back off to
                double collisionTime = packetTransTime + propDelay*abs(index - i);

                if (collisionTime > highestCollisionTime && nodes[i].GetNextEventTime() > 0)
                {
                    highestCollisionTime = collisionTime;
                }
                
                collision = true;
            }
        }
        
        // If collision free, transmit successfully
        if (!collision)
        {
            // Dither percision is different per type, because of orders of magnitudes of operations
            double ditherCeiling; 
            if (persistenceType == PersistenceType::Persistent)
                ditherCeiling = 1e-10;
            else
                ditherCeiling = DBL_MIN;
                
            nodes[index].TransmitPacketSuccessfully(ditherCeiling);
            //std::cout << "Successful transmission at index: " << index << " at transmission time: " << packetTransTime << endl;
        }

        // Now proecss all other nodes
        for (int i=0; i<N; i++) 
        {
            if (collision)
            {
                if (i == index)
                {
                    nodes[index].TransmitPacketWithCollision();
                    nodes[index].ApplyExponentialBackOff(packetTransTime);
                    continue;
                }
                else if (std::find(collisionIndexCache.begin(), collisionIndexCache.end(), i) != collisionIndexCache.end())
                {
                    nodes[i].TransmitPacketWithCollision();
                    nodes[i].ApplyExponentialBackOff(packetTransTime + propDelay*abs(index - i));
                    continue;
                }
            }            

            if (nodes[i].WillDetectBusBusy(packetTransTime, abs(index-i))) 
            {
                switch(persistenceType)
                {
                    case PersistenceType::Persistent:
                    {
                        nodes[i].ApplyBusyWait(packetTransTime + transDelay + propDelay*abs(index-i));
                        break;
                    }
                    case PersistenceType::NonPersistent:
                    {
                        nodes[i].ApplyBusyExponentialBackOff(packetTransTime + transDelay + propDelay*abs(index-i));
                        break;
                    }
                }
            }
        }  
    }

    return CalculatePerformance();
}