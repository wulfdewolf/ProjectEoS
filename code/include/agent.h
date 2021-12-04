#ifndef agent_H
#define agent_H

#include <stdio.h>
#include <bits/stdc++.h> 
#include <limits>
#include "signal.h"
using namespace std;

// AGENT CLASS
class Agent {
    
    public: 

    // Number
    int number;

    // Edges
    vector<int> edges;

    // Signals 
    vector<Signal*> signals;

    // Constructor
    Agent(int init_number) { number = init_number; };

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

    // Delete a signal
    void delete_signal(int i);

    // Delete all signals
    void delete_signals();

    // Writing method
    void write(ostream& outs, int iteration, int agent);
};

#endif