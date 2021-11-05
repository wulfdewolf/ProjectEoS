/*
*
*   SIGNAL
*   
*/
#include "../include/signal.h"

// Calculate acoustic formants from articulatory characteristics
void Signal::to_acoustic() { 

    // Formants
    this->F1 = ((-392 + 392 * this->r) * pow(this->h,2) + (596 - 668 * this->r) * this->h + (-146 + 166 * this->r)) * pow(this->p,2) 
               + ((348 - 348 * this->r) * pow(this->h,2) + (-494 + 606 * this->r) * this->h + (141 - 175 * this->r)) * this->p       
               + ((340 - 72 * this->r) * pow(this->h,2) + (-796 + 108 * this->r) * this->h + (708 - 38 * this->r));

    float F2 = ((-1200 + 1208 * this->r) * pow(this->h,2) + (1320 - 1328 * this->r) * this->h + (118 + 158 * this->r)) * pow(this->p,2) 
               + ((1864 - 1488 * this->r) * pow(this->h,2) + (-2644 + 1510 * this->r) * this->h + (-561 + 221 * this->r)) * this->p
               + ((-670 + 490 * this->r) * pow(this->h,2) + (1355 - 697 * this->r) * this->h + (1517 - 117 * this->r));

    float F3 = ((604 - 604 * this->r) * pow(this->h,2) + (1038 - 1178 * this->r) * this->h + (246 + 566 * this->r)) * pow(this->p,2) 
               + ((-1150 + 1262 * this->r) * pow(this->h,2) + (-1443 + 1313 * this->r) * this->h + (-317 - 483 * this->r)) * this->p
               + ((1130 - 863 * this->r) * pow(this->h,2) + (-315 + 44 * this->r) * this->h + (2427 - 127 * this->r));

    float F4 = ((-1120 + 16 * this->r) * pow(this->h,2) + (1696 - 180 * this->r) * this->h + (500 + 522 * this->r)) * pow(this->p,2) 
               + ((-140 + 240 * this->r) * pow(this->h,2) + (-578 + 214 * this->r) * this->h + (-692 - 419 * this->r)) * this->p
               + ((1480 - 602 * this->r) * pow(this->h,2) + (-1220 + 289 * this->r) * this->h + (3678 - 178 * this->r));

    // Weights
    float c = bark_to_hertz(3.5);
    float w1 = (c - (F3 - F2)) / c;
    float w2 = ((F4 - F3) - (F3 - F2)) / (F4 - F2);

    // Effective second formant
    if(F3 - F2 > c) {
        this->eF2 = F2;
    } else if(F3 - F2 <= c && F4 - F2 > c) {
        this->eF2 = ((2 - w1) * F2 + (w1 * F3)) / 2;
    } else if(F4 - F2 <= c && F3 - F2 < F4 - F3) {
        this->eF2 = (((w2 * F2 + (2 - w2) * F3) / 2) - 1);
    } else if(F4 - F2 <= c && F3 - F2 >= F4 - F3) {
        this->eF2 = (((2 + w2) * F3 - (w2 * F4) / 2) - 1); 
    }
    
};

 // Shift closer to a given signal
 void Signal::shift_closer(Signal* to, mt19937 gen) {

    // Create six neighbors
    vector<Signal*> neighbors;

    float h,r,p;

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
    float min = numeric_limits<float>::infinity();
    int closest = 0;
    for(int i=0; i < 6; i++) {
        float dist = neighbors[i]->acoustic_distance(to);
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

float Signal::acoustic_distance(Signal* to) {
    return sqrt(pow(this->F1 - to->F1, 2) + 0.3 * pow(this->eF2 - to->eF2, 2));
}

float Signal::articulatory_distance(Signal* to) {
    return sqrt(pow(this->h - to->h, 2) + pow(this->r - to->r, 2) + pow(this->p - to->p, 2));
}