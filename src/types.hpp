#ifndef TYPES_HPP
#define TYPES_HPP

#include <cstdint>

using coordinate_t = int16_t;
using direction_t = int;
using terrain_t = uint16_t;
using unit_t = uint16_t;

constexpr terrain_t default_terrain = 2U;

constexpr unit_t no_unit = 0U;

#endif // TYPES_HPP
