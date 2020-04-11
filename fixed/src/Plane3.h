
#ifndef __PLANE3_H__
#define __PLANE3_H__

#include <stdio.h>
#include "Fixed.h"
#include "Vector3.h"

struct Plane3 {

    Fixed a, b, c, d;

    inline Plane3() {
    }

    inline Plane3(Fixed a, Fixed b, Fixed c, Fixed d): a(a), b(b), c(c), d(d) {
    }

    inline Plane3(const Plane3 &other): a(other.a), b(other.b), c(other.c), d(other.d) {
    }

    inline Fixed getScale() {
        return fixedSqrt(a * a + b * b + c * c);
    }

    inline Plane3 getNormalized() {
        Fixed q = intToFixed(1) / getScale();
        return Plane3(a * q, b * q, c * q, d * q);
    }

    inline void normalize() {
        Fixed q = intToFixed(1) / getScale();
        a *= q;
        b *= q;
        c *= q;
        d *= q;
    }

    inline Fixed evaluate(const Vector3 &v) {
        return a * v.x + b * v.y + c * v.z + d;
    }

    inline void print() {
        printf("Plane3(");
        printFixed(a);
        printf(", ");
        printFixed(b);
        printf(", ");
        printFixed(c);
        printf(", ");
        printFixed(d);
        printf(")");
    }

    // note: only works if normalized!
    inline Vector3 projectPointOntoPlane(const Vector3 &p) {
        Fixed v = evaluate(p);
        return Vector3(p.x - v * a, p.y - v * b, p.z - v * c);
    }

};

inline Plane3 buildPlane3FromPoints(Vector3 a, Vector3 b, Vector3 c) {
    Vector3 n = (c - a) % (b - a);
    n.normalize();
    return Plane3(n.x, n.y, n.z, -(n * a));
}

#endif
