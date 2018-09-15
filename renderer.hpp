#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "color.hpp"
#include "coordinate.hpp"

class Renderer
{
public:
    Renderer();

    ~Renderer();

    void init();

    void deinit();

    void mouse(int x, int y);

    Coordinate mouse_position();

    void resize(int width, int height);

    void set_texture_alpha_mode(SDL_Texture *texture, uint8_t alpha);

    SDL_Texture *load_texture(const char *const filename);

    void clear(const Color &color);

    void set_blend_mode(SDL_BlendMode blend_mode);

    void draw_rect(const Color &color, SDL_Rect rect);

    void draw_rect(const Color &outline_color, const Color &fill_color, SDL_Rect rect);

    void blit(SDL_Texture *texture, SDL_Rect src_rect, SDL_Rect dst_rect) const;

    void present();

    void zoom(int zoom_mod);

    int get_width() const;

    int get_height() const;

    Coordinate      m_mouse_position;
    int             m_window_width = 1024;
    int             m_window_height = 768;
    SDL_Window      *m_window = nullptr;
    SDL_Renderer    *m_renderer = nullptr;
    int             m_zoom = 1;
};

#endif // RENDERER_HPP
