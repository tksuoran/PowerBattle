#ifndef UNIT_TYPE_HPP
#define UNIT_TYPE_HPP

#include <cstdint>
#include <string>

const int CategoryAir           = 1;
const int CategoryGround        = 2;
const int CategorySea           = 3;
const int CategorySub           = 4;
const int CategoryCity          = 5;

struct UnitType
{
    std::string name;
    uint16_t move_type_bits;
    uint16_t moves[2];
    // uint16_t threat_defence;
    uint16_t fuel;
    uint16_t refuel_per_turn;
    uint16_t cargo_type;
    uint16_t load_count_per_turn;
    // uint16_t tech_level;
    uint16_t production_time;
    // uint16_t cost; // reserved
    uint16_t hit_points;
    uint16_t repair_per_turn;
    uint16_t levels; // reserved

    // bomber    = 1
    // settler   = 2
    // destroyer = 3
    // sub       = 4
    // city      = 5
    uint16_t battle_type;

    uint16_t attack[4];
    uint16_t defence[4];
    // uint16_t light_ground_modifier;
    uint16_t range[2];
    uint16_t ranged_attack_modifier;
    uint16_t ranged_attack_ammo;
    uint16_t ranged_attack_count_per_turn;
    uint16_t vision[2];
    uint16_t visible_from[2];

};

#endif // UNIT_TYPE_HPP
