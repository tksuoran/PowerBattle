#include <functional>

#include <gsl/gsl_assert>
#include <gsl/gsl_util>

#include "game.hpp"
#include "map.hpp"
#include "map_editor.hpp"
#include "map_view.hpp"

void MapEditor::mouse(bool left_mouse, bool right_mouse, Coordinate mouse_position)
{
    // if (event.type == SDL_MOUSEBUTTONDOWN)
    // {
    //     Map *map = m_game->get_map();
    //     Coordinate position = wrap(pixel_to_tile(mouse_position));
    //     uint16_t terrain = map->get_terrain(position);
    //     fprintf(stdout, "%d, %d: %04x\n", position.x, position.y, terrain);
    //     return;
    // }

    if (!left_mouse && !right_mouse)
    {
        return;
    }

    Map *map = m_game->get_map();
    MapView &view = m_game->m_map_view;

    Coordinate center_position = map->wrap(view.pixel_to_tile(mouse_position));
    if (!view.hit_tile(center_position))
    {
        return;
    }

    terrain_t brush = left_mouse ? left_brush : right_brush;
    last_brush = brush;

    std::function<void(Coordinate)> set_terrain_op = [map, brush] (Coordinate position) -> void
    {
        map->set_terrain(position, brush);
    };

    Game *game = m_game;
    std::function<void(Coordinate)> update_group_terrain = [game, map] (Coordinate position) -> void
    {
        terrain_t terrain = game->get_base_terrain(position);
        int group = game->m_terrain_types[terrain].group;
        if (group < 0)
        {
            return;
        }

        uint32_t neighbor_mask;
        bool promote;
        bool demote;
        size_t counter = 0U;
        do
        {
            promote = false;
            demote = false;
            const MultiShape& multishape = gsl::at(multishapes, group);

            neighbor_mask = 0U;
            for (direction_t direction = DirectionFirst; direction <= DirectionLast; ++direction)
            {
                Coordinate neighbor_position = map->neighbor(position, direction);
                terrain_t neighbor_terrain = game->get_base_terrain(neighbor_position);
                int neighbor_group = game->m_terrain_types[neighbor_terrain].group;
                if (
                    (neighbor_group == group) ||
                    ((multishape.link_group >= 0) &&
                     (neighbor_group == multishape.link_group)) ||
                    ((multishape.link_group2 >= 0) &&
                     (neighbor_group == multishape.link_group2)) ||
                    ((multishape.link_first >= 0) &&
                     (multishape.link_last >= 0) &&
                     (neighbor_terrain >= multishape.link_first) &&
                     (neighbor_terrain <= multishape.link_last)) ||
                    ((multishape.link2_first >= 0) &&
                     (multishape.link2_last >= 0) &&
                     (neighbor_terrain >= multishape.link2_first) &&
                     (neighbor_terrain <= multishape.link2_last))
                )
                {
                    neighbor_mask = neighbor_mask | (1U << direction);
                }
                if ((multishape.demoted >= 0) &&
                    (neighbor_group != group) &&
                    (neighbor_group != multishape.demoted))
                {
                    demote = true;
                }
            }
            promote = (neighbor_mask == direction_mask_all) && (multishape.promoted >= 0);
            Expects((promote && demote) == false);
            if (promote)
            {
                group = multishape.promoted;
            }
            else if (demote)
            {
                group = multishape.demoted;
            }
        }
        while ((promote || demote) && (++counter < 2U));

        terrain_t updated_terrain = game->get_multishape_terrain(group, neighbor_mask);
        map->set_terrain(position, updated_terrain);
    };

    map->HexCircle(center_position, 0, brush_size - 1, set_terrain_op);
    map->HexCircle(center_position, 0, brush_size + 1, update_group_terrain);
}
