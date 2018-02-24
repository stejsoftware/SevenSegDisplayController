#ifndef _DIGIT_H_
#define _DIGIT_H_

#include <Arduino.h>

//    -  |   A
//   / / | F   B
//    -  |   G
//   / / | E   C
//    -. |   D  DP

enum SEGMENT
{
    A = 1 << 0,
    B = 1 << 6,
    C = 1 << 5,
    D = 1 << 4,
    E = 1 << 3,
    F = 1 << 1,
    G = 1 << 2,
    DP = 1 << 7
};

uint8_t clearSegments(uint8_t digit);
uint8_t clearSegment(uint8_t digit, SEGMENT segment);
uint8_t setSegment(uint8_t digit, SEGMENT segment);
uint8_t setNumber(uint8_t digit, char number);
uint8_t setDecimalPoint(uint8_t digit);

template <class T>
uint8_t numDigits(T number)
{
    uint8_t digits = 0;

    if (number < 0)
        digits = 1;

    while (number)
    {
        number /= 10;
        digits++;
    }

    return digits;
}

#endif // _DIGIT_H_
