#include "CSMACDNetwork.cpp"


CSMACDNetwork::CSMACDNetwork(PersistenceType newPersistenceType, int newN, int newR, int newL, double newA, double newD, double newS) {
    persistenceType = newPersistenceType;
    N = newN;
    R = newR;
    L = newL;
    A = newA;
    D = newD;
    S = newS;
}

CSMACDNetwork::InitializeNetwork() {
    for (int i=0; i<N; i++) {
        NodeEventQueue newNode(A, R);
        newNode.InitializeQueue(simulationTime);
        nodes.push_back(newNode);
    }
}