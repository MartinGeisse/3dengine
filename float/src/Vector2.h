
#ifndef __VECTOR2_H__
#define __VECTOR2_H__

#include <stdio.h>
#include <math.h>

struct Vector2;
inline Vector2 operator-(const Vector2 &a);
inline Vector2 operator+(const Vector2 &a, const Vector2 &b);
inline void operator+=(Vector2 &a, const Vector2 &b);
inline Vector2 operator-(const Vector2 &a, const Vector2 &b);
inline void operator-=(Vector2 &a, const Vector2 &b);
inline Vector2 operator*(const Vector2 &a, float s);
inline Vector2 operator*(float s, const Vector2 &a);
inline void operator*=(Vector2 &a, float s);
inline Vector2 operator/(const Vector2 &a, float s);
inline void operator/=(Vector2 &a, float s);
inline float operator*(const Vector2 &a, const Vector2 &b);

struct Vector2 {

    float x, y;

    inline Vector2(): x(0), y(0) {
    }

    inline Vector2(float x, float y): x(x), y(y) {
    }

    inline Vector2(const Vector2 &other): x(other.x), y(other.y) {
    }

    inline float normSquared() {
        return x * x + y * y;
    }

    inline float norm() {
        return sqrtf(normSquared());
    }

    inline Vector2 getNormalized() {
        return *this / norm();
    }

    inline void normalize() {
        *this /= norm();
    }

    inline void print() {
        printf("(%f, %f)", x, y);
    }

    inline void rotateLeft90() {
        int temp = x;
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

inline Vector2 operator*(const Vector2 &a, float s) {
    return Vector2(a.x * s, a.y * s);
}

inline Vector2 operator*(float s, const Vector2 &a) {
    return Vector2(a.x * s, a.y * s);
}

inline void operator*=(Vector2 &a, float s) {
    a.x *= s;
    a.y *= s;
}

inline Vector2 operator/(const Vector2 &a, float s) {
    return Vector2(a.x / s, a.y / s);
}

inline void operator/=(Vector2 &a, float s) {
    a.x /= s;
    a.y /= s;
}

inline float operator*(const Vector2 &a, const Vector2 &b) {
    return a.x * b.x + a.y * b.y;
}

#endif
