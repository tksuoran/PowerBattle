#include "area.hpp"

bool Area::hit(Coordinate position) const
{
    return (position.x >= offset.x) &&
           (position.y >= offset.y) &&
           (position.x < offset.x + extent.x) &&
           (position.y < offset.y + extent.y);
}

void Area::center(Coordinate window_size)
{
    this->offset = Coordinate((window_size.x - extent.x) / 2,
                              (window_size.y - extent.y) / 2);
}

void Area::move(Coordinate offset)
{
    this->offset = offset;
}

void Area::resize(Coordinate extent)
{
    this->extent = extent;
}

void Area::show()
{
    visible = true;
}

void Area::hide()
{
    visible = false;
}

SDL_Rect Area::get_sdl_rect() const
{
    SDL_Rect res;
    res.x = offset.x;
    res.y = offset.y;
    res.w = extent.x;
    res.h = extent.y;
    return res;
}
