#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <math.h>
#include <bits/stdc++.h> 

using namespace std;

// Bark-frequency conversion
double bark_to_hertz(double bark);
double hertz_to_bark(double hertz); 

// Distance
double acoustic_distance(double aF1, double aF2, double bF1, double bF2);
double acoustic_distance_from_char(double ha, double ra, double pa, double hb, double rb, double pb);
double acoustic_distance_from_char1(double ha, double ra, double pa, double F1b, double F2b);
double articulatory_distance(double ha, double ra, double pa, double hb, double rb, double pb);

// Formants
double to_F1(double h, double r, double p);
double to_F2(double h, double r, double p); 

#endif
