
#ifndef __MATRIX3_H__
#define __MATRIX3_H__

#include <stdio.h>
#include "Fixed.h"
#include "Vector3.h"

struct Matrix3;

Matrix3 operator*(const Matrix3 &a, const Matrix3 &b);
void operator*=(Matrix3 &a, const Matrix3 &b);
Vector3 operator*(const Matrix3 &a, const Vector3 &v);

struct Matrix3 {

    Fixed m11, m12, m13, m21, m22, m23, m31, m32, m33;

    inline Matrix3() :
        m11(fixedOne), m12(fixedZero), m13(fixedZero),
        m21(fixedZero), m22(fixedOne), m23(fixedZero),
        m31(fixedZero), m32(fixedZero), m33(fixedOne) {
    }

    inline Matrix3(Fixed m11, Fixed m12, Fixed m13, Fixed m21, Fixed m22, Fixed m23, Fixed m31, Fixed m32, Fixed m33):
        m11(m11), m12(m12), m13(m13), m21(m21), m22(m22), m23(m23), m31(m31), m32(m32), m33(m33) {
    }

    inline Matrix3(const Matrix3 &other):
        m11(other.m11), m12(other.m12), m13(other.m13),
        m21(other.m21), m22(other.m22), m23(other.m23),
        m31(other.m31), m32(other.m32), m33(other.m33) {
    }

    inline Fixed determinant() {
        return m11 * m22 * m33 + m12 * m23 * m31 + m13 * m21 * m32
            - m13 * m22 * m31 - m11 * m23 * m32 - m12 * m21 * m33;
    }

    inline Matrix3 getInverse() {
        Fixed a = intToFixed(1) / determinant();
        return Matrix3(

            a * (m22 * m33 - m23 * m32),
            a * (m13 * m32 - m12 * m33),
            a * (m12 * m23 - m13 * m22),

            a * (m23 * m31 - m21 * m33),
            a * (m11 * m33 - m13 * m31),
            a * (m13 * m21 - m11 * m23),

            a * (m21 * m32 - m22 * m31),
            a * (m12 * m31 - m11 * m32),
            a * (m11 * m22 - m12 * m21)

        );
    }

    inline void print() {
        printf("(");
        printFixed(m11);
        printf(", ");
        printFixed(m12);
        printf(", ");
        printFixed(m13);
        printf(", ");
        printFixed(m21);
        printf(", ");
        printFixed(m22);
        printf(", ");
        printFixed(m23);
        printf(", ");
        printFixed(m31);
        printf(", ");
        printFixed(m32);
        printf(", ");
        printFixed(m33);
        printf(")");
    }

};

inline Matrix3 operator*(const Matrix3 &a, const Matrix3 &b) {
    return Matrix3(

        a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31,
        a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32,
        a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33,

        a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31,
        a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32,
        a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33,

        a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31,
        a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32,
        a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33

    );
}

inline void operator*=(Matrix3 &a, const Matrix3 &b) {
    a = a * b;
}

inline Vector3 operator*(const Matrix3 &a, const Vector3 &v) {
    return Vector3(
        a.m11 * v.x + a.m12 * v.y + a.m13 * v.z,
        a.m21 * v.x + a.m22 * v.y + a.m23 * v.z,
        a.m31 * v.x + a.m32 * v.y + a.m33 * v.z
    );
}

#endif
