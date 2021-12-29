/*
*
*   BARABASI-ALBERT NETWORK
*   
*/
#include "../include/BAnetwork.h"

BAnetwork::BAnetwork(int population_size, mt19937 rand_gen, string filename) : Network(population_size, rand_gen, 6) {

    // Initialisation
    for(int i = 0; i < 4; i++) {

        Agent* new_agent = new Agent(i);

        for(int j = 0; j < 4; j++) {
            if(j != i) new_agent->edges.push_back(j);
        }
        network.push_back(new_agent);
    }

    // Preferential attachment
    uniform_real_distribution<> prob_dis(0, 1);
    while(network.size() < population_size) {
        uniform_int_distribution<> agent_dis(0, (network.size())-1); 

        // new Agent
        Agent* new_agent = new Agent(network.size());

        // Add the new Agent to the network
        network.push_back(new_agent);

        // New edges count
        int count = 4;

        // Add 4 edges according to the probabilities provided
        while(count != 0) {

            // Random agent

            int agent_number = agent_dis(rand_gen);
            Agent* agent = network[agent_number];

            // If this agent isn't connected to the new one yet
            if(!connected(new_agent->number, agent_number)) {

                // Calculate the probability of connecting
                int degree_agent = agent->edges.size();
                double degree_total = (edges)*2;
                double connect_probability = degree_agent / degree_total;

                // Connect if the chances dictate it
                if(connect_probability >= prob_dis(rand_gen)) {

                    connect(new_agent->number, agent_number);
                    count--;
                };  
            }
        }

    }

    // Write to file
    write_network("../results/" + filename);
}