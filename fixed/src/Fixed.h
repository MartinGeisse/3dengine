
#ifndef FIXED_H
#define FIXED_H

#include <stdio.h>

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

    // Check for division by zero.
    // We want to check this specifically because Linux is so braindead that it describes a division by zero as
    // a "floating point exception" even though no floating point operation is involved. We do have actual
    // floating point code to emulate fixed-point operations and we want to distinguish the errors.
    if (b.value == 0) {
        printf("division by zero\n");
        exit(1);
    }

    // Handle sign, so the actual division is unsigned / unsigned.
    unsigned int aabs, babs;
    bool negative;
    if (a.value < 0) {
        if (b.value < 0) {
            aabs = -a.value;
            babs = -b.value;
            negative = false;
        } else {
            aabs = -a.value;
            babs = b.value;
            negative = true;
        }
    } else {
        if (b.value < 0) {
            aabs = a.value;
            babs = -b.value;
            negative = true;
        } else {
            aabs = a.value;
            babs = b.value;
            negative = false;
        }
    }

    // This loop takes 48 iterations. I think we might get this down to 32 if we accept that result overflow produces
    // a result that is different from the correct result truncated to 32 bits: The first 16 iterations produce bits
    // 32-47, so if no overflow occurs, these bits are zero and the current working dividend after those 16 bits is
    // still the original dividend.
    unsigned int workingDividend = 0;
    unsigned int result = 0;
    for (int i = 0; i < 48; i++) {
        workingDividend = (workingDividend << 1) | (aabs >> 31);
        aabs = aabs << 1;
        result = result << 1;
        if (workingDividend >= babs) {
            workingDividend -= babs;
            result++;
        }
    }

    // apply sign
    return Fixed::build(negative ? -result : result);

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
