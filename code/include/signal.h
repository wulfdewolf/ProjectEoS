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

    // Acoustic characteristics
    double F1 = 0.0;
    double F2 = 0.0;
    double F3 = 0.0;
    double F4 = 0.0;

    // Articulatory characteristics
    double h = 0.0;
    double r = 0.0;
    double p = 0.0;

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
                tot_dist += others[j]->acoustic_distance_to(rd_h, rd_r, rd_p);
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
    Signal(Signal* prototype, mt19937 gen) {
        h = 0.5;
        r = 0.5;
        p = 0.5;
        to_acoustic();

        double min = prototype->acoustic_distance_to(this);
        double dist;
        bool progress;
        while(true) {
            this->shift(prototype, gen);
            dist = prototype->acoustic_distance_to(this);
            if(dist >= min) {
                break;
            } else {
                min = dist;
            }
        }
    };

    // Constructor: prototype replication
    Signal(Signal* prototype, mt19937 gen, double ac_noise, double ar_noise=0) {
        uniform_int_distribution<> art_noise_dis(-ar_noise/2, ar_noise/2);
        uniform_int_distribution<> noise_dis(-ac_noise/2, ac_noise/2);

        // Add noise to articulatory characteristics
        this->h = prototype->h + art_noise_dis(gen);
        this->r = prototype->r + art_noise_dis(gen);
        this->p = prototype->p + art_noise_dis(gen);

        // Calculate formants
        this->to_acoustic();

        // Add noise to formants 
        this->F1 = prototype->F1 + prototype->F1 * noise_dis(gen);
        this->F2 = prototype->F2 + prototype->F2 * noise_dis(gen);
        this->F3 = prototype->F3 + prototype->F3 * noise_dis(gen);
        this->F4 = prototype->F4 + prototype->F4 * noise_dis(gen);
    }

    // Calculate acoustic formants from articulatory characteristics
    void to_acoustic(); 

    // Shift closer to a given signal
    void shift(Signal* to, mt19937 gen, bool away=false);

    // Acoustic distance
    double acoustic_distance_to(Signal* to);
    double acoustic_distance_to(double to_h, double to_r, double to_p);
    double articulatory_distance_to(Signal* to);

    // Printing method
    void write(ostream& outs, int iteration, int agent, int size, int signal);
};


#endif