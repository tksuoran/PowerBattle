#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <memory>
#include "coordinate.hpp"
#include "map.hpp"
#include "map_view.hpp"
#include "map_editor.hpp"
#include "terrain_palette.hpp"
#include "terrain_type.hpp"
#include "unit_type.hpp"
#include "hotbar.hpp"
#include "pixel_lookup.hpp"
#include "renderer.hpp"

class Game
    : public KeyEventHandler
    , public WindowEventHandler
    , public MouseEventHandler
{
public:
    Game() = default;

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

    void draw();

    // WindowEventHandler
    void resize(int width, int height) override;
    void quit() override;

    // KeyEventHandler
    void key_event(const SDL_Event& e) override;

    // MouseEventHandler
    void mouse_event(const SDL_Event& e) override;

    std::vector<Coordinate> const& get_terrain_shapes() const;

    terrain_t get_base_terrain(terrain_t terrain) const;

    terrain_t get_base_terrain(Coordinate tile_coordinate) const;

    TerrainType const &get_terrain_type(terrain_t terrain) const;

    uint16_t get_multishape_terrain(uint16_t group, uint16_t neighbor_mask);

    //std::vector<UnitType> unit_types;
    //std::vector<Player> players;
    Renderer                    m_renderer;
    SDL_Texture                 *m_hex_tiles = nullptr;
    SDL_Texture                 *m_unit_tiles = nullptr;

    bool                        m_running = true;
    bool                        m_left_mouse = false;
    bool                        m_right_mouse = false;
    bool                        m_mouse_event = false;
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
    PixelLookup                 m_pixelLookup;
};

#endif // GAME_HPP
