#include "CSMACDNetwork.hpp"

#include <cfloat>
#include <iostream>

CSMACDNetwork::CSMACDNetwork(PersistenceType newPersistenceType, int newN, int newR, int newL, double newA, double newD, double newS) {
    persistenceType = newPersistenceType;
    N = newN;
    R = newR;
    L = newL;
    A = newA;
    D = newD;
    S = newS;

    simulationTime = 1000;
    propDelay = D/S;

    lastPacket.startTime = 0;
    lastPacket.endTime = 0;
    lastPacket.startIndex =  -1;
}

CSMACDNetwork::~CSMACDNetwork() {}

void CSMACDNetwork::InitializeNetwork() {
    for (int i=0; i<N; i++) {
        NodeEventQueue newNode(A, R);
        newNode.InitializeQueue(simulationTime);
        nodes.push_back(newNode);
    }
}

bool CSMACDNetwork::GetNextPacket() {
    double startTime = DBL_MAX;
    uint index = 0;
    bool found = false;

    for (uint i=0; i < nodes.size(); i++) {
        if (nodes[i].GetQueueSize() > 0 && nodes[i].GetNextEventTime() < startTime) {
            startTime = nodes[i].GetNextEventTime();
            index = i;
            found = true;
        }
    }

    if (found) {
        lastPacket.startTime = startTime;

        double packetLength = nodes[index].numGen.GenerateRandomPoissonValue(1.0f/L);
        double serviceTime = (packetLength / R);

        lastPacket.endTime = packetLength + serviceTime;
        lastPacket.startIndex = index;

        nodes[index].PopEvent();
    }
    return found;
}

void CSMACDNetwork::RunSimulation() {
    int i=0;
    while (GetNextPacket()) {
        i++;
        std::cout << lastPacket.startTime << std::endl;
    }
    std::cout << i <<  " packets generated" << std::endl;
    return;
}