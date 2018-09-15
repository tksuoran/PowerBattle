#include "map.hpp"

Map::Map(int width, int height) :
    m_width(width),
    m_height(height)
{
    m_map.resize(width * height);
    std::fill(std::begin(m_map), std::end(m_map), MapCell());
}

uint16_t Map::get_terrain(Coordinate tile_coordinate)
{
    assert(tile_coordinate.x >= 0);
    assert(tile_coordinate.y >= 0);
    assert(tile_coordinate.x < m_width);
    assert(tile_coordinate.y < m_height);
    return m_map[tile_coordinate.x + tile_coordinate.y * m_width].terrain;
}

void Map::set_terrain(Coordinate tile_coordinate, uint16_t terrain_value)
{
    assert(tile_coordinate.x >= 0);
    assert(tile_coordinate.y >= 0);
    assert(tile_coordinate.x < m_width);
    assert(tile_coordinate.y < m_height);
    m_map[tile_coordinate.x + tile_coordinate.y * m_width].terrain = terrain_value;
}

uint16_t Map::get_unit_icon(Coordinate tile_coordinate)
{
    assert(tile_coordinate.x >= 0);
    assert(tile_coordinate.y >= 0);
    assert(tile_coordinate.x < m_width);
    assert(tile_coordinate.y < m_height);
    return m_map[tile_coordinate.x + tile_coordinate.y * m_width].unit_icon;
}

void Map::set_unit_icon(Coordinate tile_coordinate, uint16_t unit_icon_value)
{
    assert(tile_coordinate.x >= 0);
    assert(tile_coordinate.y >= 0);
    assert(tile_coordinate.x < m_width);
    assert(tile_coordinate.y < m_height);
    m_map[tile_coordinate.x + tile_coordinate.y * m_width].unit_icon = unit_icon_value;
}

Coordinate Map::wrap(Coordinate in) const
{
    Coordinate ret = in;
    while (ret.x >= m_width)
    {
        ret.x -= m_width;
    }
    while (ret.x < 0)
    {
        ret.x += m_width;
    }
    while (ret.y >= m_height)
    {
        ret.y -= m_height;
    }
    while (ret.y < 0)
    {
        ret.y += m_height;
    }
    return ret;
}

Coordinate Map::neighbor(Coordinate position, int direction) const
{
    return wrap(position.neighbor(direction));
}

void Map::HexCircle(Coordinate center_position, int r0, int r1, std::function<void(Coordinate position)> op)
{
    assert(r0 >= 0);
    assert(r1 >= r0);

    if (r0 == 0)
    {
        op(center_position);
        r0 = 1;
    }

    for (int radius = r0; radius <= r1; ++radius)
    {
        Coordinate position = center_position;
        for (int s = 0; s < radius; ++s)
        {
            position = wrap(position.neighbor(NeighborNorth));
        }
        for (int direction = 0; direction < 6; ++direction)
        {
            for (int s = 0; s < radius; ++s)
            {
                position = wrap(position.neighbor((direction + 2) % 6));
                op(position);
            }
        }
    }

}
