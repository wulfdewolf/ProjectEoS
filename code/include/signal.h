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
    double F1 = 0.0;
    double eF2 = 0.0;
    double h = 0.0;
    double r = 0.0;
    double p = 0.0;

    // Counts
    double used = 0.0;
    double success = 0.0;
    
    // Constructor: random 
    Signal(mt19937 gen) { 
        uniform_real_distribution<> prob_dis(0, 1);
        h = prob_dis(gen);
        r = prob_dis(gen);
        p = prob_dis(gen);
        to_acoustic();
    }

    // Constructor: random distributed
    Signal(mt19937 gen, vector<Signal*> others) { 
        uniform_real_distribution<> prob_dis(0, 1);
        double max_dist = 0.0; 
        double rd_h = prob_dis(gen);
        double rd_r = prob_dis(gen);
        double rd_p = prob_dis(gen);

        for(int i = 0; i < 5; i++) {
            double tot_dist = 0.0;

            // Calculate distance to all other signals
            for(int j = 0; j < others.size(); j++) {
                tot_dist += acoustic_distance_from_char(others[j]->h, others[j]->r, others[j]->p, rd_h, rd_r, rd_p);
            }

            // Keep track of the best one
            if(tot_dist >= max_dist) {
                max_dist = tot_dist;
                this->h = rd_h;
                this->r = rd_r;
                this->p = rd_p;
                this->to_acoustic();
            }

            // New try
            rd_h = prob_dis(gen);
            rd_r = prob_dis(gen);
            rd_p = prob_dis(gen);
        }
        this->to_acoustic();
    }


    // Constructor: from acoustic characteristics
    Signal(Signal* prototype, double noise, mt19937 gen) {
        h = 0.5;
        r = 0.5;
        p = 0.5;
        to_acoustic();

        double min = acoustic_distance(prototype->F1, prototype->eF2, this->F1, this->eF2);
        double dist;
        int stuck_count = 5;
        do {
            this->shift_closer(prototype, gen);
            dist = acoustic_distance(prototype->F1, prototype->eF2, this->F1, this->eF2);
            if(dist >= min) {
                stuck_count--;
            } else {
                min = dist;
            }
        } while(stuck_count != 0);
    };

    // Constructor: prototype replication
    Signal(Signal* prototype, double noise_percentage, mt19937 gen, bool _) {
        uniform_int_distribution<> noise_dis(-noise_percentage/2, noise_percentage/2);

        // Add noise to F1
        this->F1 = prototype->F1 + prototype->F1 * noise_dis(gen);

        // Add noise to eF2
        this->eF2 = prototype->eF2 + prototype->eF2 * noise_dis(gen);
    }

    // Calculate acoustic formants from articulatory characteristics
    void to_acoustic(); 

    // Shift closer to a given signal
    void shift_closer(Signal* to, mt19937 gen);
};

#endif