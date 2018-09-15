#ifndef AREA_HPP
#define AREA_HPP

#include "coordinate.hpp"
#include <SDL2/SDL.h>

class Area
{
public:
    bool hit(Coordinate position) const;

    void center(Coordinate window_size);

    void move(Coordinate offset);

    void resize(Coordinate extent);

    void show();

    void hide();

    SDL_Rect get_sdl_rect() const;

    bool visible = true;
    Coordinate offset;
    Coordinate extent;
};

#endif // AREA_HPP

