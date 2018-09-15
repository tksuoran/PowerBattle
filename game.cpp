#include <cassert>

#include "game.hpp"
#include "map.hpp"
#include "globals.hpp"
#include "file_util.hpp"

Game::Game()
{
}

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

Coordinate Game::get_terrain_shape(uint16_t terrain)
{
    assert(terrain < m_terrain_shapes.size());
    return m_terrain_shapes[terrain];
}

void Game::init_terrain_shapes()
{
    // First row below multihexes has 
    int ty_offset = group_count * group_height;
    for (int tx = 0; tx < group_width; ++tx)
    {
        m_extra_shapes.push_back(Coordinate(tx * tile_full_width,
                                            ty_offset * tile_height));
    }
    ty_offset += 1;

    // Edge shapes
    for (int tx = 0; tx < group_width; ++tx)
    {
        m_edge_shapes.push_back(Coordinate(tx * tile_full_width,
                                           ty_offset * tile_height));
    }
    ty_offset += 1;

    // Basetiles
    for (int ty = 0; ty < basetiles_height; ++ty)
    {
        for (int tx = 0; tx < basetiles_width; ++tx)
        {
            m_terrain_shapes.push_back(Coordinate(tx * tile_full_width,
                                                  (ty + ty_offset) * tile_height));
        }
    }
    ty_offset += basetiles_height;

    // Grid shapes
    for (int tx = 0; tx < group_width; ++tx)
    {
        m_grid_shapes.push_back(Coordinate(tx * tile_full_width,
                                           ty_offset * tile_height));
    }

    m_multigroup_terrain_offset = m_terrain_shapes.size();
    for (int ty = 0; ty < group_count * group_height; ++ty)
    {
        for (int tx = 0; tx < group_width; ++tx)
        {
            m_terrain_shapes.push_back(Coordinate(tx * tile_full_width,
                                                  ty * tile_height));
        }
    }
}

void Game::init_unit_shapes()
{
    for (int ty = 0; ty < unit_group_height; ++ty)
    {
        for (int tx = 0; tx < unit_group_width; ++tx)        
        {
            m_unit_shapes.push_back(Coordinate(tx * tile_full_width,
                                               ty * tile_height));
        }
    }
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
    m_terrain_types.resize(basetiles_width * basetiles_height);
    m_terrain_types[20].group = 0;
    m_terrain_types[28].group = 1;
    m_terrain_types[41].group = 2;
    m_terrain_types[45].group = 3;
    m_terrain_types[ 5].group = 4;
    m_terrain_types[ 3].group = 5;
    m_terrain_types[ 1].group = 6;
}

void Game::load_terrain_defs()
{
    std::vector<unsigned char> def_data = read_file("data/definitions/terrain_types.def");
    const int hex_types = 55;
    const int load_string_length = 12;
    size_t pos = 0U;
    for (int i = 0; i < hex_types + 1; ++i)
    {
        OriginalTerrainType type;
        type.Color    = def_data[pos++];
        type.MAllow   = def_data[pos++];
        type.MCost    = def_data[pos++];
        type.Threat   = def_data[pos++];
        type.ADamage  = def_data[pos++];
        type.DBonus   = def_data[pos++];
        type.CSize    = def_data[pos++];
        type.Strength = def_data[pos++];
        type.Mult     = def_data[pos++];
        type.VChg     = def_data[pos++];
        type.HNum     = def_data[pos++];
        // printf("\nTerrain %d:\n", i);
        // printf("Color    = %u\n", type.Color);
        // printf("MAllow   = %u\n", type.MAllow);
        // printf("MCost    = %u\n", type.MCost);
        // printf("Threat   = %u\n", type.Threat);
        // printf("ADamage  = %u\n", type.ADamage);
        // printf("DBonus   = %u\n", type.DBonus);
        // printf("CSize    = %u\n", type.CSize);
        // printf("Strength = %u\n", type.Strength);
        // printf("Mult     = %u\n", type.Mult);
        // printf("VChg     = %u\n", type.VChg);
        // printf("HNum     = %u\n", type.HNum);
    }
    for (int i = 0; i < hex_types + 1; ++i)
    {
        std::string name = get_string(def_data, pos,  load_string_length);
        pos += load_string_length;
        // printf("Terrain %d Name  = %s\n", i, name.c_str());
    }
    for (int i = 0; i < 11; ++i)
    {
        std::string name = get_string(def_data, pos,  load_string_length);
        pos += load_string_length;
        // printf("Field %d  Name  = %s\n", i, name.c_str());
    }
}

void Game::load_unit_defs()
{
    std::vector<unsigned char> def_data = read_file("data/definitions/unit_types.def");
    const int types = 42;
    const int load_string_length = 12;
    size_t pos = 0U;
    for (int i = 0; i < types + 1; ++i)
    {
        OriginalUnitType type;
        type.MType      = def_data[pos++];  // Movement Type Bits
        type.Moves[0]   = def_data[pos++];  // Movement Points per Turn
        type.Moves[1]   = def_data[pos++];  // Movement Points per Turn
        type.TDef       = def_data[pos++];  // Threat Defence
        type.Fuel       = def_data[pos++];  // Fuel Tank Size
        type.Refuel     = def_data[pos++];  // Refuel Points per Turn
        type.CType      = def_data[pos++];  // Cargo Type Bits
        type.Cargo      = def_data[pos++];  // Cargo Space
        type.Loading    = def_data[pos++];  // Loadings/Unloadings per Turn
        type.Tech       = def_data[pos++];  // Required Techlevel for Production
        type.PTime      = def_data[pos++];  // Production Time
        type.Cost       = def_data[pos++];  // Reserved
        type.Hits       = def_data[pos++];  // Maximum HitPoints
        type.Repair     = def_data[pos++];  // Repair Points per Turn
        type.Levels     = def_data[pos++];  // Reserved
        type.BType      = def_data[pos++];  // Battle Type #
        type.Attack[0]  = def_data[pos++];  // Attack Points against BattleTypes
        type.Attack[1]  = def_data[pos++];  // Attack Points against BattleTypes
        type.Attack[2]  = def_data[pos++];  // Attack Points against BattleTypes
        type.Attack[3]  = def_data[pos++];  // Attack Points against BattleTypes
        type.Defence[0] = def_data[pos++];  // Defence  -  "  -
        type.Defence[1] = def_data[pos++];  // Defence  -  "  -
        type.Defence[2] = def_data[pos++];  // Defence  -  "  -
        type.Defence[3] = def_data[pos++];  // Defence  -  "  -
        type.LgtGndMod  = def_data[pos++];  // Light Ground Modifier
        type.Range[0]   = def_data[pos++];  // Artillery Attack Range
        type.Range[1]   = def_data[pos++];  // Artillery Attack Range
        type.RAttack    = def_data[pos++];  // Ranged Attack Modifier
        type.Ammo       = def_data[pos++];  // Ammunitation Storage
        type.Shots      = def_data[pos++];  // Artillery Attacks per Turn
        type.LAtt       = def_data[pos++];  // Attack LevelModifier
        type.LDef       = def_data[pos++];  // Defence LevelModifier
        type.CAttack    = def_data[pos++];  // City Attack Modifier
        type.Vision[0]  = def_data[pos++];  // Vision Range
        type.Vision[1]  = def_data[pos++];  // Vision Range
        type.VFrom[0]   = def_data[pos++];  // Visible From -Range
        type.VFrom[1]   = def_data[pos++];  // Visible From -Range
        type.LUI        = def_data[pos++];  // Leveled Unit Icon #
        type.fff        = def_data[pos++];  // Reserved
        type.ggg        = def_data[pos++];  // Reserved
        type.hhh        = def_data[pos++];  // Reserved
        type.Sfx        = def_data[pos++];  // Sound Effect #
        type.Frequ      = def_data[pos++];  // Sound Effect Frequency
        type.Specials   = def_data[pos++];  // Special Abilities
        type.Undefd     = def_data[pos++];  // Reserved
        type.L          = def_data[pos++];  // Reserved
        // printf("\nUnit type %d:\n", i);
        // printf("MType      = %u\n", type.MType);
        // printf("Moves[0]   = %u\n", type.Moves[0]);
        // printf("Moves[1]   = %u\n", type.Moves[1]);
        // printf("TDef       = %u\n", type.TDef);
        // printf("Fuel       = %u\n", type.Fuel);
        // printf("Refuel     = %u\n", type.Refuel);
        // printf("CType      = %u\n", type.CType);
        // printf("Cargo      = %u\n", type.Cargo);
        // printf("Loading    = %u\n", type.Loading);
        // printf("Tech       = %u\n", type.Tech);
        // printf("PTime      = %u\n", type.PTime);
        // printf("Cost       = %u\n", type.Cost);
        // printf("Hits       = %u\n", type.Hits);
        // printf("Repair     = %u\n", type.Repair);
        // printf("Levels     = %u\n", type.Levels);
        // printf("BType      = %u\n", type.BType);
        // printf("Attack[0]  = %u\n", type.Attack[0]);
        // printf("Attack[1]  = %u\n", type.Attack[1]);
        // printf("Attack[2]  = %u\n", type.Attack[2]);
        // printf("Attack[3]  = %u\n", type.Attack[3]);
        // printf("Defence[0] = %u\n", type.Defence[0]);
        // printf("Defence[1] = %u\n", type.Defence[1]);
        // printf("Defence[2] = %u\n", type.Defence[2]);
        // printf("Defence[3] = %u\n", type.Defence[3]);
        // printf("LgtGndMod  = %u\n", type.LgtGndMod);
        // printf("Range[0]   = %u\n", type.Range[0]);
        // printf("Range[1]   = %u\n", type.Range[1]);
        // printf("RAttack    = %u\n", type.RAttack);
        // printf("Ammo       = %u\n", type.Ammo);
        // printf("Shots      = %u\n", type.Shots);
        // printf("LAtt       = %u\n", type.LAtt);
        // printf("LDef       = %u\n", type.LDef);
        // printf("CAttack    = %u\n", type.CAttack);
        // printf("Vision[0]  = %u\n", type.Vision[0]);
        // printf("Vision[1]  = %u\n", type.Vision[1]);
        // printf("VFrom[0]   = %u\n", type.VFrom[0]);
        // printf("VFrom[1]   = %u\n", type.VFrom[1]);
        // printf("LUI        = %u\n", type.LUI);
        // printf("fff        = %u\n", type.fff);
        // printf("ggg        = %u\n", type.ggg);
        // printf("hhh        = %u\n", type.hhh);
        // printf("Sfx        = %u\n", type.Sfx);
        // printf("Frequ      = %u\n", type.Frequ);
        // printf("Specials   = %u\n", type.Specials);
        // printf("Undefd     = %u\n", type.Undefd);
        // printf("L          = %u\n", type.L);
    }
    for (int i = 0; i < types + 1; ++i)
    {
        std::string name = get_string(def_data, pos, load_string_length);
        pos += load_string_length;
        // printf("Unit %d Name = %s\n", i, name.c_str());
    }
    for (int i = 0; i < 36; ++i)
    {
        std::string name = get_string(def_data, pos, load_string_length);
        pos += load_string_length;
        // printf("Field %d Name  = %s\n", i, name.c_str());
    }
}

bool Game::init()
{
    m_renderer.init();

    m_hex_tiles = m_renderer.load_texture("data/graphics/hex_.png");
    m_unit_tiles = m_renderer.load_texture("data/graphics/unit_.png");

    load_terrain_defs();
    load_unit_defs();

    init_terrain_shapes();
    init_terrain_types();
    init_unit_shapes();

    std::vector<unsigned char> file = read_file("data/maps/default");
    size_t lo = 1;
    size_t hi = 0;
    size_t pos = 0;
    uint16_t width = (file[pos + lo]) | (file[pos + hi] << 8U); pos += 2;
    uint16_t height = (file[pos + lo]) | (file[pos + hi] << 8U); pos += 2;

    m_map = std::make_unique<Map>(width, height);
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            assert(pos < file.size());
            uint16_t terrain = (file[pos + lo]) | (file[pos + hi] << 8U); pos += 2;
            assert(pos < file.size());
            uint16_t unit_icon = (file[pos + lo]) | (file[pos + hi] << 8U); pos += 2;
            static_cast<void>(unit_icon);
            if (terrain < m_terrain_shapes.size())
            {
                m_map->set_terrain(Coordinate(x, y), terrain);
            }
            else
            {
                fprintf(stdout, "%d, %d: %u\n", x, y, terrain);
                fflush(stdout);
            }
        }
        pos += 3;
    }
    m_map_editor.m_game = this;
    m_map_view.m_game = this;
    m_terrain_palette.m_game = this;
    m_terrain_palette.hide();
    m_hotbar.m_game = this;
    layout();

    return true;
}

void Game::layout()
{
    int width = m_renderer.get_width();
    int height = m_renderer.get_height();
    m_map_view.resize(Coordinate(width, height));
    m_terrain_palette.resize(Coordinate(basetiles_width * tile_full_width,
                                        basetiles_height * tile_height));

    m_terrain_palette.center(Coordinate(width, height));
    m_hotbar.resize(Coordinate(10 * tile_full_width,
                               tile_height));
    m_hotbar.center(Coordinate(width, height / 4));
    m_hotbar.offset.y = height - tile_height * 1.25f;
}

void Game::deinit()
{
	SDL_DestroyTexture(m_hex_tiles);
    SDL_DestroyTexture(m_unit_tiles);
    m_hex_tiles = nullptr;
    m_unit_tiles = nullptr;
    m_renderer.deinit();
}

void Game::main_loop()
{
    bool running = true;
    bool left_mouse = false;
    bool right_mouse = false;
    while (running)
    {
        SDL_Event e;
        bool mouse = false;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
                case SDL_WINDOWEVENT:
                {
                    if (e.window.event == SDL_WINDOWEVENT_RESIZED)
                    {
                        m_renderer.resize(e.window.data1, e.window.data2);
                        layout();
                    }
                    break;
                }
                case SDL_QUIT:
                {
                    running = false;
                    break;
                }

                case SDL_KEYDOWN:
                {
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
                        default:
                            break;
                    }
                }
                case SDL_KEYUP:
                {
                    if (e.key.keysym.sym == SDLK_ESCAPE)
                    {
                        running = false;
                    }
                    break;
                }

                case SDL_MOUSEBUTTONUP:
                {
                    mouse = true;
                    m_renderer.mouse(e.button.x, e.button.y);
                    if (e.button.button == SDL_BUTTON_LEFT)
                    {
                        left_mouse = false;
                    }
                    else if (e.button.button == SDL_BUTTON_RIGHT)
                    {
                        right_mouse = false;
                    }
                    break;
                }

                case SDL_MOUSEBUTTONDOWN:
                {
                    mouse = true;
                    m_renderer.mouse(e.button.x, e.button.y);
                    if (e.button.button == SDL_BUTTON_LEFT)
                    {
                        left_mouse = true;
                    }
                    else if (e.button.button == SDL_BUTTON_RIGHT)
                    {
                        right_mouse = true;
                    }
                    break;
                }

                case SDL_MOUSEMOTION:
                {
                    mouse = true;
                    m_renderer.mouse(e.motion.x, e.motion.y);
                    break;
                }

                default:
                {
                    break;
                }
            }
        } 

        if (mouse)
        {
            if (m_map_view.hit(m_renderer.mouse_position()))
            {
                m_map_editor.mouse(e, left_mouse, right_mouse, m_renderer.mouse_position());
            }
            else if (m_terrain_palette.visible && m_terrain_palette.hit(m_renderer.mouse_position()))
            {
                m_terrain_palette.mouse(e, left_mouse, right_mouse, m_renderer.mouse_position());
            }
        }

        m_renderer.clear(Color(32U, 64U, 128U));
        m_map_view.draw();

        if (m_map_view.hit(m_renderer.mouse_position()))
        {
            Coordinate center_position = m_map->wrap(m_map_view.pixel_to_tile(m_renderer.mouse_position()));
            Game *game = this;
            if (m_map_view.hit_tile(center_position))
            {
                Renderer *renderer = &m_renderer;
                SDL_Texture* texture = m_hex_tiles;
                m_renderer.set_texture_alpha_mode(texture, 127U);
                std::function<void(Coordinate)> brush_preview = [renderer, texture, game] (Coordinate position) -> void
                {
                    MapEditor &map_editor = game->m_map_editor;
                    MapView &map_view = game->m_map_view;
                    Map *map = game->get_map();
                    uint16_t terrain = map_editor.last_brush;
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
                    int y_offset = (view_relative_tile.x & 1) ? 0 : tile_odd_y_offset;
                    dst_rect.y = (view_relative_tile.y * tile_height + y_offset);
                    renderer->blit(texture, src_rect, dst_rect);
                };
                m_map->HexCircle(center_position, 0, m_map_editor.brush_size - 1, brush_preview);
                m_renderer.set_texture_alpha_mode(texture, 255U);
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
    assert(group < group_count);
    assert(m_terrain_types[multishapes[group].base_terrain_type].group == group);
    return multishapes[group].base_terrain_type;
}

uint16_t Game::get_base_terrain(Coordinate tile_coordinate) const
{
    uint16_t terrain = m_map->get_terrain(tile_coordinate);
    return get_base_terrain(terrain);
}

uint16_t Game::get_multishape_terrain(uint16_t group, uint16_t neighbor_mask)
{
    return m_multigroup_terrain_offset + group * group_width * group_height + neighbor_mask;
}
