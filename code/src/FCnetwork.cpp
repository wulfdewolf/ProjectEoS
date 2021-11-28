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

    // Fill the network with agents
    for(int i = 0; i < N; i++) {
        Agent* new_agent = new Agent(i);
        this->network.push_back(new_agent);
    }
}

// Generate the network
void FCnetwork::generate() {

    // Connect all agents with all other agents
    for(int i=0; i < this->N; i++) {
        for(int j=0; j < this->N; j++) {
            if(i != j && !connected(this->network[i]->number, this->network[j]->number)) connect(this->network[i]->number, this->network[j]->number);
        }
    }
}