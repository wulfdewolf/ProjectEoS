/*
*
*   agent
*   
*/
#include "../include/agent.h"

// Methods
// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Check if agents are connected
bool Agent::connected(int number) {
   return count(edges.begin(), edges.end(),number) != 0;
}

// Add an edge
void Agent::connect(int number) {
    edges.push_back(number);
}

// Get a random message
int Agent::random_message(mt19937 gen) {
    int n_signals = signals.size();
    int signal_idx = 0;

    if(n_signals == 0) {

        // Generate a random new signal
        Signal* new_signal = new Signal(gen);
        signals.push_back(new_signal);

    } else {

        // Pick a random signal
        uniform_int_distribution<> signal_dis(0, n_signals-1); 
        signal_idx = signal_dis(gen);
    }

    // Increase signal used counter
    signals[signal_idx]->used++;
    return signal_idx;
}

// Receive a message
int Agent::receive_message(Signal* message, mt19937 gen) {
    int n_signals = signals.size();
    int closest = 0;

    if(n_signals == 0) {
        
        // Generate a new signal based on the received signal
        Signal* new_signal = new Signal(message, gen);
        signals.push_back(new_signal);

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
    signals[closest]->used++;
    return closest;
}

// Receive an answer
bool Agent::receive_answer(Signal* answer, int original) {

    // Calculate distances and keep track of closest one
    double min = numeric_limits<double>::infinity();
    int closest = 0;
    for(int i=0; i < signals.size(); i++) {
        double dist = answer->acoustic_distance_to(signals[i]);
        if(dist < min) {
            min = dist;
            closest = i;
        }
    }

    if(closest == original) {
        signals[original]->success++;
        return true;
    } else {
        return false;
    }
}

// Receive a success/fail answer
void Agent::receive_success(bool success, int original, Signal* message, double noise, mt19937 gen) {

    if(success){
        signals[original]->shift(message, gen);
        signals[original]->success++;
    } else {
        if(signals[original]->used != 0 && (signals[original]->success / signals[original]->used) > 0.5) {

            // If threshold is achieved, move away from the signal
            signals[original]->shift(message, gen, true);

            // And create a new signal that looks like the perceived one 
            Signal* new_signal = new Signal(message, gen);
            signals.push_back(new_signal);

        } else {

            // Otherwise just shift closer
            signals[original]->shift(message, gen);
        }
    }
}

// Remove bad signals
void Agent::remove_bad_vowels() {
    for(int i=0; i < signals.size(); i++) {
        if(signals[i]->used > 5.0 && (signals[i]->success / signals[i]->used) < 0.7) {
            delete_signal(i);
        }
    }
}

// Merge signals
bool Agent::merge_signals(double noise) {
    for(int i=0; i < signals.size(); i++) {
        for(int j=0; j < signals.size(); j++) {
            if(i != j) {
                if(signals[i]->acoustic_distance_to(signals[j]) < (log(1+noise)/0.1719-log(1-noise)/0.1719) 
                   || signals[i]->articulatory_distance_to(signals[j]) < 0.17) {
                     // 0.17 is the minimum distance achievable by shifting phonemes
	                 // RelBark(1+AmbNoise)-RelBark(1-AmbNoise) is the maximum distance a phoneme can be shifted by ambient noise
                    if((signals[i]->used == 0) || (signals[j]->used != 0 && signals[i]->success / signals[i]->used < signals[j]->success < signals[j]->used)) {
                        signals[j]->success += signals[i]->success;
                        signals[j]->used += signals[i]->used;
                        delete_signal(i);
                    } else {
                        signals[i]->success += signals[j]->success;
                        signals[i]->used += signals[j]->used;
                        delete_signal(j);
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

// Calculate repertoire energy
double Agent::repertoire_energy() {
    double energy = 0.0;
    for(int s1 = 0; s1 < signals.size()-1; s1++) {
        for(int s2 = s1+1; s2 < signals.size(); s2++) {
            double distance_to = signals[s1]->acoustic_distance_to(signals[s2]);
            if(distance_to != 0) energy += 1 / pow(distance_to,2);
        }
    }
    return energy;
}

// Reset repertoire
void Agent::reset_repertoire() {
    free();
    signals.clear();
}

// Delete signal
void Agent::delete_signal(int i) {
    delete(signals[i]);
    signals.erase(signals.begin() + i);
}

// Free allocated memory 
void Agent::free() {

    // Free all signals
    for(int i = 0; i < signals.size(); i++) delete(signals[i]);
}

// Printing method
void Agent::write(ostream& outs, int iteration, int agent) {
    for(int s = 0; s < signals.size(); s++) {
        signals[s]->write(outs, iteration, agent, signals.size(), s);
    }
}
