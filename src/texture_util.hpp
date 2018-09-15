#ifndef TEXTURE_UTIL_HPP
#define TEXTURE_UTIL_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <bitset>
#include "globals.hpp"

void mask_tile(const int tile_x, const int tile_y, SDL_Surface *const mask, SDL_Surface *const surface);

void color_mask_tile(unsigned char r0, unsigned char g0, unsigned char b0,
                     int tile_x, int tile_y,
                     SDL_Surface *surface);

void apply_mask();

using tile_mask_t = std::bitset<tile_full_size>;

tile_mask_t get_mask_bits();

#endif // TEXTURE_UTIL_HPP
