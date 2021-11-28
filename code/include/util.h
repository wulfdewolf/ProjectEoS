#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <math.h>
#include <bits/stdc++.h> 
using namespace std;

// Articulatory parameters normalisation
void normalise(double *h, double *r, double *p);

// Bark-frequency conversion
double bark_to_hertz(double bark);
double hertz_to_bark(double hertz); 

// Distance
double acoustic_distance(double aF1, double aF2, double aF3, double aF4, double bF1, double bF2, double bF3, double bF4);
double acoustic_distance(double aF1, double aF2, double aF3, double aF4, double bh, double br, double bp);
double articulatory_distance(double ah, double ar, double ap, double bh, double br, double bp);

// Formants
double to_F1(double h, double r, double p);
double to_F2(double h, double r, double p); 
double to_F3(double h, double r, double p); 
double to_F4(double h, double r, double p); 
double to_eF2(double F2, double F3, double F4); 


#endif