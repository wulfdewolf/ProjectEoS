#ifndef BANETWORK_H
#define BANETWORK_H

#include <stdio.h>
#include <bits/stdc++.h> 
#include "agent.h"
#include "network.h"
using namespace std;

// BARABASI ALBERT NETWORK CLASS
class BAnetwork : public Network {

    public: 

    // Set the variables
    void initialise(int N, mt19937 rand_gen);

    // Generate the network
    void generate();

};

#endif