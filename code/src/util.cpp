#include "../include/util.h"

double bark_to_hertz(double bark) {
    double _bark = bark;
	if(bark < 2.0) _bark = (bark - 0.3) / 0.85;
    if(bark > 20.1) _bark = (bark + 4.422) / 1.22;
    return (_bark + 0.53) / (26.28 - _bark);
}

double hertz_to_bark(double hertz) {
    double bark = ((26.81 * hertz) / (1960.0 + hertz)) - 0.53;
	if(bark < 2.0) return bark + 0.15 * (2.0 - bark);
    else if(bark > 20.1) return bark + 0.22 * (bark - 20.1);
    else return bark;
}

void normalise(double *h, double *r, double *p) {
    if(*h > 1) *h = 1.0;
    if(*h < 0) *h = 0.0;
    if(*r > 1) *r = 1.0;
    if(*r < 0) *r = 0.0;
    if(*p > 1) *p = 1.0;
    if(*p < 0) *p = 0.0;
}

double acoustic_distance(double aF1, double aF2, double aF3, double aF4, double bF1, double bF2, double bF3, double bF4) {
    double aeF2 = to_eF2(aF2, aF3, aF4);
    double beF2 = to_eF2(bF2, bF3, bF4);
    return sqrt(pow(aF1 - bF1, 2) + 0.5 * pow(aeF2 - beF2, 2));
}

double acoustic_distance(double aF1, double aF2, double aF3, double aF4, double bh, double br, double bp) {
    double bF1 = to_F1(bh, br, bp);
    double bF2 = to_F2(bh, br, bp);
    double bF3 = to_F3(bh, br, bp);
    double bF4 = to_F4(bh, br, bp);
    return acoustic_distance(aF1, aF2, aF3, aF4, bF1, bF2, bF3, bF4);
}

double articulatory_distance(double ah, double ar, double ap, double bh, double br, double bp) {
    return sqrt(pow(ah - bh, 2) + pow(ar - br, 2) + pow(ap - bp, 2));
}
 
double to_F1(double h, double r, double p) {
    return hertz_to_bark(((-392.0 + 392.0 * r) * pow(h,2) + (596.0 - 668.0 * r) * h + (-146.0 + 166.0 * r)) * pow(p,2) 
                         + ((348.0 - 348.0 * r) * pow(h,2) + (-494.0 + 606.0 * r) * h + (141.0 - 175.0 * r)) * p       
                         + ((340.0 - 72.0 * r) * pow(h,2) + (-796.0 + 108.0 * r) * h + (708.0 - 38.0 * r)));
}

double to_F2(double h, double r, double p) { 
    return hertz_to_bark(((-1200.0 + 1208.0 * r) * pow(h,2) + (1320.0 - 1328.0 * r) * h + (118.0 - 158.0 * r)) * pow(p,2) 
                         + ((1864.0 - 1488.0 * r) * pow(h,2) + (-2644.0 + 1510.0 * r) * h + (-561.0 + 221.0 * r)) * p
                         + ((-670.0 + 490.0 * r) * pow(h,2) + (1355.0 - 697.0 * r) * h + (1517.0 - 117.0 * r)));
}

double to_F3(double h, double r, double p) { 
    return hertz_to_bark(((604.0 - 604.0 * r) * pow(h,2) + (1038.0 - 1178.0 * r) * h + (246.0 + 566.0 * r)) * pow(p,2) 
                         + ((-1150.0 + 1262.0 * r) * pow(h,2) + (-1443.0 + 1313.0 * r) * h + (-317.0 - 483.0 * r)) * p
                         + ((1130.0 - 863.0 * r) * pow(h,2) + (-315.0 + 44.0 * r) * h + (2427.0 - 127.0 * r)));
}

double to_F4(double h, double r, double p) { 
    return hertz_to_bark(((-1120.0 + 16.0 * r) * pow(h,2) + (1696.0 - 180.0 * r) * h + (500.0 + 522.0 * r)) * pow(p,2) 
                         + ((-140.0 + 240.0 * r) * pow(h,2) + (-578.0 + 214.0 * r) * h + (-692.0 - 419.0 * r)) * p
                         + ((1480.0 - 602.0 * r) * pow(h,2) + (-1220.0 + 289.0 * r) * h + (3678.0 - 178.0 * r)));
}

double to_eF2(double F2, double F3, double F4) { 
    double c = 3.5;

    if((F3 - F2) > c) return F2;
	else {
        double w1 = (c - (F3 - F2)) / c;
		double w2 = ((F4 - F3) - (F3 - F2)) / (F4 - F2);
		if (w2 < 0) w2 = -w2;

		if((F4 - F2) > c) return ((2.0 - w1) * F2 + w1 * F3) / 2.0;
		else if ((F3 - F2) < (F4 - F3)) return ((w2 * F2 + (2.0 - w2) * F3) / 2.0) - 1.0;
		else return (((2.0 - w2) * F3 + w2 * F4) / 2.0) - 1.0;
	}
}