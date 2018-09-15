#include <cassert>
#include <sstream>

#include <fmt/format.h>
#include <gsl/gsl_assert>
#include <gsl/gsl_util> // for at

#include "file_util.hpp"
#include "game.hpp"
#include "globals.hpp"
#include "map.hpp"

    Game::~Game()
{
    deinit();
}

Map *Game::get_map() const
{
    return m_map.get();
}

MapView &Game::get_map_view()
{
    return m_map_view;
}

void Game::init_terrain_types()
{
    // { 20, 11, 22, -1, -1, -1, -1 }, // 0 Road
    // { 28, 28, 29, -1, -1, -1, -1 }, // 1 Peak
    // { 41, 41, 44, -1, -1, -1, -1 }, // 2 Forest
    // { 45, 45, 49,  1, -1, 28, 29 }, // 3 Mountain
    // {  5,  5, 18,  5,  6, -1, -1 }, // 4 Water Low
    // {  3,  3,  4,  6, -1, -1, -1 }, // 5 Water Medium
    // {  1,  1,  2, -1, -1, -1, -1 }, // 6 Water Deep

    constexpr terrain_t TerrainRoad       {20U};
    constexpr terrain_t TerrainPeak       {28U};
    constexpr terrain_t TerrainForest     {41U};
    constexpr terrain_t TerrainMountain   {45U};
    constexpr terrain_t TerrainWaterLow   { 5U};
    constexpr terrain_t TerrainWaterMedium{ 3U};
    constexpr terrain_t TerrainWaterDeep  { 1U};

    constexpr int16_t GroupRoad       {0U};
    constexpr int16_t GroupPeak       {1U};
    constexpr int16_t GroupForest     {2U};
    constexpr int16_t GroupMountain   {3U};
    constexpr int16_t GroupWaterLow   {4U};
    constexpr int16_t GroupWaterMedium{5U};
    constexpr int16_t GroupWaterDeep  {6U};

    m_terrain_types.resize(basetiles_width * basetiles_height);
    m_terrain_types[TerrainRoad       ].group = GroupRoad;
    m_terrain_types[TerrainPeak       ].group = GroupPeak;
    m_terrain_types[TerrainForest     ].group = GroupForest;
    m_terrain_types[TerrainMountain   ].group = GroupMountain;
    m_terrain_types[TerrainWaterLow   ].group = GroupWaterLow;
    m_terrain_types[TerrainWaterMedium].group = GroupWaterMedium;
    m_terrain_types[TerrainWaterDeep  ].group = GroupWaterDeep;
}

TerrainType const &Game::get_terrain_type(uint16_t terrain) const
{
    uint16_t base_terrain = get_base_terrain(terrain);
    return m_terrain_types[base_terrain];
}

bool Game::init()
{
    m_renderer.init();

    m_hex_tiles = m_renderer.load_texture("data/graphics/hex_.png");
    m_unit_tiles = m_renderer.load_texture("data/graphics/unit_.png");

    init_terrain_types();
    load_terrain_defs();
    load_unit_defs();

    init_terrain_shapes();
    init_unit_shapes();


#if 0
    std::vector<unsigned char> file = read_file("data/maps/default");
    size_t lo = 1U;
    size_t hi = 0U;
    size_t pos = 0U;
    constexpr unsigned int byte_bits{8};
    uint16_t width = (file[pos + lo]) | (file[pos + hi] << byte_bits); pos += 2;
    uint16_t height = (file[pos + lo]) | (file[pos + hi] << byte_bits); pos += 2;
    m_map = std::make_unique<Map>(width, height);
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Expects(pos < file.size());
            terrain_t terrain = (file[pos + lo]) | (file[pos + hi] << byte_bits); pos += 2;
            Expects(pos < file.size());
            unit_t unit_icon = (file[pos + lo]) | (file[pos + hi] << byte_bits); pos += 2;
            static_cast<void>(unit_icon);
            if (terrain < m_terrain_shapes.size())
            {
                m_map->set_terrain(Coordinate(x, y), terrain);
            }
            else
            {
                fmt::print(stdout, "{}, {}: {}\n", x, y, terrain);
                fflush(stdout);
            }
        }
        pos += 3U;
    }
    //write_file("data/maps/default.new", m_map->m_map.data(), sizeof(MapCell) * m_map->m_map.size());

    {
        FileWriteStream f("data/maps/default.new");
        m_map->write(f);
    }
#endif
    {
        FileReadStream f("data/maps/default.new");
        m_map = std::make_unique<Map>(f);
    }

#if 0
        Expects(m->m_width == m_map->m_width);
        Expects(m->m_height == m_map->m_height);
        for (coordinate_t y = 0; y < m->m_height; ++y)
        {
            for (coordinate_t x = 0; x < m->m_width; ++x)
            {
                Coordinate pos(x, y);
                Expects(m->get_terrain(pos) == m_map->get_terrain(pos));
                Expects(m->get_unit_icon(pos) == m_map->get_unit_icon(pos));
            }
        }
    }
#endif

    m_map_editor.m_game = this;
    m_map_view.m_game = this;
    m_terrain_palette.m_game = this;
    m_terrain_palette.hide();
    m_hotbar.m_game = this;
    layout();

    return true;
}

void Game::resize(int width, int height)
{
    static_cast<void>(width);
    static_cast<void>(height);
    layout();
}

void Game::quit()
{
    m_running = false;
}

void Game::layout()
{
    int width = m_renderer.get_width();
    int height = m_renderer.get_height();
    m_map_view.resize(Coordinate(width, height));
    m_terrain_palette.resize(Coordinate(basetiles_width * tile_full_width,
                                        basetiles_height * tile_height));

    m_terrain_palette.center(Coordinate(width, height));
    constexpr int hotbar_slot_count = 10;
    m_hotbar.resize(Coordinate(hotbar_slot_count * tile_full_width,
                               tile_height));
    m_hotbar.center(Coordinate(width, height / 4));
    constexpr int hotbar_y_offset = tile_height * 1.25F;
    m_hotbar.offset.y = height - hotbar_y_offset;
}

void Game::deinit()
{
	SDL_DestroyTexture(m_hex_tiles);
    SDL_DestroyTexture(m_unit_tiles);
    m_hex_tiles = nullptr;
    m_unit_tiles = nullptr;
    m_renderer.deinit();
}

void Game::draw()
{
    m_renderer.clear(Color(0.125F, 0.25F, 0.5F));
    m_map_view.draw();

    // Top HUD line
    {
        Coordinate mouse_position = m_renderer.mouse_position();
        Map *map = get_map();
        MapView &view = m_map_view;
        //Coordinate tile_position = view.wrap(view.pixel_to_tile(mouse_position));
        Coordinate tile_position = view.pixel_to_tile(mouse_position);
        //Coordinate pixel_position = view.tile_to_pixel_center(tile_position);
        //Coordinate tile_position2 = view.pixel_to_tile(pixel_position);
        //Coordinate tile_position2 = view.wrap(view.pixel_to_tile(pixel_position));
        auto tile = map->get_terrain(tile_position);
        auto terrain = get_terrain_type(tile);

        fmt::memory_buffer buf;
        fmt::format_to(buf, "{} {}, {}", terrain.name, tile_position.x, tile_position.y); 
        //ss << "M: " << mouse_position.x << ", " << mouse_position.y << " ";
        //ss << "T: " << tile_position.x << ", " << tile_position.y << " ";
        //ss << "P: " << pixel_position.x << ", " << pixel_position.y << " ";
        //ss << "T': " << tile_position2.x << ", " << tile_position2.y;
        //view.draw_text(pixel_position.x, pixel_position.y, ss.str());
        //view.draw_text(0, m_renderer.get_height() - 1, ss.str());
        view.draw_text(tile_full_width, 0, buf.data());
        if (tile < m_terrain_shapes.size())
        {
            Coordinate &shape = m_terrain_shapes[tile];
            SDL_Rect src_rect;
            src_rect.x = shape.x;
            src_rect.y = shape.y;
            src_rect.w = tile_full_width;
            src_rect.h = tile_height;
            SDL_Rect dst_rect;
            dst_rect.x = 0;
            dst_rect.y = 0;
            dst_rect.w = tile_full_width;
            dst_rect.h = tile_height;
            m_renderer.blit(m_hex_tiles, src_rect, dst_rect);
        }
    }

    if (m_map_view.hit(m_renderer.mouse_position()))
    {
        Coordinate center_position = m_map->wrap(m_map_view.pixel_to_tile(m_renderer.mouse_position()));
        Game *game = this;
        if (m_map_view.hit_tile(center_position))
        {
            Renderer *renderer = &m_renderer;
            SDL_Texture* texture = m_hex_tiles;
            m_renderer.set_texture_alpha_mode(texture, 0.5F);
            std::function<void(Coordinate)> brush_preview = [renderer, texture, game] (Coordinate position) -> void
            {
                MapEditor &map_editor = game->m_map_editor;
                MapView &map_view = game->m_map_view;
                Map *map = game->get_map();
                terrain_t terrain = map_editor.last_brush;
                Coordinate &shape = game->m_terrain_shapes[terrain + map_editor.offset];
                Coordinate view_relative_tile = map->wrap(position - map_view.m_tile_offset);

                SDL_Rect src_rect;
                src_rect.x = shape.x;
                src_rect.y = shape.y;
                src_rect.w = tile_full_width;
                src_rect.h = tile_height;

                SDL_Rect dst_rect;
                dst_rect.w = tile_full_width;
                dst_rect.h = tile_height;
                dst_rect.x = view_relative_tile.x * tile_interleave_width;
                int y_offset = view_relative_tile.is_odd() ? 0 : tile_odd_y_offset;
                dst_rect.y = (view_relative_tile.y * tile_height + y_offset);
                renderer->blit(texture, src_rect, dst_rect);
            };
            m_map->HexCircle(center_position, 0, m_map_editor.brush_size - 1, brush_preview);
            m_renderer.set_texture_alpha_mode(texture, 1.0F);
        }
    }

    if (m_terrain_palette.visible)
    {
        m_terrain_palette.draw();
    }

    if (m_hotbar.visible)
    {
        m_hotbar.draw();
    }

    m_renderer.present();
}

void Game::key_event(const SDL_Event& e)
{
    if (e.type != SDL_KEYDOWN)
    {
        return;
    }

    switch (e.key.keysym.sym)
    {
        case SDLK_g:
        {
            m_map_view.m_grid++;
            if (m_map_view.m_grid == 3)
            {
                m_map_view.m_grid = 0;
            }
            break;
        }
        case SDLK_w:
        case SDLK_UP:       m_map_view.translate(Coordinate( 0, -4)); break;
        case SDLK_s:
        case SDLK_DOWN:     m_map_view.translate(Coordinate( 0,  4)); break;
        case SDLK_a:
        case SDLK_LEFT:     m_map_view.translate(Coordinate(-4,  0)); break;
        case SDLK_d:
        case SDLK_RIGHT:    m_map_view.translate(Coordinate( 4,  0)); break;
        case SDLK_PAGEUP:
        {
            m_map_view.zoom(1);
            break;
        }
        case SDLK_PAGEDOWN:
        {
            m_map_view.zoom(-1);
            break;
        }
        case SDLK_HOME:
        {
            m_map_editor.brush_size++;
            break;
        }
        case SDLK_END:
        {
            if (m_map_editor.brush_size > 1)
            {
                m_map_editor.brush_size--;
            }
            break;
        }
        case SDLK_ESCAPE:
        {
            m_running = false;
            break;
        }
        default:
        {
            break;
        }
    }
}

void Game::mouse_event(const SDL_Event& e)
{
    m_mouse_event = true;
    if ((e.type == SDL_MOUSEBUTTONDOWN) ||
        (e.type == SDL_MOUSEBUTTONUP)
    )
    {
        if (m_terrain_palette.visible && m_terrain_palette.hit(m_renderer.mouse_position()))
        {
            m_terrain_palette.mouse_event(e);
        }
        else
        {
            bool pressed = (e.type == SDL_MOUSEBUTTONDOWN);
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                m_left_mouse = pressed;
            }
            else if (e.button.button == SDL_BUTTON_RIGHT)
            {
                m_right_mouse = pressed;
            }
        }
    }
}

void Game::main_loop()
{
    m_running = true;
    m_left_mouse = false;
    m_right_mouse = false;
    m_renderer.set_window_event_handler(this);
    m_renderer.set_key_event_handler(this);
    m_renderer.set_mouse_event_handler(this);
    while (m_running)
    {
        m_mouse_event = false;

        m_renderer.handle_events();

        if (m_mouse_event)
        {
            if (m_map_view.hit(m_renderer.mouse_position()))
            {
                m_map_editor.mouse(m_left_mouse, m_right_mouse, m_renderer.mouse_position());
            }
        }

        draw();

    }
    m_renderer.set_window_event_handler(nullptr);
    m_renderer.set_key_event_handler(nullptr);
    m_renderer.set_mouse_event_handler(nullptr);
}

std::vector<Coordinate> const& Game::get_terrain_shapes() const
{
    return m_terrain_shapes;
}

uint16_t Game::get_base_terrain(uint16_t terrain) const
{
    if (terrain < m_multigroup_terrain_offset)
    {
        return terrain;
    }
    uint16_t group = (terrain - m_multigroup_terrain_offset) / (group_width * group_height);
    Expects(group < group_count);
    auto &multishape = gsl::at(multishapes, group);
    Expects(m_terrain_types[multishape.base_terrain_type].group == group);
    return multishape.base_terrain_type;
}

terrain_t Game::get_base_terrain(Coordinate tile_coordinate) const
{
    terrain_t terrain = m_map->get_terrain(tile_coordinate);
    return get_base_terrain(terrain);
}

terrain_t Game::get_multishape_terrain(uint16_t group, uint16_t neighbor_mask)
{
    return m_multigroup_terrain_offset + group * group_width * group_height + neighbor_mask;
}
