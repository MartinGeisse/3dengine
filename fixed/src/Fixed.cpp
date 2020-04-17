
#include <stdio.h>
#include <math.h>
#include "Fixed.h"

#if FIXED_IS_FLOAT

Fixed floatToFixed(float x) {
    return x;
}

float fixedToFloat(Fixed x) {
    return x;
}

#else

const float SCALE = 65536.0;

Fixed floatToFixed(float x) {
    return buildFixed((int)(x * SCALE));
}

float fixedToFloat(Fixed x) {
    return x.value / SCALE;
}

#endif

void printFixed(Fixed x) {
    printf("%f", fixedToFloat(x));
}

Fixed fixedSqrt(Fixed x) {
    return floatToFixed(sqrtf(fixedToFloat(x)));
}
