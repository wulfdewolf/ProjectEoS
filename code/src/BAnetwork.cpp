/*
*
*   BARABASI-ALBERT NETWORK
*   
*/
#include "../include/BAnetwork.h"

// Set the variables
void BAnetwork::initialise(int N, mt19937 rand_gen) {

    this->rand_gen = rand_gen;
    this->N = N;
    this->edges = 6;

    // Initialization
    for(int i = 0; i < 4; i++) {

        Agent* new_agent = new Agent(i);

        for(int j = 0; j < 4; j++) {
            if(j != i) new_agent->edges.push_back(j);
        }
        this->network.push_back(new_agent);
    }
}

// Generate the network
void BAnetwork::generate() {
    uniform_real_distribution<> prob_dis(0, 1);

    while(this->network.size() < this->N) {
        uniform_int_distribution<> agent_dis(0, (this->network.size())-1); 

        // new Agent
        Agent* new_agent = new Agent(this->network.size());

        // Add the new Agent to the network
        this->network.push_back(new_agent);

        // New edges count
        int count = 4;

        // Add 4 edges according to the probabilities provided
        while(count != 0) {

            // Random agent
            
            int agent_number = agent_dis(this->rand_gen);
            Agent* agent = this->network[agent_number];

            // If this agent isn't connected to the new one yet
            if(!connected(new_agent->number, agent_number)) {

                // Calculate the probability of connecting
                int degree_agent = agent->edges.size();
                double degree_total = (this->edges)*2;
                double connect_probability = degree_agent / degree_total;


                // Connect if the chances dictate it
                if(connect_probability >= prob_dis(this->rand_gen)) {

                    connect(new_agent->number, agent_number);
                    count--;
                };  
            }
        }

    }

    return;
}