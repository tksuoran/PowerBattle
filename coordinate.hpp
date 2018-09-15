#ifndef COORDINATE_HPP
#define COORDINATE_HPP

#include <cassert>

struct CoordinateModifier
{
    int x = 0;
    int y = 0;
    int l = 0; // additional y modifier in case x is odd
};

constexpr CoordinateModifier hex_neighbors[] = {
    {  0, -1,  0 }, // north
    {  1,  0, -1 }, // north-east
    {  1,  1, -1 }, // south-east
    {  0,  1,  0 }, // south
    { -1,  1, -1 }, // south-west
    { -1,  0, -1 }  // north-west
};

constexpr int NeighborNorth     = 0;
constexpr int NeighborNorthEast = 1;
constexpr int NeighborSouthEast = 2;
constexpr int NeighborSouth     = 3;
constexpr int NeighborSouthWest = 4;
constexpr int NeighborNorthWest = 5;

class Coordinate
{
public:
    Coordinate() : x(0), y(0)
    {
    }

    Coordinate(int x, int y) : x(x), y(y)
    {
    }

    bool is_even() const
    {
        return (x & 1) == 0;
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

    Coordinate neighbor(int direction) const
    {
        assert(direction >= 0);
        assert(direction < 6);

        Coordinate res(x, y);
        res.x += hex_neighbors[direction].x;
        res.y += hex_neighbors[direction].y;
        if (((res.x & 1) == 0) && (hex_neighbors[direction].l == -1))
        {
            res.y -= 1;
        }
        return res;
    }

    int x;
    int y;
};

#endif // COORDINATE_HPP
