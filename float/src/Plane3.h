
#ifndef __PLANE3_H__
#define __PLANE3_H__

#include <stdio.h>
#include <math.h>

#include "Vector3.h"

struct Plane3 {

    float a, b, c, d;

    Plane3(float a, float b, float c, float d): a(a), b(b), c(c), d(d) {
    }

    Plane3(const Plane3 &other): a(other.a), b(other.b), c(other.c), d(other.d) {
    }

    float getScale() {
        return sqrtf(a * a + b * b + c * c);
    }

    Plane3 getNormalized() {
        float q = 1.0f / getScale();
        return Plane3(a * q, b * q, c * q, d * q);
    }

    void normalize() {
        float q = 1.0f / getScale();
        a *= q;
        b *= q;
        c *= q;
        d *= q;
    }

    float evaluate(const Vector3 &v) {
        return a * v.x + b * v.y + c * v.z + d;
    }

    void print() {
        printf("Plane3(%f, %f, %f, %f)", a, b, c, d);
    }

};

#endif
