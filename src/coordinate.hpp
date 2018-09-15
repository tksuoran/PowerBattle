#ifndef COORDINATE_HPP
#define COORDINATE_HPP

#include <cassert>

#include <gsl/gsl_assert>

#include "types.hpp"

struct CoordinateModifier
{
    coordinate_t x = 0;
    coordinate_t y = 0;
    coordinate_t l = 0; // additional y modifier in case x is odd
};

constexpr CoordinateModifier hex_neighbors[] = {
    {  0, -1,  0 }, // north
    {  1,  0, -1 }, // north-east
    {  1,  1, -1 }, // south-east
    {  0,  1,  0 }, // south
    { -1,  1, -1 }, // south-west
    { -1,  0, -1 }  // north-west
};

constexpr direction_t DirectionFirst          {0};
constexpr direction_t DirectionNorth          {0};
constexpr direction_t DirectionNorthEast      {1};
constexpr direction_t DirectionSouthEast      {2};
constexpr direction_t DirectionSouth          {3};
constexpr direction_t DirectionSouthWest      {4};
constexpr direction_t DirectionNorthWest      {5};
constexpr direction_t DirectionLast           {5};
constexpr direction_t DirectionCount          {6};

constexpr unsigned int direction_mask_all{0b00111111u};

class Coordinate
{
public:
    Coordinate() : x(0), y(0)
    {
    }

    Coordinate(coordinate_t x, coordinate_t y) : x(x), y(y)
    {
    }

    bool is_even() const
    {
        return (x & 1) == 0;
    }

    bool is_odd() const
    {
        return !is_even();
    }

    Coordinate operator*(int scale) const
    {
        return Coordinate(x * scale, y * scale);
    }

    Coordinate operator/(int scale) const
    {
        return Coordinate(x * scale, y * scale);
    }

    Coordinate operator-(Coordinate const& other) const
    {
        return Coordinate(x - other.x, y - other.y);
    }

    Coordinate operator+(Coordinate const& other) const
    {
        return Coordinate(x + other.x, y + other.y);
    }

    Coordinate neighbor(direction_t direction) const
    {
        Expects(direction >= DirectionFirst);
        Expects(direction <= DirectionLast);

        Coordinate res(x, y);
        res.x += hex_neighbors[direction].x;
        res.y += hex_neighbors[direction].y;
        if (res.is_even() && (hex_neighbors[direction].l == -1))
        {
            res.y -= 1;
        }
        return res;
    }

    coordinate_t x;
    coordinate_t y;
};

inline bool is_odd(coordinate_t x)
{
    return x % 2 != 0;
}

#endif // COORDINATE_HPP
