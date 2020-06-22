#include <fmt/format.h>

#include "globals.hpp"
#include "texture_util.hpp"

void mask_tile(const int tile_x, const int tile_y, SDL_Surface *const mask, SDL_Surface *const surface)
{
    auto mask_pixels = reinterpret_cast<uint8_t*>(mask->pixels);
    auto pixels = reinterpret_cast<uint8_t*>(surface->pixels);
    for (int y = 0; y < tile_height; ++y)
    {
        for (int x = 0; x < tile_full_width; ++x)
        {
            auto r = mask_pixels[y * mask->pitch + x * 4 + 3];
            if (r == 0U)
            {
                int px = (tile_x * tile_full_width) + x;
                int py = (tile_y * tile_height) + y;
                pixels[py * surface->pitch + px * 4 + 0] = 0U;
            }
        }
    }
}

void color_mask_tile(uint8_t r0, uint8_t g0, uint8_t b0,
                     int tile_x, int tile_y,
                     SDL_Surface *surface)
{
    auto pixels = reinterpret_cast<uint8_t*>(surface->pixels);
    for (int y = 0; y < tile_height; ++y)
    {
        int py = (tile_y * tile_height) + y;
        for (int x = 0; x < tile_full_width; ++x)
        {
            int px = (tile_x * tile_full_width) + x;
            uint8_t b = pixels[py * surface->pitch + px * 4 + 1];
            uint8_t g = pixels[py * surface->pitch + px * 4 + 2];
            uint8_t r = pixels[py * surface->pitch + px * 4 + 3];
            if ((r == r0) && (g == g0) && (b == b0))
            {
                pixels[py * surface->pitch + px * 4 + 0] = 0U;
            }
        }
    }
}

void apply_mask()
{
    SDL_Surface *const mask_in = IMG_Load("data/graphics/hexmask.png");
    if (mask_in == nullptr)
    {
        const char *const error_message = SDL_GetError();
        fmt::print(stderr, "IMG_Load() failed: {}\n", (error_message != nullptr) ? error_message : "");
        std::fflush(stderr);
        std::exit(EXIT_FAILURE);
    }
    SDL_Surface *const surface_in = IMG_Load("data/graphics/hex.png");
    if (surface_in == nullptr)
    {
        const char *const error_message = SDL_GetError();
        fmt::print(stderr, "IMG_Load() failed: {}\n", (error_message != nullptr) ? error_message : "");
        std::fflush(stderr);
        std::exit(EXIT_FAILURE);
    }

    SDL_Surface *const mask = SDL_ConvertSurfaceFormat(mask_in, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_Surface *const surface_out = SDL_ConvertSurfaceFormat(surface_in, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_LockSurface(mask);
    SDL_LockSurface(surface_out);

    for (int ty = 0; ty < group_count * group_height; ++ty)
    {
        for (int tx = 0; tx < group_width; ++tx)
        {
            mask_tile(tx, ty, mask, surface_out);
        }
    }

    for (int ty = 0; ty < basetiles_height + 1; ++ty)
    {
        for (int tx = 0; tx < basetiles_width; ++tx)
        {
            mask_tile(tx, 1 + ty + group_count * group_height, mask, surface_out);
        }
    }
    //mask_tile(0, 1 + basetiles_height + 1 + group_count * group_height, mask, surface_out);
    // 1 / 2 extra row above - using color mask
    for (int tx = 0; tx < 6; ++tx)
    {
        color_mask_tile(64U, 64U, 80U, tx, group_count * group_height, surface_out);
    }
    // One extra row below - using color mask
    for (int tx = 0; tx < basetiles_width - 1; ++tx)
    {
        color_mask_tile(64U, 64U, 80U, tx, group_count * group_height + basetiles_height + 2, surface_out);
    }
    mask_tile(basetiles_width - 1, group_count * group_height + basetiles_height + 2, mask, surface_out);

    SDL_UnlockSurface(mask);
    SDL_UnlockSurface(surface_out);

    if (IMG_SavePNG(surface_out, "data/graphics/hex_.png") < 0)
    {
        const char *const error_message = SDL_GetError();
        fmt::print(stderr, "IMG_SavePNG() failed: {}\n", (error_message != nullptr) ? error_message : "");
        std::fflush(stderr);
        std::exit(EXIT_FAILURE);
    }
    SDL_FreeSurface(mask_in);
    SDL_FreeSurface(mask);
    SDL_FreeSurface(surface_in);
    SDL_FreeSurface(surface_out);

    SDL_Surface *const unit_in = IMG_Load("data/graphics/unit.png");
    if (mask_in == nullptr)
    {
        const char *const error_message = SDL_GetError();
        fmt::print(stderr, "IMG_Load() failed: {}\n", (error_message != nullptr) ? error_message : "");
        std::fflush(stderr);
        std::exit(EXIT_FAILURE);
    }

    SDL_Surface *const unit = SDL_ConvertSurfaceFormat(unit_in, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_LockSurface(unit);

    for (int ty = 0; ty < unit_group_height; ++ty)
    {
        for (int tx = 0; tx < unit_group_width; ++tx)
        {
            color_mask_tile(0x30U, 0x30U, 0x60U, tx, ty, unit);
        }
    }
    if (IMG_SavePNG(unit, "data/graphics/unit_.png") < 0)
    {
        const char *error_message = SDL_GetError();
        fmt::print(stderr, "IMG_SavePNG() failed: {}\n", (error_message != nullptr) ? error_message : "");
        std::fflush(stderr);
        std::exit(EXIT_FAILURE);
    }

    SDL_UnlockSurface(unit);
    SDL_FreeSurface(unit_in);
    SDL_FreeSurface(unit);
}

tile_mask_t get_mask_bits()
{
    SDL_Surface *const mask_in = IMG_Load("data/graphics/hexmask.png");
    if (mask_in == nullptr)
    {
        const char *const error_message = SDL_GetError();
        fmt::print(stderr, "IMG_Load() failed: {}\n", (error_message != nullptr) ? error_message : "");
        std::fflush(stderr);
        std::exit(EXIT_FAILURE);
    }
    SDL_Surface *const mask = SDL_ConvertSurfaceFormat(mask_in, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_FreeSurface(mask_in);
    SDL_LockSurface(mask);

    tile_mask_t res;
    unsigned char *const mask_pixels = (unsigned char*)(mask->pixels);
    for (int y = 0; y < tile_height; ++y)
    {
        for (int x = 0; x < tile_full_width; ++x)
        {
            unsigned char r = mask_pixels[y * mask->pitch + x * 4 + 3];
            size_t bit_index = x + y * tile_full_width;
            res.set(bit_index, r != 0U);
        }
    }

    SDL_UnlockSurface(mask);
    SDL_FreeSurface(mask);

    return res;
}
