
#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include <stdio.h>
#include "Fixed.h"

struct Vector3;
inline Vector3 operator-(const Vector3 &a);
inline Vector3 operator+(const Vector3 &a, const Vector3 &b);
inline void operator+=(Vector3 &a, const Vector3 &b);
inline Vector3 operator-(const Vector3 &a, const Vector3 &b);
inline void operator-=(Vector3 &a, const Vector3 &b);
inline Vector3 operator*(const Vector3 &a, Fixed s);
inline Vector3 operator*(Fixed s, const Vector3 &a);
inline void operator*=(Vector3 &a, Fixed s);
inline Vector3 operator/(const Vector3 &a, Fixed s);
inline void operator/=(Vector3 &a, Fixed s);
inline Fixed operator*(const Vector3 &a, const Vector3 &b);
inline Vector3 operator%(const Vector3 &a, const Vector3 &b);
inline Vector3 operator%=(Vector3 &a, const Vector3 &b);

struct Vector3 {

    Fixed x, y, z;

    inline Vector3(): x(fixedZero), y(fixedZero), z(fixedZero) {
    }

    inline Vector3(Fixed x, Fixed y, Fixed z): x(x), y(y), z(z) {
    }

    inline Vector3(const Vector3 &other): x(other.x), y(other.y), z(other.z) {
    }

    inline Fixed normSquared() {
        return x * x + y * y + z * z;
    }

    inline Fixed norm() {
        return fixedSqrt(normSquared());
    }

    inline Vector3 getNormalized() {
        return *this / norm();
    }

    inline void normalize() {
        *this /= norm();
    }

    inline void print() {
        printf("(");
        printFixed(x);
        printf(", ");
        printFixed(y);
        printf(", ");
        printFixed(z);
        printf(")");
    }

};

inline Vector3 operator-(const Vector3 &a) {
    return Vector3(-a.x, -a.y, -a.z);
}

inline Vector3 operator+(const Vector3 &a, const Vector3 &b) {
    return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline void operator+=(Vector3 &a, const Vector3 &b) {
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
}

inline Vector3 operator-(const Vector3 &a, const Vector3 &b) {
    return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline void operator-=(Vector3 &a, const Vector3 &b) {
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
}

inline Vector3 operator*(const Vector3 &a, Fixed s) {
    return Vector3(a.x * s, a.y * s, a.z * s);
}

inline Vector3 operator*(Fixed s, const Vector3 &a) {
    return Vector3(a.x * s, a.y * s, a.z * s);
}

inline void operator*=(Vector3 &a, Fixed s) {
    a.x *= s;
    a.y *= s;
    a.z *= s;
}

inline Vector3 operator/(const Vector3 &a, Fixed s) {
    return Vector3(a.x / s, a.y / s, a.z / s);
}

inline void operator/=(Vector3 &a, Fixed s) {
    a.x /= s;
    a.y /= s;
    a.z /= s;
}

inline Fixed operator*(const Vector3 &a, const Vector3 &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vector3 operator%(const Vector3 &a, const Vector3 &b) {
    return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

inline Vector3 operator%=(Vector3 &a, const Vector3 &b) {
    a = a % b;
}

#endif
