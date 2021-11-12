/*
*
*   FULLY CONNECTED NETWORK
*   
*/
#include "../include/FCnetwork.h"

// Set the variables
void FCnetwork::initialise(int N, mt19937 rand_gen) {

    this->rand_gen = rand_gen;
    this->N = N;
    this->edges = 0;

    // Fill the network with nodes
    for(int i = 0; i < N; i++) {
        Node* new_node = new Node(i);
        this->network.push_back(new_node);
    }
}

// Generate the network
void FCnetwork::generate() {

    // Connect all nodes with all other nodes
    for(int i=0; i < this->N; i++) {
        for(int j=0; j < this->N; j++) {
            if(i != j && !connected(this->network[i]->number, this->network[j]->number)) connect(this->network[i]->number, this->network[j]->number);
        }
    }
}