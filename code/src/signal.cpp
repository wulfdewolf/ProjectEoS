/*
*
*   SIGNAL
*   
*/
#include "../include/signal.h"

// Calculate acoustic formants from articulatory characteristics
void Signal::to_acoustic() { 
    this->F1 = to_F1(this->h, this->r, this->p);
    this->eF2 = to_F2(this->h, this->r, this->p);
};

 // Shift closer to a given signal
 void Signal::shift_closer(Signal* to, mt19937 gen) {

    // Create six neighbors
    vector<vector<double>> combs;
    double _h, _r, _p;

    for(int add_h=0; add_h < 2; add_h++) {
        add_h ? _h = this->h + 0.1 : _h = this->h - 0.1;
        for(int add_r=0; add_r < 2; add_r++) {
            add_r ? _r = this->r + 0.1 : _r = this->r - 0.1;
            for(int add_p=0; add_p < 2; add_p++) {
                add_p ? _p = this->p + 0.1 : _p = this->p - 0.1;
                vector<double> comb;
                comb.push_back(_h);
                comb.push_back(_r);
                comb.push_back(_p);
                combs.push_back(comb);
            }
        }
    }

    // Find closest
    double min = numeric_limits<double>::infinity();
    int closest = 0;
    for(int i=0; i < combs.size()-1; i++) {
        double dist = acoustic_distance_from_char1(combs[i][0], combs[i][1], combs[i][2], to->F1, to->eF2);
        if(dist < min) {
            min = dist;
            closest = i;
        }
    }

    // Take over closest's values
    this->h = combs[closest][0];
    this->r = combs[closest][1];
    this->p = combs[closest][2];
    this->to_acoustic();
}