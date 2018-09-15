#include "map.hpp"
#include <gsl/gsl_assert>

Map::Map(coordinate_t width, coordinate_t height)
{
    m_width = width;
    m_height = height;
    m_map.resize(width * height);
    m_map.shrink_to_fit();
    std::fill(std::begin(m_map), std::end(m_map), MapCell());
}

terrain_t Map::get_terrain(Coordinate tile_coordinate) const
{
    Expects(tile_coordinate.x >= coordinate_t{0});
    Expects(tile_coordinate.y >= coordinate_t{0});
    Expects(tile_coordinate.x < m_width);
    Expects(tile_coordinate.y < m_height);
    return m_map[tile_coordinate.x + tile_coordinate.y * m_width].terrain;
}

void Map::set_terrain(Coordinate tile_coordinate, terrain_t terrain_value)
{
    Expects(tile_coordinate.x >= coordinate_t{0});
    Expects(tile_coordinate.y >= coordinate_t{0});
    Expects(tile_coordinate.x < m_width);
    Expects(tile_coordinate.y < m_height);
    m_map[tile_coordinate.x + tile_coordinate.y * m_width].terrain = terrain_value;
}

uint16_t Map::get_unit_icon(Coordinate tile_coordinate) const
{
    Expects(tile_coordinate.x >= coordinate_t{0});
    Expects(tile_coordinate.y >= coordinate_t{0});
    Expects(tile_coordinate.x < m_width);
    Expects(tile_coordinate.y < m_height);
    return m_map[tile_coordinate.x + tile_coordinate.y * m_width].unit_icon;
}

void Map::set_unit_icon(Coordinate tile_coordinate, unit_t unit_icon_value)
{
    Expects(tile_coordinate.x >= coordinate_t{0});
    Expects(tile_coordinate.y >= coordinate_t{0});
    Expects(tile_coordinate.x < m_width);
    Expects(tile_coordinate.y < m_height);
    m_map[tile_coordinate.x + tile_coordinate.y * m_width].unit_icon = unit_icon_value;
}

Coordinate Map::wrap(Coordinate in) const
{
    Coordinate ret = in;
    while (ret.x >= m_width)
    {
        ret.x -= m_width;
    }
    while (ret.x < coordinate_t{0})
    {
        ret.x += m_width;
    }
    while (ret.y >= m_height)
    {
        ret.y -= m_height;
    }
    while (ret.y < coordinate_t{0})
    {
        ret.y += m_height;
    }
    return ret;
}

Coordinate Map::neighbor(Coordinate position, direction_t direction) const
{
    return wrap(position.neighbor(direction));
}

void Map::HexCircle(Coordinate center_position, int r0, int r1, const std::function<void(Coordinate position)> &op)
{
    Expects(r0 >= 0);
    Expects(r1 >= r0);

    if (r0 == 0)
    {
        op(center_position);
        r0 = 1;
    }

    constexpr direction_t offset{2};

    for (int radius = r0; radius <= r1; ++radius)
    {
        auto position = center_position;
        for (int s = 0; s < radius; ++s)
        {
            position = wrap(position.neighbor(DirectionNorth));
        }
        for (auto direction = DirectionFirst; direction < DirectionCount; ++direction)
        {
            for (int s = 0; s < radius; ++s)
            {
                position = wrap(position.neighbor((direction + offset) % DirectionCount));
                op(position);
            }
        }
    }
}
