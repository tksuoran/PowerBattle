#ifndef HOTBAR_HPP
#define HOTBAR_HPP

#include "area.hpp"

class Game;

class Hotbar : public Area
{
public:
    void draw() const;

    Game *m_game;
};

#endif // HOTBAR_HPP
