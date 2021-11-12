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

        Node* new_node = new Node(i);

        for(int j = 0; j < 4; j++) {
            if(j != i) new_node->edges.push_back(j);
        }
        this->network.push_back(new_node);
    }
}

// Generate the network
void BAnetwork::generate() {
    std::uniform_real_distribution<> prob_dis(0, 1);

    while(this->network.size() < this->N) {
        std::uniform_int_distribution<> node_dis(0, (this->network.size())-1); 

        // New node
        Node* new_node = new Node(this->network.size());

        // Add the new node to the network
        this->network.push_back(new_node);

        // New edges count
        int count = 4;

        // Add 4 edges according to the probabilities provided
        while(count != 0) {

            // Random node
            
            int node_number = node_dis(this->rand_gen);
            Node* node = this->network[node_number];

            // If this node isn't connected to the new one yet
            if(!connected(new_node->number, node_number)) {

                // Calculate the probability of connecting
                int degree_node = node->edges.size();
                double degree_total = (this->edges)*2;
                double connect_probability = degree_node / degree_total;


                // Connect if the chances dictate it
                if(connect_probability >= prob_dis(this->rand_gen)) {

                    connect(new_node->number, node_number);
                    count--;
                };  
            }
        }

    }

    return;
}