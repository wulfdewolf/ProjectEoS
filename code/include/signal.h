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
    double F1;
    double eF2;
    double h;
    double r;
    double p;

    // Counts
    double used = 0.0;
    double success = 0.0;

    // Constructor: random
    Signal(mt19937 gen) { 
        uniform_real_distribution<> prob_dis(0, 1);
        this->h = prob_dis(gen);
        this->r = prob_dis(gen);
        this->p = prob_dis(gen);
        this->to_acoustic();
    };

    // Constructor: articulatory characteristics
    Signal(double h, double r, double p) {
        this->h = h;
        this->r = r;
        this->p = p;
        this->to_acoustic();
    }

    // Constructor: from acoustic characteristics
    Signal(Signal* prototype, double noise, mt19937 gen) {
        h = 0.5;
        r = 0.5;
        p = 0.5;
        this->to_acoustic();

        double min = this->acoustic_distance(prototype);
        double dist;
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
    Signal(Signal* prototype, double noise_percentage, mt19937 gen, bool _) {
        uniform_real_distribution<> noise_dis(-noise_percentage/2, noise_percentage/2);

        // Add noise to F1
        this->F1 = prototype->F1 + prototype->F1 * noise_dis(gen);

        // Add noise to eF2
        this->eF2 = prototype->eF2 + prototype->eF2 * noise_dis(gen);
    }

    // Calculate acoustic formants from articulatory characteristics
    void to_acoustic(); 

    // Shift closer to a given signal
    void shift_closer(Signal* to, mt19937 gen);

    // Distance
    double acoustic_distance(Signal* to);
    double articulatory_distance(Signal* to);
};

#endif