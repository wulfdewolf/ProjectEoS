#include "../include/util.h"

double bark_to_hertz(double bark) {
    return 600.0 * sinh(bark / 6.0);
}

double hertz_to_bark(double hertz) {
    return 6.0 * asinh(hertz / 600.0);
}