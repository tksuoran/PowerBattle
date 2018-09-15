#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <memory>
#include "coordinate.hpp"
#include "map_view.hpp"
#include "map_editor.hpp"
#include "terrain_palette.hpp"
#include "terrain_type.hpp"
#include "unit_type.hpp"
#include "hotbar.hpp"
#include "renderer.hpp"

class Map;

class Game
{
public:
    Game();

    ~Game();

    Map *get_map() const;

    MapView &get_map_view();
    Coordinate get_terrain_shape(uint16_t terrain);

    void init_terrain_shapes();
    void init_unit_shapes();
    void init_terrain_types();
    void load_terrain_defs();
    void load_unit_defs();
    bool init();
    void layout();
    void deinit();
    void main_loop();

    std::vector<Coordinate> const& get_terrain_shapes() const;

    uint16_t get_base_terrain(uint16_t terrain) const;

    uint16_t get_base_terrain(Coordinate tile_coordinate) const;

    uint16_t get_multishape_terrain(uint16_t group, uint16_t neighbor_mask);

    //std::vector<UnitType> unit_types;
    //std::vector<Player> players;
    Renderer                    m_renderer;
    SDL_Texture                 *m_hex_tiles = nullptr;
    SDL_Texture                 *m_unit_tiles = nullptr;

    uint16_t                    m_multigroup_terrain_offset;
    uint16_t                    m_last_terrain;
    std::vector<Coordinate>     m_terrain_shapes;
    std::vector<Coordinate>     m_unit_shapes;
    std::vector<Coordinate>     m_extra_shapes; // border, brush sizes
    std::vector<Coordinate>     m_grid_shapes;  // dotted grid, line grid, mask, target, shade
    std::vector<Coordinate>     m_edge_shapes;  // N, NE, SE, S, SW, NW
    std::vector<TerrainType>    m_terrain_types;
    std::vector<UnitType>       m_unit_types;
    std::unique_ptr<Map>        m_map;
    MapView                     m_map_view;
    MapEditor                   m_map_editor;
    TerrainPalette              m_terrain_palette;
    Hotbar                      m_hotbar;
};

#endif // GAME_HPP
