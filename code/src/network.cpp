/*
*
*   NETWORK
*   
*/
#include "../include/network.h"

// Read network from file
void Network::read_network(string filename) {

    ifstream infile(filename);

    // Network initialisation from file
    infile >> this->N >> this->edges;

    // Create all the agents
    Agent* new_agent;
    for(int i = 0; i < this->N; i++) {
        new_agent = new Agent(i);
        this->network.push_back(new_agent);
    }

    // Go over the file  to create all the edges
    int agentA_number, agentB_number;
    Agent* agentA;
    Agent* agentB;
    while (infile >> agentA_number >> agentB_number) {
        agentA = this->network[agentA_number];
        agentB = this->network[agentB_number];

        if(!agentA->connected(agentB_number)) agentA->connect(agentB_number);
        if(!agentB->connected(agentA_number)) agentB->connect(agentA_number);
    }
}

// Print network to file
void Network::print_network(string filename) {

    ofstream file(filename);

    // Print network size and edges as first line
    file << this->N << " " << this->edges << endl;

    for (int i = 0; i < this->N; i++) {

        Agent* current_agent = this->network[i];

        for (int j = 0; j < current_agent->edges.size(); j++) {

            Agent* connected_agent = this->network[current_agent->edges[j]];

            file << current_agent->number << " " << connected_agent->number << endl;
        }
    }
}

// Check if agents are connected
bool Network::connected(int A, int B) {
    bool AtoB = this->network[A]->connected(B);
    bool BtoA = this->network[B]->connected(A);

    if((AtoB && !BtoA) || (!AtoB && BtoA)) {
        this->free_memory();
        throw "Inconsistent edges in network! Exiting...";
    }
    else return AtoB;
}

// Connect two agents
void Network::connect(int A, int B) {

    this->edges++;
    this->network[A]->connect(B);
    this->network[B]->connect(A);
    return;
}

// Run the imitation game simulation
void Network::simulation(string filename, int iterations, double noise, double new_signal_prob, double clean_prob) {
    uniform_real_distribution<> prob_dis(0, 1);
    uniform_int_distribution<> agent_dis(0, (this->network.size())-1); 

    // Calculate running time of this function
    auto start = high_resolution_clock::now(); 

    // File to write distributions to
    ofstream file(filename);

    // Loop over iterations 
    for(int i = 0; i < iterations; i++) {

        // Select sender
        Agent* sender = this->network[agent_dis(this->rand_gen)];

        // Select receiver
        uniform_int_distribution<> receiver_dis(0, (sender->edges.size())-1); 
        Agent* receiver = this->network[sender->edges[receiver_dis(this->rand_gen)]];

        /*
            Play the imitation game
        */

        // Sometimes add a distributed new signal 
        if(prob_dis(this->rand_gen) <= new_signal_prob) {
            sender->signals.push_back(new Signal(this->rand_gen, sender->signals));
        }

        // Merge signals until nothing is mergeable
        bool merged = true;
        while(merged) {
            merged = sender->merge_signals(noise);
        }

        // Create message from prototype
        int message_prototype = sender->random_message(this->rand_gen);
        Signal* message = new Signal(sender->signals[message_prototype], this->rand_gen, noise);

        // Send to receiver and create answer from prototype
        int answer_prototype = receiver->receive_message(message, this->rand_gen);
        Signal* answer = new Signal(receiver->signals[answer_prototype], this->rand_gen, noise);

        // Receive replication
        bool success = sender->receive_answer(answer, message_prototype);
        
        // Receive success
        receiver->receive_success(success, answer_prototype, message, noise, this->rand_gen);

        // Clean up
        delete(message);
        delete(answer);

        // Allow agents to remove their signals
        if(prob_dis(this->rand_gen) <= clean_prob) {
            for(int a = 0; a < this->N; a++) {
                this->network[a]->remove_bad_vowels();
            }
        }

        // Write agent signals
        for(int a = 0; a < this->N; a++) {
            this->network[a]->write(file, i, a);
        }
    }

    // Calculate running time and print
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<milliseconds>(stop - start); 
    cout << "Simulation took: " << duration.count() << "milliseconds" << endl;
}

// Free allocated memory
void Network::free_memory() {
    for(int i = 0; i < this->N; i++) {
        this->network[i]->delete_signals();
        delete(this->network[i]);
    }
}