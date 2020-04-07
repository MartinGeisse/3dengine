
#ifndef __TRANSFORM3_H__
#define __TRANSFORM3_H__

#include <stdio.h>
#include <math.h>

#include "Matrix3.h"
#include "Vector3.h"

struct Transform3;
Transform3 operator*(const Transform3 &a, const Transform3 &b);
void operator*=(Transform3 &a, const Transform3 &b);
Vector3 operator*(const Transform3 &a, const Vector3 &v);

struct Transform3 {

    Matrix3 m;
    Vector3 v;

    Transform3(const Matrix3 &m, const Vector3 &v): m(m), v(v) {
    }

    Transform3(const Transform3 &other): m(other.m), v(other.v) {
    }

    void print() {
        printf("Transform(");
        m.print();
        printf(", ");
        v.print();
        printf(")");
    }

};

Transform3 operator*(const Transform3 &a, const Transform3 &b) {
    return Transform3(a.m * b.m, a.m * b.v + a.v);
}

void operator*=(Transform3 &a, const Transform3 &b) {
    a = a * b;
}

Vector3 operator*(const Transform3 &a, const Vector3 &v) {
    return a.m * v + a.v;
}

#endif
