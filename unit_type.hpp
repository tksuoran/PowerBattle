#ifndef UNIT_TYPE_HPP
#define UNIT_TYPE_HPP

#include <cstdint>

const int CategoryAir           = 1;
const int CategoryGround        = 2;
const int CategorySea           = 3;
const int CategorySub           = 4;
const int CategoryCity          = 5;

struct OriginalUnitType
{
    uint8_t MType;      // Movement Type Bits
    uint8_t Moves[2];   // Movement Points per Turn
    uint8_t TDef;       // Threat Defence
    uint8_t Fuel;       // Fuel Tank Size
    uint8_t Refuel;     // Refuel Points per Turn
    uint8_t CType;      // Cargo Type Bits
    uint8_t Cargo;      // Cargo Space
    uint8_t Loading;    // Loadings/Unloadings per Turn
    uint8_t Tech;       // Required Techlevel for Production
    uint8_t PTime;      // Production Time
    uint8_t Cost;       // Reserved
    uint8_t Hits;       // Maximum HitPoints
    uint8_t Repair;     // Repair Points per Turn
    uint8_t Levels;     // Reserved
    uint8_t BType;      // Battle Type #
    uint8_t Attack[4];  // Attack Points against BattleTypes
    uint8_t Defence[4]; // Defence  -  "  -
    uint8_t LgtGndMod;  // Light Ground Modifier
    uint8_t Range[2];   // Artillery Attack Range
    uint8_t RAttack;    // Ranged Attack Modifier
    uint8_t Ammo;       // Ammunitation Storage
    uint8_t Shots;      // Artillery Attacks per Turn
    uint8_t LAtt;       // Attack LevelModifier
    uint8_t LDef;       // Defence LevelModifier
    uint8_t CAttack;    // City Attack Modifier
    uint8_t Vision[2];  // Vision Range
    uint8_t VFrom[2];   // Visible From -Range
    uint8_t LUI;        // Leveled Unit Icon #
    uint8_t fff;        // Reserved
    uint8_t ggg;        // Reserved
    uint8_t hhh;        // Reserved
    uint8_t Sfx;        // Sound Effect #
    uint8_t Frequ;      // Sound Effect Frequency
    uint8_t Specials;   // Special Abilities
    uint8_t Undefd;     // Reserved
    uint8_t L;          // Reserved
};

struct UnitType
{
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
