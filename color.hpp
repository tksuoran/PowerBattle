#ifndef COLOR_HPP
#define COLOR_HPP

#include <cstdint>

class Color
{
public:
    Color() :
        r(0U),
        g(0U),
        b(0U),
        a(0U)
    {
    }

    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xffU) :
        r(r),
        g(g),
        b(b),
        a(a)
    {
    }

    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

#endif // COLOR_HPP
