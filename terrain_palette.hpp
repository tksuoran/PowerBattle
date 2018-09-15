#ifndef TERRAIN_PALETTE_HPP
#define TERRAIN_PALETTE_HPP

#include "area.hpp"

class Game;

class TerrainPalette : public Area
{
public:
    Game *m_game;

    void mouse(const SDL_Event &event, bool left_mouse, bool right_mouse, Coordinate position);

    void draw() const;
};

#endif // TERRAIN_PALETTE_HPP
