#include "../include/util.h"

double bark_to_hertz(double bark) {
    return 600.0 * sinh(bark / 6.0);
}

double hertz_to_bark(double hertz) {
    return 6.0 * asinh(hertz / 600.0);
}

double acoustic_distance(double aF1, double aF2, double bF1, double bF2) {
    return sqrt(pow(aF1 - bF1, 2) + 0.3 * pow(bF2 - bF2, 2));
}

double acoustic_distance_from_char(double ha, double ra, double pa, double hb, double rb, double pb) {
    double aF1 = to_F1(ha, ra, pa);
    double aF2 = to_F2(ha, ra, pa);
    double bF1 = to_F1(hb, rb, pb);
    double bF2 = to_F2(hb, rb, pb);
    return acoustic_distance(aF1, aF2, bF1, bF2);
}

double acoustic_distance_from_char1(double ha, double ra, double pa, double bF1, double bF2) {
    double aF1 = to_F1(ha, ra, pa);
    double aF2 = to_F2(ha, ra, pa);
    return acoustic_distance(aF1, aF2, bF1, bF2);
}

double articulatory_distance(double ha, double ra, double pa, double hb, double rb, double pb) {
    return sqrt(pow(ha - hb, 2) + pow(ra - rb, 2) + pow(pa - pb, 2));
}
 
double to_F1(double h, double r, double p) {
    return hertz_to_bark(((-392.0 + 392.0 * r) * pow(h,2) + (596.0 - 668.0 * r) * h + (-146.0 + 166.0 * r)) * pow(p,2) 
                         + ((348.0 - 348.0 * r) * pow(h,2) + (-494.0 + 606.0 * r) * h + (141.0 - 175.0 * r)) * p       
                         + ((340.0 - 72.0 * r) * pow(h,2) + (-796.0 + 108.0 * r) * h + (708.0 - 38.0 * r)));
}

double to_F2(double h, double r, double p) { 
    double F2 = hertz_to_bark(((-1200.0 + 1208.0 * r) * pow(h,2) + (1320.0 - 1328.0 * r) * h + (118.0 - 158.0 * r)) * pow(p,2) 
               + ((1864.0 - 1488.0 * r) * pow(h,2) + (-2644.0 + 1510.0 * r) * h + (-561.0 + 221.0 * r)) * p
               + ((-670.0 + 490.0 * r) * pow(h,2) + (1355.0 - 697.0 * r) * h + (1517.0 - 117.0 * r)));

    double F3 = hertz_to_bark(((604.0 - 604.0 * r) * pow(h,2) + (1038.0 - 1178.0 * r) * h + (246.0 + 566.0 * r)) * pow(p,2) 
               + ((-1150.0 + 1262.0 * r) * pow(h,2) + (-1443.0 + 1313.0 * r) * h + (-317.0 - 483.0 * r)) * p
               + ((1130.0 - 863.0 * r) * pow(h,2) + (-315.0 + 44.0 * r) * h + (2427.0 - 127.0 * r)));

    double F4 = hertz_to_bark(((-1120.0 + 16.0 * r) * pow(h,2) + (1696.0 - 180.0 * r) * h + (500.0 + 522.0 * r)) * pow(p,2) 
               + ((-140.0 + 240.0 * r) * pow(h,2) + (-578.0 + 214.0 * r) * h + (-692.0 - 419.0 * r)) * p
               + ((1480.0 - 602.0 * r) * pow(h,2) + (-1220.0 + 289.0 * r) * h + (3678.0 - 178.0 * r)));

    // Weights
    double c = 3.5;
    double w1 = (c - (F3 - F2)) / c;
    double w2 = ((F4 - F3) - (F3 - F2)) / (F4 - F2);

    // Effective second formant
    if(F3 - F2 > c) {
        return F2;
    } else if(F3 - F2 <= c && F4 - F2 > c) {
        return ((2.0 - w1) * F2 + (w1 * F3)) / 2.0;
    } else if(F4 - F2 <= c && F3 - F2 < F4 - F3) {
        return (((w2 * F2 + (2.0 - w2) * F3) / 2.0) - 1.0);
    } else if(F4 - F2 <= c && F3 - F2 >= F4 - F3) {
        return (((2.0 + w2) * F3 - (w2 * F4) / 2.0) - 1.0); 
    }
    return 0;
}