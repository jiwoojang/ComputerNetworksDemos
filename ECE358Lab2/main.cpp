#include <iostream>

#include "CSMACDNetwork.hpp"
using namespace std;


void runSimpleSim() {
    cout << "Running Main" << endl;
    CSMACDNetwork simulation(PersistenceType::Persistent,
    {
        {0.000001, 0.00002},
        {0.000001001},
    });

    CSMACDNetwork::SimulationResult result = simulation.RunSimulation();
    std::cout << result.throughput << ", " << result.efficiency << std::endl;
}

int main() {
    // See CSMACDNetwork.hpp for network simulation constant parameters
    for (int i = 20; i <= 100; i+=20)
    {
        CSMACDNetwork simulation(PersistenceType::NonPersistent, i, 20);
        //CSMACDNetwork simulation(PersistenceType::NonPersistent, i, 7);
        simulation.InitializeNetwork();
        
        CSMACDNetwork::SimulationResult result = simulation.RunSimulation();
        std::cout << i << " Nodes" << endl;
        std::cout << result.throughput << ", " << result.efficiency << std::endl;
    }
}