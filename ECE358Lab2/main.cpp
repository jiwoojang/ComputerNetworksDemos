#include <iostream>

#include "CSMACDNetwork.hpp"
using namespace std;



int main() {
    cout << "Running Main" << endl;

    // See CSMACDNetwork.hpp for network simulation constant parameters
    for (int i = 20; i <= 100; i+=20)
    {
    CSMACDNetwork simulation(PersistenceType::Persistent, i, 7);
    //CSMACDNetwork simulation(PersistenceType::NonPersistent, i, 7);
    simulation.InitializeNetwork();
    
    CSMACDNetwork::SimulationResult result = simulation.RunSimulation();
    std::cout << i << " Nodes" << endl;
    std::cout << result.throughput << ", " << result.efficiency << std::endl;
    }
}