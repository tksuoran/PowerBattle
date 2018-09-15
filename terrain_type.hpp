#ifndef TERRAIN_TYPE_HPP
#define TERRAIN_TYPE_HPP

#include <cstdint>

struct OriginalTerrainType
{
    uint8_t Color;
    uint8_t MAllow;
    uint8_t MCost;
    uint8_t Threat;
    uint8_t ADamage;
    uint8_t DBonus;
    uint8_t CSize;
    uint8_t Strength;
    uint8_t Mult;
    uint8_t VChg;
    uint8_t HNum;
};

struct TerrainType
{
    //uint16_t color;
    //uint16_t move_type_allow_mask;
    //uint16_t move_cost;
    //uint16_t threat;
    //uint16_t damaged_version;
    //uint16_t defence_bonus;
    //uint16_t city_size;
    //uint16_t strength;
    int16_t group = -1;
};

constexpr uint16_t TerrainUnknown        = 0U;
constexpr uint16_t TerrainWaterLow       = 5U;
constexpr uint16_t TerrainFortress       = 19U;
constexpr uint16_t TerrainRoad           = 20U;
constexpr uint16_t TerrainBridge         = 12U;
constexpr uint16_t TerrainPlain          = 31U;
constexpr uint16_t TerrainField          = 39U;
constexpr uint16_t TerrainMediumWater    = 3U;
constexpr uint16_t TerrainDefault        = TerrainPlain;

// Bridges
constexpr int TerrainBridges[] = { 11, 12, 13, 11, 12, 13 };

struct MultiShape
{
    int  base_terrain_type; // a
    int  link_first;        // b
    int  link_last;         // c
    int  link_group;        // d
    int  link_group2;       // e
    int  link2_first;       // f
    int  link2_last;        // g
    int  promoted;
    int  demoted;
};

constexpr MultiShape multishapes[] = {
    { 20, 11, 22, -1, -1, -1, -1, -1, -1 }, // 0 Road
    { 28, 28, 29, -1, -1, -1, -1, -1,  3 }, // 1 Peak
    { 41, 41, 44, -1, -1, -1, -1, -1, -1 }, // 2 Forest
    { 45, 45, 49,  1, -1, 28, 29,  1, -1 }, // 3 Mountain
    {  5,  5, 18,  5,  6, -1, -1, -1, -1 }, // 4 Water Low
    {  3,  3,  4,  6, -1, -1, -1, -1, -1 }, // 5 Water Medium
    {  1,  1,  2, -1, -1, -1, -1, -1, -1 }, // 6 Water Deep
};

#endif // TERRAIN_TYPE_HPP
