
#ifndef __VECTOR2_H__
#define __VECTOR2_H__

#include <stdio.h>
#include <math.h>

struct Vector2;
Vector2 operator-(const Vector2 &a);
Vector2 operator+(const Vector2 &a, const Vector2 &b);
void operator+=(Vector2 &a, const Vector2 &b);
Vector2 operator-(const Vector2 &a, const Vector2 &b);
void operator-=(Vector2 &a, const Vector2 &b);
Vector2 operator*(const Vector2 &a, float s);
Vector2 operator*(float s, const Vector2 &a);
void operator*=(Vector2 &a, float s);
Vector2 operator/(const Vector2 &a, float s);
void operator/=(Vector2 &a, float s);
float operator*(const Vector2 &a, const Vector2 &b);

struct Vector2 {

    float x, y;

    Vector2(float x, float y): x(x), y(y) {
    }

    Vector2(const Vector2 &other): x(other.x), y(other.y) {
    }

    float normSquared() {
        return x * x + y * y;
    }

    float norm() {
        return sqrtf(normSquared());
    }

    Vector2 getNormalized() {
        return *this / norm();
    }

    void normalize() {
        *this /= norm();
    }

    void print() {
        printf("(%f, %f)", x, y);
    }

};

Vector2 operator-(const Vector2 &a) {
    return Vector2(-a.x, -a.y);
}

Vector2 operator+(const Vector2 &a, const Vector2 &b) {
    return Vector2(a.x + b.x, a.y + b.y);
}

void operator+=(Vector2 &a, const Vector2 &b) {
    a.x += b.x;
    a.y += b.y;
}

Vector2 operator-(const Vector2 &a, const Vector2 &b) {
    return Vector2(a.x - b.x, a.y - b.y);
}

void operator-=(Vector2 &a, const Vector2 &b) {
    a.x -= b.x;
    a.y -= b.y;
}

Vector2 operator*(const Vector2 &a, float s) {
    return Vector2(a.x * s, a.y * s);
}

Vector2 operator*(float s, const Vector2 &a) {
    return Vector2(a.x * s, a.y * s);
}

void operator*=(Vector2 &a, float s) {
    a.x *= s;
    a.y *= s;
}

Vector2 operator/(const Vector2 &a, float s) {
    return Vector2(a.x / s, a.y / s);
}

void operator/=(Vector2 &a, float s) {
    a.x /= s;
    a.y /= s;
}

float operator*(const Vector2 &a, const Vector2 &b) {
    return a.x * b.x + a.y * b.y;
}

#endif
