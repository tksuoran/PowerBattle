#ifndef MAPVIEW_HPP
#define MAPVIEW_HPP

#include "area.hpp"
#include "coordinate.hpp"

class Game;

class MapView : public Area
{
public:
    MapView(Game *game = nullptr);

    Game *m_game;

    int        m_grid = 0;
    Coordinate m_view_offset; // pixels
    Coordinate m_tile_offset; // LX, LY
    //Coordinate m_pixel_offset; // SXA, SYA

    void translate(Coordinate offset);

    Coordinate wrap(Coordinate in) const;

    bool hit_tile(Coordinate tile_coordinate) const;

    Coordinate pixel_to_tile(Coordinate pixel_coordinate) const;

    // Convert absolute map coordinate to pixel coordinate
    Coordinate tile_to_pixel(Coordinate tile_coordinate) const;

    void zoom(int value);
  
    void draw() const;
};

#endif // MAPVIEW_HPP
