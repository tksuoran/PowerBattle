#include <limits>

#include <fmt/format.h>

#include "pixel_lookup.hpp"
#include "texture_util.hpp"

PixelLookup::PixelLookup()
{
    constexpr static int16_t unset = std::numeric_limits<int16_t>::max();

    for (int lx = 0; lx < lut_width; ++lx)
    {
        for (int ly = 0; ly < lut_height; ++ly)
        {
            size_t lut_index = lx + ly * lut_width;
            m_lut[lut_index] = Coordinate(unset, unset);
        }
    }

    tile_mask_t mask_bits = get_mask_bits();
    for (int tx = -1; tx < 2; ++tx)
    {
        int x0 = tx * tile_interleave_width;
        int y_offset = is_odd(tx) ? 0 : tile_odd_y_offset;
        for (int ty = -1; ty < 2; ++ty)
        {
            int y0 = (ty * tile_height + y_offset);
            // (x0, y0) is tile top-left
            for (int mx = 0; mx < tile_full_width; ++mx)
            {
                for (int my = 0; my < tile_height; ++my)
                {
                    int lx = x0 + mx;
                    int ly = y0 + my;
                    if ((lx >= 0) && (ly >= 0) && (lx < lut_width) && (ly < lut_height))
                    {
                        size_t mask_bit_index = mx + my * tile_full_width;
                        size_t lut_index = lx + ly * lut_width;
                        bool mask = mask_bits[mask_bit_index];
                        if (mask)
                        {
                            Coordinate old = m_lut[lut_index];
                            m_lut[lut_index] = Coordinate(tx, ty);
                            if ((old.x != unset) || (old.y != unset))
                            {
                                fmt::print(stderr, "bad\n");
                                std::fflush(stderr);
                                std::abort();
                            }
                        }
                    }
                }
            }
        }
    }

    for (int lx = 0; lx < lut_width; ++lx)
    {
        for (int ly = 0; ly < lut_height; ++ly)
        {
            size_t lut_index = lx + ly * lut_width;
            Coordinate old = m_lut[lut_index];
            if ((old.x == unset) || (old.y == unset))
            {
                fmt::print(stderr, "bad\n");
                std::fflush(stderr);
                std::abort();
            }
        }
    }
}


Coordinate PixelLookup::pixel_to_tile(const Coordinate pixel_coordinate) const
{
    int tx = pixel_coordinate.x / lut_width;
    int ty = pixel_coordinate.y / lut_height;
    int lx = pixel_coordinate.x % lut_width;
    int ly = pixel_coordinate.y % lut_height;
    size_t lut_index = lx + ly * lut_width;
    Coordinate mod = m_lut[lut_index];
    return Coordinate(tx * 2, ty) + mod;
}
