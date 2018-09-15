#ifndef COLOR_HPP
#define COLOR_HPP

#include <cstdint>

#include "util.hpp"

class Color
{
public:
    Color() = default;

    Color(float r, float g, float b)
        : r(float_to_unorm8(r))
        , g(float_to_unorm8(g))
        , b(float_to_unorm8(b))
    {
    }

    Color(float r, float g, float b, float a)
        : r(float_to_unorm8(r))
        , g(float_to_unorm8(g))
        , b(float_to_unorm8(b))
        , a(float_to_unorm8(a))
    {
    }

    uint8_t r{0xff};
    uint8_t g{0xff};
    uint8_t b{0xff};
    uint8_t a{0xff};
};



#endif // COLOR_HPP
