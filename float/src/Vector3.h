
#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include <stdio.h>
#include <math.h>

struct Vector3;
Vector3 operator-(const Vector3 &a);
Vector3 operator+(const Vector3 &a, const Vector3 &b);
void operator+=(Vector3 &a, const Vector3 &b);
Vector3 operator-(const Vector3 &a, const Vector3 &b);
void operator-=(Vector3 &a, const Vector3 &b);
Vector3 operator*(const Vector3 &a, float s);
Vector3 operator*(float s, const Vector3 &a);
void operator*=(Vector3 &a, float s);
Vector3 operator/(const Vector3 &a, float s);
void operator/=(Vector3 &a, float s);
float operator*(const Vector3 &a, const Vector3 &b);
Vector3 operator%(const Vector3 &a, const Vector3 &b);
Vector3 operator%=(Vector3 &a, const Vector3 &b);

struct Vector3 {

    float x, y, z;

    Vector3(float x, float y, float z): x(x), y(y), z(z) {
    }

    Vector3(const Vector3 &other): x(other.x), y(other.y), z(other.z) {
    }

    float normSquared() {
        return x * x + y * y + z * z;
    }

    float norm() {
        return sqrtf(normSquared());
    }

    Vector3 getNormalized() {
        return *this / norm();
    }

    void normalize() {
        *this /= norm();
    }

    void print() {
        printf("(%f, %f, %f)", x, y, z);
    }

};

Vector3 operator-(const Vector3 &a) {
    return Vector3(-a.x, -a.y, -a.z);
}

Vector3 operator+(const Vector3 &a, const Vector3 &b) {
    return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

void operator+=(Vector3 &a, const Vector3 &b) {
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
}

Vector3 operator-(const Vector3 &a, const Vector3 &b) {
    return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

void operator-=(Vector3 &a, const Vector3 &b) {
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
}

Vector3 operator*(const Vector3 &a, float s) {
    return Vector3(a.x * s, a.y * s, a.z * s);
}

Vector3 operator*(float s, const Vector3 &a) {
    return Vector3(a.x * s, a.y * s, a.z * s);
}

void operator*=(Vector3 &a, float s) {
    a.x *= s;
    a.y *= s;
    a.z *= s;
}

Vector3 operator/(const Vector3 &a, float s) {
    return Vector3(a.x / s, a.y / s, a.z / s);
}

void operator/=(Vector3 &a, float s) {
    a.x /= s;
    a.y /= s;
    a.z /= s;
}

float operator*(const Vector3 &a, const Vector3 &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 operator%(const Vector3 &a, const Vector3 &b) {
    return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

Vector3 operator%=(Vector3 &a, const Vector3 &b) {
    a = a % b;
}

#endif
