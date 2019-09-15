#include <iostream>
#include <vector>
#include <string>

#include "MM1Queue.hpp"
#include "MM1NQueue.hpp"

using namespace std;

int main()
{
    RandomNumberGenerator numGen;

    for (int i = 0; i < 1000; ++i)
    {
        double num = numGen.GenerateRandomValue(75);
        cout << num << endl;
    }

    cout << endl;
}