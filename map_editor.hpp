#ifndef MAPEDITOR_HPP
#define MAPEDITOR_HPP

#include <cstdint>
#include <SDL2/SDL.h>
#include "coordinate.hpp"
#include "terrain_type.hpp"

class Game;

class MapEditor
{
public:
    void mouse(const SDL_Event &event, bool left_mouse, bool right_mouse, Coordinate position);

    Game* m_game;
    uint16_t left_brush  = TerrainPlain;
    uint16_t right_brush = TerrainWaterLow;
    uint16_t last_brush  = TerrainPlain;
    uint16_t brush_size  = 1U;
    int offset = 0;
};

#endif // MAPEDITOR_HPP
