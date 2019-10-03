#pragma once
#include "NodeEventQueue.hpp"

class CSMACDNetwork
{
public:

    enum PersistenceType
    {
        Persistent,
        NonPersistent
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

    // Simulation Parameters
    double simulationTime;
    PersistenceType persistenceType;
};