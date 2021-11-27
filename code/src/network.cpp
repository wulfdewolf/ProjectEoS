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

    // Create all the nodes
    Node* new_node;
    for(int i = 0; i < this->N; i++) {
        new_node = new Node(i);
        this->network.push_back(new_node);
    }

    // Go over the file  to create all the edges
    int nodeA_number, nodeB_number;
    Node* nodeA;
    Node* nodeB;
    while (infile >> nodeA_number >> nodeB_number) {
        nodeA = this->network[nodeA_number];
        nodeB = this->network[nodeB_number];

        if(!nodeA->connected(nodeB_number)) nodeA->connect(nodeB_number);
        if(!nodeB->connected(nodeA_number)) nodeB->connect(nodeA_number);
    }
}

// Print network to file
void Network::print_network(string filename) {

    ofstream file(filename);

    // Print network size and edges as first line
    file << this->N << " " << this->edges << endl;

    for (int i = 0; i < this->N; i++) {

        Node* current_node = this->network[i];

        for (int j = 0; j < current_node->edges.size(); j++) {

            Node* connected_node = this->network[current_node->edges[j]];

            file << current_node->number << " " << connected_node->number << endl;
        }
    }
}

// Check if nodes are connected
bool Network::connected(int A, int B) {
    bool AtoB = this->network[A]->connected(B);
    bool BtoA = this->network[B]->connected(A);

    if((AtoB && !BtoA) || (!AtoB && BtoA)) {
        this->free_memory();
        throw "Inconsistent edges in network! Exiting...";
    }
    else return AtoB;
}

// Connect two nodes
void Network::connect(int A, int B) {

    this->edges++;
    this->network[A]->connect(B);
    this->network[B]->connect(A);
    return;
}

// Run the imitation game simulation
void Network::simulation(string filename, int iterations, double noise) {
    uniform_real_distribution<> prob_dis(0, 1);
    uniform_int_distribution<> node_dis(0, (this->network.size())-1); 

    // Calculate running time of this function
    auto start = high_resolution_clock::now(); 

    // File to write distributions to
    ofstream file(filename);

    // Loop over iterations 
    for(int i = 0; i < iterations; i++) {

        // Select sender
        Node* sender = this->network[node_dis(this->rand_gen)];

        // Select receiver
        uniform_int_distribution<> receiver_dis(0, (sender->edges.size())-1); 
        Node* receiver = this->network[sender->edges[receiver_dis(this->rand_gen)]];

        /*
            Play the imitation game
        */

        // Sometimes add a distributed new signal 
        if(prob_dis(this->rand_gen) <= 0.01) {
            cout << "new\n";
            sender->signals.push_back(new Signal(this->rand_gen, sender->signals));
        }

        // Merge signals until nothing is mergeable
        bool merged = true;
        while(merged) {
            merged = sender->merge_signals(noise);
        }

        // Create message from prototype
        int message_prototype = sender->random_message(this->rand_gen);
        Signal* message = new Signal(sender->signals[message_prototype], noise, this->rand_gen, true);

        // Send to receiver and create answer from prototype
        int answer_prototype = receiver->receive_message(message, noise, this->rand_gen);
        Signal* answer = new Signal(receiver->signals[answer_prototype], noise, this->rand_gen, true);

        // Receive replication
        bool success = sender->receive_answer(answer, message_prototype);
        
        // Receive success
        receiver->receive_success(success, answer_prototype, message, noise, this->rand_gen);

        // Clean up
        delete(message);
        delete(answer);

        // Allow agents to clean up their signals
        if(prob_dis(this->rand_gen) <= 0.1) {
            for(int a = 0; a < this->N; a++) {
                this->network[a]->remove_bad_vowels();
            }
        }

        // Write agent signals
        for(int a = 0; a < this->N; a++) {
            for(int s=0; s < this->network[a]->signals.size(); s++) {
                file << i;
                file << " " << a;
                file << " " << this->network[a]->signals.size();
                file << " " << s;
                file << " " << this->network[a]->signals[s]->success;
                file << " " << this->network[a]->signals[s]->used;
                file << " " << this->network[a]->signals[s]->F1;
                file << " " << this->network[a]->signals[s]->eF2 << endl;
            }
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