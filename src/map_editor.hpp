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
    void mouse(bool left_mouse, bool right_mouse, Coordinate position);

    Game* m_game;
    terrain_t left_brush  = TerrainPlain;
    terrain_t right_brush = TerrainWaterLow;
    terrain_t last_brush  = TerrainPlain;
    terrain_t brush_size  = 1U;
    int offset = 0;
};

#endif // MAPEDITOR_HPP
