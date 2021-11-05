#ifndef SIGNAL_H
#define SIGNAL_H

#include <stdio.h>
#include <math.h>
#include <bits/stdc++.h> 
#include "util.h"
using namespace std;

// SIGNAL CLASS
class Signal {
    
    public: 

    // Characteristics
    float F1;
    float eF2;
    float h;
    float r;
    float p;

    // Counts
    int used = 0;
    int success = 0;

    // Constructor: random
    Signal(mt19937 gen) { 
        uniform_real_distribution<> prob_dis(0, 1);
        this->h = prob_dis(gen);
        this->r = prob_dis(gen);
        this->p = prob_dis(gen);
        this->to_acoustic();
    };

    // Constructor: articulatory characteristics
    Signal(float h, float r, float p) {
        this->h = h;
        this->r = r;
        this->p = p;
        this->to_acoustic();
    }

    // Constructor: from acoustic characteristics
    Signal(Signal* prototype, float noise, mt19937 gen) {
        h = 0.5;
        r = 0.5;
        p = 0.5;
        this->to_acoustic();

        float min = this->acoustic_distance(prototype);
        float dist;
        int stuck_count = 5;
        do {
            this->shift_closer(prototype, gen);
            dist = this->acoustic_distance(prototype);
            if(dist >= min) {
                stuck_count--;
            } else {
                min = dist;
            }
        } while(stuck_count != 0);
    };

    // Constructor: prototype replication
    Signal(Signal* prototype, float noise_percentage, mt19937 gen, bool _) {

        // Add noise to F1
        uniform_real_distribution<> F1_noise_dis(-(prototype->F1) * noise_percentage/2, prototype->F1 * noise_percentage/2);
        this->F1 = prototype->F1 + F1_noise_dis(gen);

        // Add noise to eF2
        uniform_real_distribution<> eF2_noise_dis(-(prototype->eF2) * noise_percentage/2, prototype->eF2 * noise_percentage/2);
        this->eF2 = prototype->eF2 + eF2_noise_dis(gen);
    }

    // Calculate acoustic formants from articulatory characteristics
    void to_acoustic(); 

    // Shift closer to a given signal
    void shift_closer(Signal* to, mt19937 gen);

    // Distance
    float acoustic_distance(Signal* to);
    float articulatory_distance(Signal* to);
};

#endif