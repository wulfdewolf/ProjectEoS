#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <bits/stdc++.h> 
#include "agent.h"
#include "util.h"
using namespace std;
using namespace std::chrono; 


// NETWORK CLASS
class Network {

    public: 

    // agents
    int N, edges;

    // Random number generator
    mt19937 rand_gen;

    // Vector with agents
    vector<Agent*> network;

    // Read a network from a file in the form of an edge-list
    void read_network(string filename);

    // Print the network to a file in the form of an edge-list
    void print_network(string filename);

    // Check if agents are connected
    bool connected(int A, int B);

    // Connecting two agents
    void connect(int A, int B);

    // Imitation game simulation
    void simulation(string filename, int iterations, double noise, double new_signal_prob, double clean_prob);

    // Free the allocated memory
    void free_memory();

};

#endif