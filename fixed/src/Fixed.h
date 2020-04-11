
#ifndef FIXED_H
#define FIXED_H

struct Fixed {

    int value;

    inline static Fixed build(int value) {
        Fixed result;
        result.value = value;
        return result;
    }

    inline static Fixed fromInt(signed short integral) {
        return Fixed(integral, 0);
    }

    inline Fixed() {
    }

    inline Fixed(signed short integral, unsigned short fractional) : value((((int)integral) << 16) + fractional) {
    }

    void print();

    inline int toInt() {
        return value >> 16;
    }

    inline int getFractionalPart() {
        return value & 0xffff;
    }

};

inline Fixed operator-(Fixed a) {
    return Fixed::build(-a.value);
}

inline Fixed operator+(Fixed a, Fixed b) {
    return Fixed::build(a.value + b.value);
}

inline void operator+=(Fixed &a, Fixed b) {
    a.value += b.value;
}

inline Fixed operator-(Fixed a, Fixed b) {
    return Fixed::build(a.value - b.value);
}

inline void operator-=(Fixed &a, Fixed b) {
    a.value -= b.value;
}

inline Fixed operator*(Fixed a, Fixed b) {
    int ah = a.value >> 16;
    int al = a.value & 0xffff;
    int bh = b.value >> 16;
    int bl = b.value & 0xffff;
    return Fixed::build(
        ((ah * bh) << 16) +
        ah * bl +
        al * bh +
        (((al * bl) >> 16) & 0xffff) // explicit masking because I'm not sure how (signed * unsigned) behaves in C
    );
}

inline Fixed operator*=(Fixed &a, Fixed b) {
    a = a * b;
}

inline Fixed operator/(Fixed a, Fixed b) {
    int p = (a.value & 0xffff0000) / b.value;
    int q = (a.value << 16) / b.value;
    return Fixed::build((p << 16) + q);
}

inline Fixed operator/=(Fixed &a, Fixed b) {
    a = a / b;
}

inline bool operator<(Fixed a, Fixed b) {
    return a.value < b.value;
}

inline bool operator<=(Fixed a, Fixed b) {
    return a.value <= b.value;
}

inline bool operator>(Fixed a, Fixed b) {
    return a.value > b.value;
}

inline bool operator>=(Fixed a, Fixed b) {
    return a.value >= b.value;
}

const Fixed fixedZero(0, 0);
const Fixed fixedOne(1, 0);
const Fixed fixedMinusOne(-1, 0);
const Fixed fixedEpsilon(0, 66); // 1/1000
const Fixed fixedMinusEpsilon(-1, 65536 - 66); // 1/1000

Fixed fixedSin(Fixed x);
Fixed fixedCos(Fixed x);
Fixed fixedSqrt(Fixed x);

Fixed floatToFixed(float x);
float fixedToFloat(Fixed x);

#endif
