/*
*
*   NETWORK
*   
*/
#include "../include/network.h"

// Initialisor
// ----------------------------------------------------------------------------

// Initialisor for when a network is to be generated
Network::Network(int population_size, mt19937 rand_gen, int edges)
    : population_size(population_size),
      rand_gen(rand_gen),
      edges(edges) {
    
    // Subclasses should implement generation process
}

// Initialisor for when a network is to be read from a file
Network::Network(string filename) {

    ifstream infile(filename);

    // Network initialisation from file
    infile >> population_size >> edges;

    // Create all the agents
    Agent* new_agent;
    for(int i = 0; i < population_size; i++) {
        new_agent = new Agent(i);
        network.push_back(new_agent);
    }

    // Go over the file  to create all the edges
    int agentA_number, agentB_number;
    Agent* agentA;
    Agent* agentB;
    while (infile >> agentA_number >> agentB_number) {
        agentA = network[agentA_number];
        agentB = network[agentB_number];

        if(!agentA->connected(agentB_number)) agentA->connect(agentB_number);
        if(!agentB->connected(agentA_number)) agentB->connect(agentA_number);
    }
}


// Methods
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

// Write network to file
void Network::write_network(string filename) {

    ofstream file(filename);

    // Print network size and edges as first line
    file << population_size << " " << edges << endl;

    for (int i = 0; i < population_size; i++) {

        Agent* current_agent = network[i];

        for (int j = 0; j < current_agent->edges.size(); j++) {

            Agent* connected_agent = network[current_agent->edges[j]];

            file << current_agent->number << " " << connected_agent->number << endl;
        }
    }
}

// Check if agents are connected
bool Network::connected(int A, int B) {
    bool AtoB = network[A]->connected(B);
    bool BtoA = network[B]->connected(A);

    if((AtoB && !BtoA) || (!AtoB && BtoA)) {
        free();
        throw "Inconsistent edges in network! Exiting...";
    }
    else return AtoB;
}

// Connect two agents
void Network::connect(int A, int B) {
    edges++;
    network[A]->connect(B);
    network[B]->connect(A);
    return;
}

// Run the imitation game simulation
void Network::simulation(int average_over, int iterations, double noise, double new_signal_prob, double clean_prob, vector<int> write_iterations, string filename) {
    uniform_real_distribution<> prob_dis(0, 1);
    uniform_int_distribution<> agent_dis(0, (network.size())-1); 

    // Calculate running time of this function
    auto start = high_resolution_clock::now(); 

    // Files to write results to
    ofstream repertoire_file("repertoires_" + filename);
    ofstream statistics_file("statistics_" + filename);

    // Loop over average_over
    for(int a = 0; a < average_over; a++) {
        double success_counter = 0;

        // Loop over iterations 
        for(int i = 1; i < iterations + 1; i++) {

            // Select sender
            Agent* sender = network[agent_dis(rand_gen)];

            // Select receiver
            uniform_int_distribution<> receiver_dis(0, (sender->edges.size())-1); 
            Agent* receiver = network[sender->edges[receiver_dis(rand_gen)]];

            /*
                Play the imitation game
            */

            // Sometimes add a distributed new signal 
            if(prob_dis(rand_gen) <= new_signal_prob) {
                sender->signals.push_back(new Signal(rand_gen, sender->signals));
            }

            // Merge signals until nothing is mergeable
            bool merged = true;
            while(merged) {
                merged = sender->merge_signals(noise);
            }

            // Create message from prototype
            int message_prototype = sender->random_message(rand_gen);
            Signal* message = new Signal(sender->signals[message_prototype], rand_gen, noise);

            // Send to receiver and create answer from prototype
            int answer_prototype = receiver->receive_message(message, rand_gen);
            Signal* answer = new Signal(receiver->signals[answer_prototype], rand_gen, noise);

            // Receive replication
            bool success = sender->receive_answer(answer, message_prototype);
            if(success) success_counter++;

            // Receive success
            receiver->receive_success(success, answer_prototype, message, noise, rand_gen);

            // Clean up
            delete(message);
            delete(answer);

            // Allow agents to remove their signals
            if(prob_dis(rand_gen) <= clean_prob) {
                for(int agent = 0; agent < population_size; agent++) {
                    network[agent]->remove_bad_vowels();
                }
            }

            // Write agent signals (only for last average run and per given timestep)
            if(a == average_over-1 && find(write_iterations.begin(), write_iterations.end(), i) != write_iterations.end()) {
                for(int agent = 0; agent < population_size; agent++) {
                    network[agent]->write(repertoire_file, i, agent);
                }
            }
        }

        // Calculate average repertoire size
        double average_repertoire_size = 0.0;
        for(int agent = 0; agent < population_size; agent++) {
            average_repertoire_size += network[agent]->signals.size();
        }

        // Calculate average repertoire energy
        double average_repertoire_energy = 0.0;
        for(int agent = 0; agent < population_size; agent++) {
            average_repertoire_energy += network[agent]->repertoire_energy();
        }

        // Reset agents' repertoires
        for(int agent = 0; agent < population_size; agent++) {
            network[agent]->reset_repertoire();
        }

        // Write average entry
        statistics_file << success_counter              / iterations        << " ";
        statistics_file << average_repertoire_size      / population_size   << " ";
        statistics_file << average_repertoire_energy    / population_size   << endl;

    }

    // Close files
    repertoire_file.close();
    statistics_file.close();

    // Calculate running time and print
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<milliseconds>(stop - start); 
    cout << "Simulation took: " << duration.count() << "milliseconds" << endl;
}

// Free allocated memory
void Network::free() {

    // Free all agents
    for(int i = 0; i < population_size; i++) {

        // Tell agents to free their allocated memory
        network[i]->free();
        delete(network[i]);
    }
}