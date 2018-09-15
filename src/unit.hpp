#ifndef UNIT_HPP
#define UNIT_HPP

#include <cstddef>
#include <cstdint>
#include "types.hpp"

struct Unit
{
    uint16_t     unit_type;
    coordinate_t x;
    coordinate_t y;
    uint16_t     command;
    uint16_t     level;
    uint16_t     hit_points;
    uint16_t     move_points;
    uint16_t     product_unit_type;
    uint16_t     product_turn_count;
    size_t  manufacturing_factory_id;
};

#endif // UNIT_HPP
