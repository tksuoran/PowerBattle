#ifndef MAP_HPP
#define MAP_HPP

#include <cstdint>
#include <functional>
#include <vector>

#include "coordinate.hpp"

class MapCell
{
public:
    MapCell(uint16_t terrain = 2/*TerrainPlain*/, uint16_t unit_icon = 0U) :
        terrain(terrain),
        unit_icon(unit_icon)
    {
    }

    uint16_t terrain;
    uint16_t unit_icon;
};

class Map
{
public:
    Map(int width, int height);

    uint16_t get_terrain(Coordinate tile_coordinate);

    void set_terrain(Coordinate tile_coordinate, uint16_t terrain_value);

    uint16_t get_unit_icon(Coordinate tile_coordinate);

    void set_unit_icon(Coordinate tile_coordinate, uint16_t unit_icon_value);

    Coordinate wrap(Coordinate in) const;

    Coordinate neighbor(Coordinate position, int direction) const;

    void HexCircle(Coordinate center_position, int r0, int r1, std::function<void(Coordinate position)> op);

    int m_width;    // MXX
    int m_height;   // MYY
    std::vector<MapCell> m_map;
};

#endif // MAP_HPP
