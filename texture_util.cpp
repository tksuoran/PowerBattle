#include "texture_util.hpp"
#include "globals.hpp"

void mask_tile(const int tile_x, const int tile_y, SDL_Surface *const mask, SDL_Surface *const surface)
{
    unsigned char *mask_pixels = (unsigned char*)(mask->pixels);
    unsigned char *pixels = (unsigned char*)(surface->pixels);
    for (int y = 0; y < tile_height; ++y)
    {
        for (int x = 0; x < tile_full_width; ++x)
        {
            unsigned char r = mask_pixels[y * mask->pitch + x * 4 + 3];
            if (r == 0U)
            {
                int px = (tile_x * tile_full_width) + x;
                int py = (tile_y * tile_height) + y;
                pixels[py * surface->pitch + px * 4 + 0] = 0U;
            }
        }
    }
}

void color_mask_tile(unsigned char r0, unsigned char g0, unsigned char b0,
                     int tile_x, int tile_y,
                     SDL_Surface *surface)
{
    unsigned char *pixels = (unsigned char*)(surface->pixels);
    for (int y = 0; y < tile_height; ++y)
    {
        int py = (tile_y * tile_height) + y;
        for (int x = 0; x < tile_full_width; ++x)
        {
            int px = (tile_x * tile_full_width) + x;
            unsigned char b = pixels[py * surface->pitch + px * 4 + 1];
            unsigned char g = pixels[py * surface->pitch + px * 4 + 2];
            unsigned char r = pixels[py * surface->pitch + px * 4 + 3];
            if ((r == r0) && (g == g0) && (b == b0))
            {
                pixels[py * surface->pitch + px * 4 + 0] = 0U;
            }
        }
    }
}

void apply_mask()
{
    SDL_Surface *mask_in = IMG_Load("data/graphics/hexmask.png");
    if (mask_in == nullptr)
    {
        const char *error_message = SDL_GetError();
        fprintf(stderr, "IMG_Load() failed: %s\n", error_message ? error_message : "");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }
    SDL_Surface *surface_in = IMG_Load("data/graphics/hex.png");
    if (surface_in == nullptr)
    {
        const char *error_message = SDL_GetError();
        fprintf(stderr, "IMG_Load() failed: %s\n", error_message ? error_message : "");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    SDL_Surface *mask = SDL_ConvertSurfaceFormat(mask_in, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_Surface *surface_out = SDL_ConvertSurfaceFormat(surface_in, SDL_PIXELFORMAT_RGBA8888, 0);
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
    // 1 / 2 extra row above - using color mask
    // One extra row below - using color mask
    for (int tx = 0; tx < basetiles_width; ++tx)
    {
        color_mask_tile(64U, 64U, 80U, tx, group_count * group_height, surface_out);
        color_mask_tile(64U, 64U, 80U, tx, group_count * group_height + basetiles_height + 1, surface_out);
    }

    SDL_UnlockSurface(mask);
    SDL_UnlockSurface(surface_out);

    if (IMG_SavePNG(surface_out, "data/graphics/hex_.png") < 0)
    {
        const char *error_message = SDL_GetError();
        fprintf(stderr, "IMG_SavePNG() failed: %s\n", error_message ? error_message : "");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }
    SDL_FreeSurface(mask_in);
    SDL_FreeSurface(mask);
    SDL_FreeSurface(surface_in);
    SDL_FreeSurface(surface_out);

    SDL_Surface *unit_in = IMG_Load("data/graphics/unit.png");
    if (mask_in == nullptr)
    {
        const char *error_message = SDL_GetError();
        fprintf(stderr, "IMG_Load() failed: %s\n", error_message ? error_message : "");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    SDL_Surface *unit = SDL_ConvertSurfaceFormat(unit_in, SDL_PIXELFORMAT_RGBA8888, 0);
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
        fprintf(stderr, "IMG_SavePNG() failed: %s\n", error_message ? error_message : "");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    SDL_UnlockSurface(unit);
    SDL_FreeSurface(unit_in);
    SDL_FreeSurface(unit);
}
