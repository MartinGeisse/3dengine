
#ifndef __VECTOR2_H__
#define __VECTOR2_H__

#include <stdio.h>
#include "Fixed.h"

struct Vector2;
inline Vector2 operator-(const Vector2 &a);
inline Vector2 operator+(const Vector2 &a, const Vector2 &b);
inline void operator+=(Vector2 &a, const Vector2 &b);
inline Vector2 operator-(const Vector2 &a, const Vector2 &b);
inline void operator-=(Vector2 &a, const Vector2 &b);
inline Vector2 operator*(const Vector2 &a, Fixed s);
inline Vector2 operator*(Fixed s, const Vector2 &a);
inline void operator*=(Vector2 &a, Fixed s);
inline Vector2 operator/(const Vector2 &a, Fixed s);
inline void operator/=(Vector2 &a, Fixed s);
inline Fixed operator*(const Vector2 &a, const Vector2 &b);

struct Vector2 {

    Fixed x, y;

    inline Vector2(): x(fixedZero), y(fixedZero) {
    }

    inline Vector2(Fixed x, Fixed y): x(x), y(y) {
    }

    inline Vector2(const Vector2 &other): x(other.x), y(other.y) {
    }

    inline Fixed normSquared() {
        return x * x + y * y;
    }

    inline Fixed norm() {
        return fixedSqrt(normSquared());
    }

    inline Vector2 getNormalized() {
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
        printf(")");
    }

    inline void rotateLeft90() {
        Fixed temp = x;
        x = -y;
        y = temp;
    }

};

inline Vector2 operator-(const Vector2 &a) {
    return Vector2(-a.x, -a.y);
}

inline Vector2 operator+(const Vector2 &a, const Vector2 &b) {
    return Vector2(a.x + b.x, a.y + b.y);
}

inline void operator+=(Vector2 &a, const Vector2 &b) {
    a.x += b.x;
    a.y += b.y;
}

inline Vector2 operator-(const Vector2 &a, const Vector2 &b) {
    return Vector2(a.x - b.x, a.y - b.y);
}

inline void operator-=(Vector2 &a, const Vector2 &b) {
    a.x -= b.x;
    a.y -= b.y;
}

inline Vector2 operator*(const Vector2 &a, Fixed s) {
    return Vector2(a.x * s, a.y * s);
}

inline Vector2 operator*(Fixed s, const Vector2 &a) {
    return Vector2(a.x * s, a.y * s);
}

inline void operator*=(Vector2 &a, Fixed s) {
    a.x *= s;
    a.y *= s;
}

inline Vector2 operator/(const Vector2 &a, Fixed s) {
    return Vector2(a.x / s, a.y / s);
}

inline void operator/=(Vector2 &a, Fixed s) {
    a.x /= s;
    a.y /= s;
}

inline Fixed operator*(const Vector2 &a, const Vector2 &b) {
    return a.x * b.x + a.y * b.y;
}

#endif
