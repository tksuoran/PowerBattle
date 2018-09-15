#include "terrain_palette.hpp"
#include "game.hpp"
#include "globals.hpp"

void TerrainPalette::mouse_event(const SDL_Event& e)
{
    if (e.type != SDL_MOUSEBUTTONDOWN)
    {
        return;
    }
    int x = e.button.x - offset.x;
    int y = e.button.y - offset.y;
    int tx = x / tile_full_width;
    int ty = y / tile_height;
    int tile = tx + ty * basetiles_width;
    if (e.button.button == SDL_BUTTON_LEFT)
    {
        m_game->m_map_editor.left_brush = tile;
    }
    else if (e.button.button == SDL_BUTTON_RIGHT)
    {
        m_game->m_map_editor.right_brush = tile;
    }
}

void TerrainPalette::draw() const
{
    const Renderer &renderer = m_game->m_renderer;
    SDL_Texture *hex_tiles = m_game->m_hex_tiles;

    SDL_Rect src_rect;
    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.w = tile_full_width;
    src_rect.h = tile_height;

    SDL_Rect dst_rect;
    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.w = tile_full_width;
    dst_rect.h = tile_height;
    for (int tx = 0; tx < basetiles_width; ++tx)
    {
        dst_rect.x = offset.x + tx * tile_full_width;
        for (int ty = 0; ty < basetiles_height; ++ty)
        {
            terrain_t terrain = tx + ty * basetiles_width;
            Coordinate &shape = m_game->m_terrain_shapes[terrain];
            src_rect.x = shape.x;
            src_rect.y = shape.y;
            dst_rect.y = offset.y + ty * tile_height;
            renderer.blit(hex_tiles, src_rect, dst_rect);
        }
    }
}
