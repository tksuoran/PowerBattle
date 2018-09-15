#ifndef TERRAIN_PALETTE_HPP
#define TERRAIN_PALETTE_HPP

#include "area.hpp"

class Game;

class TerrainPalette : public Area
{
public:
    Game *m_game;

    void mouse_event(const SDL_Event& event);

    void draw() const;
};

#endif // TERRAIN_PALETTE_HPP
