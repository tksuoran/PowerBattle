#ifndef MAP_HPP
#define MAP_HPP

#include <functional>
#include <vector>

#include <gsl/span>

#include "coordinate.hpp"
#include "stream.hpp"
#include "types.hpp"

struct MapCell
{
    terrain_t terrain{0U};
    unit_t unit_icon{0U};
};

struct Map
{
    Map(coordinate_t width, coordinate_t height);

    Map(FileReadStream &stream)
    {
        stream.op(m_width);
        stream.op(m_height);
        m_map.resize(m_width * m_height);
        m_map.shrink_to_fit();
        for (auto &cell : m_map)
        {
            stream.op(cell.terrain);
            stream.op(cell.unit_icon);
        }
    }

    void write(FileWriteStream &stream)
    {
        stream.op(m_width);
        stream.op(m_height);
        for (auto &cell : m_map)
        {
            stream.op(cell.terrain);
            stream.op(cell.unit_icon);
        }
    }


    terrain_t get_terrain(Coordinate tile_coordinate) const;

    void set_terrain(Coordinate tile_coordinate, terrain_t terrain_value);

    unit_t get_unit_icon(Coordinate tile_coordinate) const;

    void set_unit_icon(Coordinate tile_coordinate, unit_t unit_icon_value);

    Coordinate wrap(Coordinate in) const;

    Coordinate neighbor(Coordinate position, direction_t direction) const;

    void HexCircle(Coordinate center_position, int r0, int r1, const std::function<void(Coordinate position)> &op);

    // Data
    coordinate_t         m_width{0U};
    coordinate_t         m_height{0U};
    std::vector<MapCell> m_map;
};

#endif // MAP_HPP
