#ifndef agent_H
#define agent_H

#include <stdio.h>
#include <bits/stdc++.h> 
#include <limits>
#include <algorithm>
#include "signal.h"
using namespace std;

// AGENT CLASS
class Agent {
public: 

    // Initialisor
    // -----------------------------------------------

    Agent(int init_number) { number = init_number; };


    // Fields
    // -----------------------

    // Number
    int number;

    // Edges
    vector<int> edges;

    // Signals 
    vector<Signal*> signals;


    // Methods
    // --------------------------------------------------------------------------------------------

    // Check if agents are connected
    bool connected(int number);

    // Add an edge
    void connect(int number);

    // Get a random message
    int random_message(mt19937 gen);

    // Receive a message
    int receive_message(Signal* message, mt19937 gen);

    // Receive an answer
    bool receive_answer(Signal* message, int original);

    // Receive a success/fail answer
    void receive_success(bool success, int original, Signal* message, double noise, mt19937 gen);

    // Remove bad signals
    void remove_bad_vowels(); 

    // Merge signals
    bool merge_signals(double noise);

    // Calculate repertoire energy
    double repertoire_energy();

    // Reset repertoire
    void reset_repertoire();

    // Delete a signal
    void delete_signal(int i);

    // Free allocated memory
    void free();

    // Writing method
    void write(ostream& outs, int iteration, int agent);
};

#endif