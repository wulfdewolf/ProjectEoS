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

    // Initialisor
    // -----------------------------------------------

    BAnetwork(int population_size, mt19937 rand_gen, string filename);
};

#endif