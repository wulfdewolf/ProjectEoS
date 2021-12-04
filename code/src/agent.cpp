/*
*
*   agent
*   
*/
#include "../include/agent.h"

// Check if agents are connected
bool Agent::connected(int number) {
   return count(this->edges.begin(), this->edges.end(),number) != 0;
}

// Add an edge
void Agent::connect(int number) {
    this->edges.push_back(number);
}

// Get a random message
int Agent::random_message(mt19937 gen) {
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
int Agent::receive_message(Signal* message, mt19937 gen) {
    int n_signals = this->signals.size();
    int closest = 0;

    if(n_signals == 0) {
        
        // Generate a new signal based on the received signal
        Signal* new_signal = new Signal(message, gen);
        this->signals.push_back(new_signal);

    } else {
        // Calculate distances and keep track of closest one
        double min = numeric_limits<double>::infinity();
        for(int i=0; i < n_signals; i++) {
            double dist = message->acoustic_distance_to(signals[i]);
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
bool Agent::receive_answer(Signal* answer, int original) {

    // Calculate distances and keep track of closest one
    double min = numeric_limits<double>::infinity();
    int closest = 0;
    for(int i=0; i < this->signals.size(); i++) {
        double dist = answer->acoustic_distance_to(signals[i]);
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
void Agent::receive_success(bool success, int original, Signal* message, double noise, mt19937 gen) {
    cout << original << "\n";

    if(success){
        this->signals[original]->shift(message, gen);
        this->signals[original]->success++;
    } else {
        if(this->signals[original]->used != 0 && (this->signals[original]->success / this->signals[original]->used) > 0.5) {

            // If threshold is achieved, move away from the signal
            this->signals[original]->shift(message, gen, true);

            // And create a new signal that looks like the perceived one 
            Signal* new_signal = new Signal(message, gen);
            this->signals.push_back(new_signal);

        } else {

            // Otherwise just shift closer
            this->signals[original]->shift(message, gen);
        }
    }
}

// Remove bad signals
void Agent::remove_bad_vowels() {
    for(int i=0; i < this->signals.size(); i++) {
        if(this->signals[i]->used > 5.0 && (this->signals[i]->success / this->signals[i]->used) < 0.7) {
            this->delete_signal(i);
        }
    }
}

// Merge signals
bool Agent::merge_signals(double noise) {
    for(int i=0; i < this->signals.size(); i++) {
        for(int j=0; j < this->signals.size(); j++) {
            if(i != j) {
                if(this->signals[i]->acoustic_distance_to(this->signals[j]) < (log(1+noise)/0.1719-log(1-noise)/0.1719) 
                   || this->signals[i]->articulatory_distance_to(this->signals[j]) < 0.17) {
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
void Agent::delete_signal(int i) {
    delete(this->signals[i]);
    this->signals.erase(this->signals.begin() + i);
}

// Delete all signals
void Agent::delete_signals() {
    for(int i = 0; i < this->signals.size(); i++) {
        this->delete_signal(i);
    }
}

// Printing method
void Agent::write(ostream& outs, int iteration, int agent) {
    for(int s = 0; s < this->signals.size(); s++) {
        this->signals[s]->write(outs, iteration, agent, this->signals.size(), s);
    }
}
