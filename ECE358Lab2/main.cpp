#include <iostream>

#include "CSMACDNetwork.hpp"
using namespace std;

int main() {
    // See CSMACDNetwork.hpp for network simulation constant parameters
    std::cout << "Persistent,,,NonPersistent" << endl;
    std::cout << "Nodes,Throughput,Efficiency,Nodes,Throughput,Efficiency" << endl;

    for (int i = 20; i <= 100; i+=20)
    {
        CSMACDNetwork presistentSimulation(PersistenceType::Persistent, i, 7);
        presistentSimulation.InitializeNetwork();
        
        CSMACDNetwork::SimulationResult result = presistentSimulation.RunSimulation();
        std::cout << i << "," << result.throughput << "," << result.efficiency << ",";

        CSMACDNetwork nonPersistentSimulation(PersistenceType::NonPersistent, i, 7);
        nonPersistentSimulation.InitializeNetwork();
        
        result = nonPersistentSimulation.RunSimulation();
        std::cout << i << "," << result.throughput << "," << result.efficiency << endl;
    }

    for (int i = 20; i <= 100; i+=20)
    {
        CSMACDNetwork presistentSimulation(PersistenceType::Persistent, i, 10);
        presistentSimulation.InitializeNetwork();
        
        CSMACDNetwork::SimulationResult result = presistentSimulation.RunSimulation();
        std::cout << i << "," << result.throughput << "," << result.efficiency << ",";

        CSMACDNetwork nonPersistentSimulation(PersistenceType::NonPersistent, i, 10);
        nonPersistentSimulation.InitializeNetwork();
        
        result = nonPersistentSimulation.RunSimulation();
        std::cout << i << "," << result.throughput << "," << result.efficiency << endl;
    }

    for (int i = 20; i <= 100; i+=20)
    {
        CSMACDNetwork presistentSimulation(PersistenceType::Persistent, i, 20);
        presistentSimulation.InitializeNetwork();
        
        CSMACDNetwork::SimulationResult result = presistentSimulation.RunSimulation();
        std::cout << i << "," << result.throughput << "," << result.efficiency << ",";

        CSMACDNetwork nonPersistentSimulation(PersistenceType::NonPersistent, i, 20);
        nonPersistentSimulation.InitializeNetwork();
        
        result = nonPersistentSimulation.RunSimulation();
        std::cout << i << "," << result.throughput << "," << result.efficiency << endl;
    }

}