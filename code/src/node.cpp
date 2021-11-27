/*
*
*   NODE
*   
*/
#include "../include/node.h"

// Check if nodes are connected
bool Node::connected(int number) {
   return count(this->edges.begin(), this->edges.end(),number) != 0;
}

// Add an edge
void Node::connect(int number) {
    this->edges.push_back(number);
}

// Get a random message
int Node::random_message(mt19937 gen) {
    int n_signals = this->signals.size();
    int signal_idx = 0;

    if(n_signals == 0) {

        // Generate a random new signal
        Signal* new_signal = new Signal(gen);
        this->signals.push_back(new_signal);

    } else {

        // Pick a random signal
        uniform_int_distribution<> signal_dis(0, n_signals-1); 
        signal_idx = signal_dis(gen);
    }

    // Increase signal used counter
    this->signals[signal_idx]->used++;
    return signal_idx;
}

// Receive a message
int Node::receive_message(Signal* message, double noise, mt19937 gen) {
    int n_signals = this->signals.size();
    int closest = 0;

    if(n_signals == 0) {
        
        // Generate a new signal based on the received signal
        Signal* new_signal = new Signal(message, noise, gen);
        this->signals.push_back(new_signal);

    } else {
        // Calculate distances and keep track of closest one
        double min = numeric_limits<double>::infinity();
        int closest = 0;
        for(int i=0; i < n_signals; i++) {
            double dist = acoustic_distance(message->F1, message->eF2, signals[i]->F1, signals[i]->eF2);
            if(dist < min) {
                min = dist;
                closest = i;
            }
        }
    }

    // Increase signal used counter
    this->signals[closest]->used++;
    return closest;
}

// Receive an answer
bool Node::receive_answer(Signal* answer, int original) {

    // Calculate distances and keep track of closest one
    double min = numeric_limits<double>::infinity();
    int closest = 0;
    for(int i=0; i < this->signals.size(); i++) {
        double dist = acoustic_distance(answer->F1, answer->eF2, signals[i]->F1, signals[i]->eF2);
        if(dist < min) {
            min = dist;
            closest = i;
        }
    }

    if(closest == original) {
        this->signals[original]->success++;
        return true;
    } else {
        return false;
    }
}

// Receive a success/fail answer
void Node::receive_success(bool success, int original, Signal* message, double noise, mt19937 gen) {

    if(success){
        this->signals[original]->shift_closer(message, gen);
        this->signals[original]->success++;
    } else {
        if(this->signals[original]->used != 0 && (this->signals[original]->success / this->signals[original]->used) > 0.5) {
            // If threshold is achieved, create a new signal
            Signal* new_signal = new Signal(message, noise, gen);
            this->signals.push_back(new_signal);

        } else {

            // Otherwise just shift closer
            this->signals[original]->shift_closer(message, gen);
        }
    }
}

// Remove bad signals
void Node::remove_bad_vowels() {
    for(int i=0; i < this->signals.size(); i++) {
        if(this->signals[i]->used > 5.0 && (this->signals[i]->success / this->signals[i]->used) < 0.7) {
                this->delete_signal(i);
        }
    }
}

// Merge signals
bool Node::merge_signals(double noise) {
    for(int i=0; i < this->signals.size(); i++) {
        for(int j=0; j < this->signals.size(); j++) {
            if(i != j) {
                if(acoustic_distance(this->signals[i]->F1, this->signals[j]->eF2, this->signals[j]->F1, this->signals[j]->eF2) < (log(1+noise)/0.1719-log(1-noise)/0.1719) 
                   || articulatory_distance(this->signals[i]->h, this->signals[i]->r, this->signals[i]->p, this->signals[j]->h, this->signals[j]->r, this->signals[j]->p) < 0.17) {
                    // 0.17 is the minimum distance achievable by shifting phonemes
	                 // RelBark(1+AmbNoise)-RelBark(1-AmbNoise) is the maximum distance a phoneme can be shifted by ambient noise
                    if((this->signals[i]->used == 0) || (this->signals[j]->used != 0 && this->signals[i]->success / this->signals[i]->used < this->signals[j]->success < this->signals[j]->used)) {
                        this->signals[j]->success += this->signals[i]->success;
                        this->signals[j]->used += this->signals[i]->used;
                        this->delete_signal(i);
                    } else {
                        this->signals[i]->success += this->signals[j]->success;
                        this->signals[i]->used += this->signals[j]->used;
                        this->delete_signal(j);
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

// Delete signal
void Node::delete_signal(int i) {
    delete(this->signals[i]);
    this->signals.erase(this->signals.begin() + i);
}

// Delete all signals
void Node::delete_signals() {
    for(int i = 0; i < this->signals.size(); i++) {
        this->delete_signal(i);
    }
}
