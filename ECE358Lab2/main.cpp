#include <iostream>

#include "CSMACDNetwork.hpp"
using namespace std;



int main() {
    cout << "Running Main" << endl;

    // See CSMACDNetwork.hpp for network simulation constant parameters
    //CSMACDNetwork simulation(PersistenceType::Persistent, 40, 20);
    CSMACDNetwork simulation(PersistenceType::NonPersistent, 40, 20);
    simulation.InitializeNetwork();
    
    CSMACDNetwork::SimulationResult result = simulation.RunSimulation();
    std::cout << result.throughput << "," << result.efficiency << std::endl;
}