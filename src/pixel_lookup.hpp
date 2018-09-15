#ifndef PIXEL_LOOKUP_HPP
#define PIXEL_LOOKUP_HPP

#include <array>
#include "coordinate.hpp"
#include "globals.hpp"

class PixelLookup
{
public:
    PixelLookup();

    Coordinate pixel_to_tile(const Coordinate pixel_coordinate) const;

private:
    static const int lut_width  = tile_interleave_width * 2; 
    static const int lut_height = tile_height; 
    static const int lut_size   = lut_width * lut_height;

    using lut_t = std::array<Coordinate, lut_size>;

    lut_t m_lut;
};

#endif // MAPVIEW_HPP
