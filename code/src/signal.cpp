/*
*
*   SIGNAL
*   
*/
#include "../include/signal.h"

// Calculate acoustic formants from articulatory characteristics
void Signal::to_acoustic() { 

    // Formants
    this->F1 = ((-392.0 + 392.0 * this->r) * pow(this->h,2) + (596.0 - 668.0 * this->r) * this->h + (-146.0 + 166.0 * this->r)) * pow(this->p,2) 
               + ((348.0 - 348.0 * this->r) * pow(this->h,2) + (-494.0 + 606.0 * this->r) * this->h + (141.0 - 175.0 * this->r)) * this->p       
               + ((340.0 - 72.0 * this->r) * pow(this->h,2) + (-796.0 + 108.0 * this->r) * this->h + (708.0 - 38.0 * this->r));

    double F2 = ((-1200.0 + 1208.0 * this->r) * pow(this->h,2) + (1320.0 - 1328.0 * this->r) * this->h + (118.0 - 158.0 * this->r)) * pow(this->p,2) 
               + ((1864.0 - 1488.0 * this->r) * pow(this->h,2) + (-2644.0 + 1510.0 * this->r) * this->h + (-561.0 + 221.0 * this->r)) * this->p
               + ((-670.0 + 490.0 * this->r) * pow(this->h,2) + (1355.0 - 697.0 * this->r) * this->h + (1517.0 - 117.0 * this->r));

    double F3 = ((604.0 - 604.0 * this->r) * pow(this->h,2) + (1038.0 - 1178.0 * this->r) * this->h + (246.0 + 566.0 * this->r)) * pow(this->p,2) 
               + ((-1150.0 + 1262.0 * this->r) * pow(this->h,2) + (-1443.0 + 1313.0 * this->r) * this->h + (-317.0 - 483.0 * this->r)) * this->p
               + ((1130.0 - 863.0 * this->r) * pow(this->h,2) + (-315.0 + 44.0 * this->r) * this->h + (2427.0 - 127.0 * this->r));

    double F4 = ((-1120.0 + 16.0 * this->r) * pow(this->h,2) + (1696.0 - 180.0 * this->r) * this->h + (500.0 + 522.0 * this->r)) * pow(this->p,2) 
               + ((-140.0 + 240.0 * this->r) * pow(this->h,2) + (-578.0 + 214.0 * this->r) * this->h + (-692.0 - 419.0 * this->r)) * this->p
               + ((1480.0 - 602.0 * this->r) * pow(this->h,2) + (-1220.0 + 289.0 * this->r) * this->h + (3678.0 - 178.0 * this->r));

    // Weights
    double c = bark_to_hertz(3.5);
    double w1 = (c - (F3 - F2)) / c;
    double w2 = ((F4 - F3) - (F3 - F2)) / (F4 - F2);

    // Effective second formant
    if(F3 - F2 > c) {
        this->eF2 = F2;
    } else if(F3 - F2 <= c && F4 - F2 > c) {
        this->eF2 = ((2.0 - w1) * F2 + (w1 * F3)) / 2.0;
    } else if(F4 - F2 <= c && F3 - F2 < F4 - F3) {
        this->eF2 = (((w2 * F2 + (2.0 - w2) * F3) / 2.0) - 1.0);
    } else if(F4 - F2 <= c && F3 - F2 >= F4 - F3) {
        this->eF2 = (((2.0 + w2) * F3 - (w2 * F4) / 2.0) - 1.0); 
    }
    
};

 // Shift closer to a given signal
 void Signal::shift_closer(Signal* to, mt19937 gen) {

    // Create six neighbors
    vector<Signal*> neighbors;

    double h,r,p;

    for(int add_h=0; add_h < 2; add_h++) {
        h = this->h;
        add_h ? h += 0.1 : h -= 0.1;
        for(int add_r=0; add_r < 2; add_r++) {
            r = this->r;
            add_r ? r += 0.1 : r -= 0.1;
            for(int add_p=0; add_p < 2; add_p++) {
                p = this->p;
                add_p ? p += 0.1 : p -= 0.1;    

                Signal* neighbor = new Signal(h, r, p);
                neighbors.push_back(neighbor);
            }
        }
    }

    // Find closest
    double min = numeric_limits<double>::infinity();
    int closest = 0;
    for(int i=0; i < 6; i++) {
        double dist = neighbors[i]->acoustic_distance(to);
        if(dist < min) {
            min = dist;
            closest = i;
        }
    }

    // Take over closest's values
    this->h = neighbors[closest]->h;
    this->r = neighbors[closest]->r;
    this->p = neighbors[closest]->p;
    this->to_acoustic();

    // Delete neighbors
    for(int i=0; i < neighbors.size(); i++) {
        delete(neighbors[i]);
    }
}

double Signal::acoustic_distance(Signal* to) {
    return sqrt(pow(this->F1 - to->F1, 2) + 0.3 * pow(this->eF2 - to->eF2, 2));
}

double Signal::articulatory_distance(Signal* to) {
    return sqrt(pow(this->h - to->h, 2) + pow(this->r - to->r, 2) + pow(this->p - to->p, 2));
}