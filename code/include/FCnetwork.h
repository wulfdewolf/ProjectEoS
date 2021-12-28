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

    // Initialisor
    // -----------------------------------------------

    FCnetwork(int population_size, mt19937 rand_gen, string filename);
};

#endif