
#include "SevenSegDisplay.h"

uint8_t clearSegments(uint8_t segments)
{
    return 0;
}

uint8_t clearSegment(uint8_t segments, SEGMENT segment)
{
    segments |= segment;
    return segments;
}

uint8_t setSegment(uint8_t segments, SEGMENT segment)
{
    segments |= segment;
    return segments;
}

//    -  |   A
//   / / | F   B
//    -  |   G
//   / / | E   C
//    -. |   D  DP

uint8_t setNumber(uint8_t segments, char number)
{
    switch (number)
    {
    case '1':
        segments |= B | C;
        break;
    case '2':
        segments |= A | B | D | E | G;
        break;
    case '3':
        segments |= A | B | C | D | G;
        break;
    case '4':
        segments |= F | G | B | C;
        break;
    case '5':
        segments |= A | F | G | C | D;
        break;
    case '6':
        segments |= A | F | G | E | D | C;
        break;
    case '7':
        segments |= A | B | C;
        break;
    case '8':
        segments |= A | B | C | D | E | F | G;
        break;
    case '9':
        segments |= A | B | C | D | F | G;
        break;
    case '0':
        segments |= A | B | C | D | E | F;
        break;
    }

    return segments;
}

uint8_t setDecimalPoint(uint8_t segments)
{
    segments |= DP;
    return segments;
}
