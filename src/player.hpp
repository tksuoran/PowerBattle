#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include "unit.hpp"

struct Player
{
    std::vector<Unit> units;
};

#endif // PLAYER_HPP