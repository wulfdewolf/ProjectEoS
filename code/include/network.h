#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <bits/stdc++.h> 
#include "node.h"
#include "util.h"
using namespace std;
using namespace std::chrono; 


// NETWORK CLASS
class Network {

    public: 

    // Nodes
    int N, edges;

    // Vector with nodes
    vector<Node*> network;

    // Read a network from a file in the form of an edge-list
    void read_network(string filename);

    // Print the network to a file in the form of an edge-list
    void print_network(string filename);

    // Check if nodes are connected
    bool connected(int A, int B);

    // Connecting two nodes
    void connect(int A, int B);

    // Imitation game simulation
    void simulation(string filename, int iterations, float noise);

    // Free the allocated memory
    void free_memory();

};

#endif