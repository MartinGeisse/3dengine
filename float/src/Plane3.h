
#ifndef __PLANE3_H__
#define __PLANE3_H__

#include <stdio.h>
#include <math.h>

#include "Vector3.h"

struct Plane3 {

    float a, b, c, d;

    inline Plane3() {
    }

    inline Plane3(float a, float b, float c, float d): a(a), b(b), c(c), d(d) {
    }

    inline Plane3(const Plane3 &other): a(other.a), b(other.b), c(other.c), d(other.d) {
    }

    inline float getScale() {
        return sqrtf(a * a + b * b + c * c);
    }

    inline Plane3 getNormalized() {
        float q = 1.0f / getScale();
        return Plane3(a * q, b * q, c * q, d * q);
    }

    inline void normalize() {
        float q = 1.0f / getScale();
        a *= q;
        b *= q;
        c *= q;
        d *= q;
    }

    inline float evaluate(const Vector3 &v) {
        return a * v.x + b * v.y + c * v.z + d;
    }

    inline void print() {
        printf("Plane3(%f, %f, %f, %f)", a, b, c, d);
    }

    // note: only works if normalized!
    inline Vector3 projectPointOntoPlane(const Vector3 &p) {
        float v = evaluate(p);
        return Vector3(p.x - v * a, p.y - v * b, p.z - v * c);
    }

};

inline Plane3 buildPlane3FromPoints(Vector3 a, Vector3 b, Vector3 c) {
    Vector3 n = (c - a) % (b - a);
    n.normalize();
    return Plane3(n.x, n.y, n.z, -(n * a));
}

#endif
