/*
*
*   SIGNAL
*   
*/
#include "../include/signal.h"

// Calculate acoustic formants from articulatory characteristics
void Signal::to_acoustic() { 
    normalise(&h, &r, &p);
    this->F1 = to_F1(this->h, this->r, this->p);
    this->F2 = to_F2(this->h, this->r, this->p);
    this->F3 = to_F3(this->h, this->r, this->p);
    this->F4 = to_F4(this->h, this->r, this->p);
}

 // Shift closer to a given signal
 void Signal::shift(Signal* to, mt19937 gen, bool away) {

    // Create neighbors
    vector<vector<double>> combs;
    double _h, _r, _p;

    for(int add_h=0; add_h < 3; add_h++) {
        if(add_h == 0) _h = this->h - 0.1;
        else if(add_h == 1) _h = this->h + 0.1;
        else _h = this->h;
        for(int add_r=0; add_r < 3; add_r++) {
            if(add_r == 0) _r = this->r - 0.1;
            else if(add_r == 1) _r = this->r + 0.1;
            else _r = this->r;
            for(int add_p=0; add_p < 3; add_p++) {
                if(add_p == 0) _p = this->p - 0.1;
                else if(add_p == 1) _p = this->p + 0.1;
                else _p = this->p;
                vector<double> comb;
                normalise(&_h, &_r, &_p);
                comb.push_back(_h);
                comb.push_back(_r);
                comb.push_back(_p);
                combs.push_back(comb);
            }
        }
    }

    // Find best
    double best_dist = numeric_limits<double>::infinity();
    int best = 0;
    for(int i=0; i < combs.size()-1; i++) {
        double dist = to->acoustic_distance_to(combs[i][0], combs[i][1], combs[i][2]);
        if(away) dist = -dist;
        if(dist < best_dist) {
            best_dist = dist;
            best = i;
        }
    }

    // Take over best values
    this->h = combs[best][0];
    this->r = combs[best][1];
    this->p = combs[best][2];
    this->to_acoustic();
}

// Distance
double Signal::acoustic_distance_to(Signal* to) {
    return acoustic_distance(this->F1, this->F2, this->F3, this->F4, to->F1, to->F2, to->F3, to->F4);
}

double Signal::acoustic_distance_to(double to_h, double to_r, double to_p) {
    return acoustic_distance(this->F1, this->F2, this->F3, this->F4, to_h, to_r, to_p);
}

double Signal::articulatory_distance_to(Signal* to) {
    return articulatory_distance(this->h, this->r, this->p, to->h, to->r, to->p);
}

// Printing method
void Signal::write(ostream& outs, int iteration, int agent, int size, int signal) {
    outs << iteration << " " << agent << " " << size << " " << signal << " " <<  this->success << " " << this->used << " " << this->F1 << " " << to_eF2(this->F2, this->F3, this->F4) << "\n";
}