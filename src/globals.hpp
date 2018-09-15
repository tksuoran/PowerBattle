#ifndef GLOBALS_HPP
#define GLOBALS_HPP

constexpr int tile_full_width       = 14;
constexpr int tile_height           = 12;
constexpr int tile_interleave_width = 11;
constexpr int tile_odd_y_offset     =  6; // tile_height / 2
constexpr int tile_full_size        = tile_full_width * tile_height;

constexpr int group_count           =  7;
constexpr int group_width           =  8;
constexpr int group_height          =  8;
constexpr int basetiles_width       =  5;
constexpr int basetiles_height      = 11;

constexpr int unit_group_width      =  7;
constexpr int unit_group_height     = 10;

using terrain_t = uint16_t;

#endif // GLOBALS_HPP
