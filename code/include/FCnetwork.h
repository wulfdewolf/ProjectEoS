#ifndef FCNETWORK_H
#define FCNETWORK_H

#include <stdio.h>
#include <bits/stdc++.h> 
#include "agent.h"
#include "network.h"
using namespace std;

// FULLY CONNECTED NETWORK CLASS
class FCnetwork : public Network {

    public: 

    // Set the variables
    void initialise(int N, mt19937 rand_gen);

    // Generate the network
    void generate();

};

#endif