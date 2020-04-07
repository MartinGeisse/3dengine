
#ifndef __PLANE2_H__
#define __PLANE2_H__

#include <stdio.h>
#include <math.h>

#include "vector2.h"

struct Plane2 {

    float a, b, c;

    Plane2(float a, float b, float c): a(a), b(b), c(c) {
    }

    Plane2(const Plane2 &other): a(other.a), b(other.b), c(other.c) {
    }

    float getScale() {
        return sqrtf(a * a + b * b);
    }

    Plane2 getNormalized() {
        float q = 1.0f / getScale();
        return Plane2(a * q, b * q, c * q);
    }

    void normalize() {
        float q = 1.0f / getScale();
        a *= q;
        b *= q;
        c *= q;
    }

    float evaluate(const Vector2 &v) {
        return a * v.x + b * v.y + c;
    }

    void print() {
        printf("Plane2(%f, %f, %f)", a, b, c);
    }

};

#endif
