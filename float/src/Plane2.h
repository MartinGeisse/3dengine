
#ifndef __PLANE2_H__
#define __PLANE2_H__

#include <stdio.h>
#include <math.h>

#include "Vector2.h"

struct Plane2 {

    float a, b, c;

    inline Plane2(float a, float b, float c): a(a), b(b), c(c) {
    }

    inline Plane2(const Plane2 &other): a(other.a), b(other.b), c(other.c) {
    }

    inline float getScale() {
        return sqrtf(a * a + b * b);
    }

    inline Plane2 getNormalized() {
        float q = 1.0f / getScale();
        return Plane2(a * q, b * q, c * q);
    }

    inline void normalize() {
        float q = 1.0f / getScale();
        a *= q;
        b *= q;
        c *= q;
    }

    inline float evaluate(const Vector2 &v) {
        return a * v.x + b * v.y + c;
    }

    inline void print() {
        printf("Plane2(%f, %f, %f)", a, b, c);
    }

};

#endif
