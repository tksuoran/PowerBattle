#ifndef MAPVIEW_HPP
#define MAPVIEW_HPP

#include "area.hpp"
#include "coordinate.hpp"
#include <string>
#include <vector>

class Game;

class MapView : public Area
{
public:
    MapView(Game *game = nullptr);

    void translate(Coordinate offset);

    Coordinate wrap(Coordinate in) const;

    bool hit_tile(Coordinate tile_coordinate) const;

    Coordinate pixel_to_tile(Coordinate pixel_coordinate) const;

    // Convert absolute map coordinate to pixel coordinate
    Coordinate tile_to_pixel_top_left(Coordinate tile_coordinate) const;

    Coordinate tile_to_pixel_center(Coordinate tile_coordinate) const;

    void zoom(int value);

    struct TextEntry
    {
        TextEntry(int x, int y, const std::string& text) :
            x(x),
            y(y),
            text(text)
        {
        }

        int x;
        int y;
        std::string text;
    };

    void draw_text(int x, int y, const std::string& text)
    {
        m_text_entries.push_back(TextEntry(x, y, text));
    }
  
    void draw();

    std::vector<TextEntry> m_text_entries;

    Game        *m_game;
    int         m_grid = 0;
    Coordinate  m_view_offset; // pixels
    Coordinate  m_tile_offset; // LX, LY
};

#endif // MAPVIEW_HPP
