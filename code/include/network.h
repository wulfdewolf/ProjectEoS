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

    // Initialisors
    // -------------------------------------------------------

    // Generation
    Network(int population_size, mt19937 rand_gen, int edges);

    // Reading from file
    Network(string filename);


    // Fields
    // -------------------------

    // Agents
    int population_size, edges;

    // Random number generator
    mt19937 rand_gen;

    // Vector with agents
    vector<Agent*> network;


    // Methods
    // --------------------------------------------------------------------------------------------------------------------------

    // Read a network from a file in the form of an edge-list
    void read_network(string filename);

    // Write the network to a file in the form of an edge-list
    void write_network(string filename);

    // Check if agents are connected
    bool connected(int A, int B);

    // Connecting two agents
    void connect(int A, int B);

    // Imitation game simulation
    void simulation(int average_over, int iterations, double noise, double new_signal_prob, double clean_prob, vector<int> write_iterations, string filename);

    // Free the allocated memory
    void free();

};

#endif