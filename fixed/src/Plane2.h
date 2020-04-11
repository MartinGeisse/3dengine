
#ifndef __PLANE2_H__
#define __PLANE2_H__

#include <stdio.h>
#include "Fixed.h"
#include "Vector2.h"

struct Plane2 {

    Fixed a, b, c;

    inline Plane2() {
    }

    inline Plane2(Fixed a, Fixed b, Fixed c): a(a), b(b), c(c) {
    }

    inline Plane2(const Plane2 &other): a(other.a), b(other.b), c(other.c) {
    }

    inline Fixed getScale() {
        return fixedSqrt(a * a + b * b);
    }

    inline Plane2 getNormalized() {
        Fixed q = intToFixed(1) / getScale();
        return Plane2(a * q, b * q, c * q);
    }

    inline void normalize() {
        Fixed q = intToFixed(1) / getScale();
        a *= q;
        b *= q;
        c *= q;
    }

    inline Fixed evaluate(const Vector2 &v) {
        return a * v.x + b * v.y + c;
    }

    inline void print() {
        printf("Plane2(");
        printFixed(a);
        printf(", ");
        printFixed(b);
        printf(", ");
        printFixed(c);
        printf(")");
    }

};

inline Plane2 buildPlane2FromPoints(Vector2 a, Vector2 b) {
    Vector2 n = b - a;
    n.rotateLeft90();
    n.normalize();
    return Plane2(n.x, n.y, -(n * a));
}

#endif
