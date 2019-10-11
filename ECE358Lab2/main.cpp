#include <iostream>

#include "CSMACDNetwork.hpp"
using namespace std;

int main() {
    cout << "Running Main" << endl;

    CSMACDNetwork simulation(PersistenceType::Persistent, 20, 1e6, 1500, 7, 10, 2e8);
    simulation.InitializeNetwork();
    simulation.RunSimulation();
}