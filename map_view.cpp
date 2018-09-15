#include "map_view.hpp"

#include "game.hpp"
#include "map.hpp"
#include "globals.hpp"

MapView::MapView(Game *game) :
    m_game(game)
{
}

void MapView::translate(Coordinate offset)
{
    m_tile_offset = wrap(m_tile_offset + offset);
}

Coordinate MapView::wrap(Coordinate in) const
{
    return m_game->get_map()->wrap(in);
}

bool MapView::hit_tile(Coordinate tile_coordinate) const
{
    Map *map = m_game->get_map();
    return (tile_coordinate.x >= 0) &&
           (tile_coordinate.y >= 0) &&
           (tile_coordinate.x < map->m_width) &&
           (tile_coordinate.y < map->m_height);
}

Coordinate MapView::pixel_to_tile(Coordinate pixel_coordinate) const
{
    assert(pixel_coordinate.x >= 0);
    assert(pixel_coordinate.y >= 0);
    assert(pixel_coordinate.x < extent.x);
    assert(pixel_coordinate.y < extent.y);
    int x_adjust = (tile_full_width - tile_interleave_width) / 2;
    int x = pixel_coordinate.x - offset.x - m_view_offset.x;
    int y = pixel_coordinate.y - offset.y - m_view_offset.y;
    int hx = ((x - x_adjust)) / tile_interleave_width;
    int hy = ((hx & 1) == 1) ? (y / tile_height)
                             : (y - tile_odd_y_offset) / tile_height;
    return wrap(m_tile_offset + Coordinate(hx, hy));
}

Coordinate MapView::tile_to_pixel(Coordinate tile_coordinate) const
{
    Coordinate view_relative_tile_coordinate = wrap(tile_coordinate - m_tile_offset);
    Coordinate result = offset + Coordinate(view_relative_tile_coordinate.x * tile_interleave_width,
                                            view_relative_tile_coordinate.y * tile_height);
    if (view_relative_tile_coordinate.is_even())
    {
        result.y += tile_odd_y_offset;
    }
    return result;
}

void MapView::zoom(int value)
{
    Renderer& renderer = m_game->m_renderer;
    int old_width_in_tiles = extent.x / tile_interleave_width;
    int old_height_in_tiles = extent.y / tile_height;
    renderer.zoom(value);
    m_game->layout();
    int new_width_in_tiles = extent.x / tile_interleave_width;
    int new_height_in_tiles = extent.y / tile_height;
    Coordinate diff((old_width_in_tiles - new_width_in_tiles) / 2,
                    (old_height_in_tiles - new_height_in_tiles) / 2);
    m_tile_offset = wrap(m_tile_offset + diff);
    m_tile_offset.x &= ~0x1;
}

void MapView::draw() const
{
    Map* map = m_game->get_map();
    const Renderer &renderer = m_game->m_renderer;
    SDL_Texture *hex_tiles = m_game->m_hex_tiles;

    int width_in_tiles = extent.x / tile_interleave_width;
    int height_in_tiles = (extent.y - tile_odd_y_offset) / tile_height;
    SDL_Rect src_rect;
    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.w = tile_full_width;
    src_rect.h = tile_height;

    SDL_Rect grid_src_rect;
    if (m_grid > 0)
    {
        grid_src_rect.x = m_game->m_grid_shapes[m_grid - 1].x;
        grid_src_rect.y = m_game->m_grid_shapes[m_grid - 1].y;
        grid_src_rect.w = tile_full_width;
        grid_src_rect.h = tile_height;
    }

    SDL_Rect dst_rect;
    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.w = tile_full_width;
    dst_rect.h = tile_height;
    for (int tx = 0; tx < width_in_tiles; ++tx)
    {
        dst_rect.x = tx * tile_interleave_width;
        int y_offset = (tx & 1) ? 0 : tile_odd_y_offset;
        for (int ty = 0; ty < height_in_tiles; ++ty)
        {
            Coordinate pos = map->wrap(Coordinate(tx, ty) + m_tile_offset);
            uint16_t terrain = map->get_terrain(pos);
            if (terrain >= m_game->m_terrain_shapes.size())
            {
                terrain = 0;
            }
            Coordinate &shape = m_game->m_terrain_shapes[terrain + m_game->m_map_editor.offset];
            src_rect.x = shape.x;
            src_rect.y = shape.y;
            dst_rect.y = (ty * tile_height + y_offset);
            renderer.blit(hex_tiles, src_rect, dst_rect);
            if (m_grid > 0)
            {
                renderer.blit(hex_tiles, grid_src_rect, dst_rect);
            }
        }
    }
}
