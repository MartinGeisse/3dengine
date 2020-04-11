
#include <stdio.h>
#include <math.h>
#include "Fixed.h"

const float SCALE = 65536.0;

Fixed floatToFixed(float x) {
    return Fixed::build((int)(x * SCALE));
}

float fixedToFloat(Fixed x) {
    return x.value / SCALE;
}

void Fixed::print() {
    printf("%f", fixedToFloat(*this));
}

Fixed fixedSin(Fixed x) {
    return floatToFixed(sinf(fixedToFloat(x)));
}

Fixed fixedCos(Fixed x) {
    return floatToFixed(cosf(fixedToFloat(x)));
}

Fixed fixedSqrt(Fixed x) {
    return floatToFixed(sqrtf(fixedToFloat(x)));
}
