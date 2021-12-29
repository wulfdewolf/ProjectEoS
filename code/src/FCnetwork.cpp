/*
*
*   FULLY CONNECTED NETWORK
*   
*/
#include "../include/FCnetwork.h"

FCnetwork::FCnetwork(int population_size, mt19937 rand_gen, string filename) : Network(population_size, rand_gen, 0) {

    // Initialisation
    for(int i = 0; i < population_size; i++) {
        Agent* new_agent = new Agent(i);
        network.push_back(new_agent);
    }

    // Random connection
    for(int i=0; i < population_size; i++) {
        for(int j=0; j < population_size; j++) {
            if(i != j && !connected(network[i]->number, network[j]->number)) connect(network[i]->number, network[j]->number);
        }
    }

    // Write to file
    write_network("../results/" + filename);
}