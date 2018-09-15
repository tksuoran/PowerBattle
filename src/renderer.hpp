#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "SDL2/SDL_ttf.h"
#include "color.hpp"
#include "coordinate.hpp"

class WindowEventHandler
{
public:
    virtual void resize(int width, int height) = 0;
    virtual void quit() = 0;
};

class KeyEventHandler
{
public:
    virtual void key_event(const SDL_Event& e) = 0;
};

class MouseEventHandler
{
public:
    virtual void mouse_event(const SDL_Event& e) = 0;
};

class Renderer
{
public:
    Renderer() = default;

    ~Renderer();

    void window_event(const SDL_Event& e);

    void key_event(const SDL_Event& e);

    void mouse_event(const SDL_Event& e);

    void handle_events();

    void init();

    void deinit();

    Coordinate mouse_position();

    void resize(int width, int height);

    void set_texture_alpha_mode(SDL_Texture *texture, float alpha);

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

    void set_window_event_handler(WindowEventHandler* handler);
    void set_key_event_handler(KeyEventHandler* handler);
    void set_mouse_event_handler(MouseEventHandler* handler);

    void draw_text(int x, int y, const char *const text) const;

private:
    void load_font();

    struct GlyphMetrics
    {
        int min_x;
        int max_x;
        int min_y;
        int max_y;
        int advance;
        SDL_Rect rect;
    };

    WindowEventHandler* m_window_event_handler{nullptr};
    KeyEventHandler*    m_key_event_handler   {nullptr};
    MouseEventHandler*  m_mouse_event_handler {nullptr};
    Coordinate          m_mouse_position;
    int                 m_window_width {1024};
    int                 m_window_height{768};
    SDL_Window*         m_window       {nullptr};
    SDL_Renderer*       m_renderer     {nullptr};
    TTF_Font*           m_font         {nullptr};
    std::array<GlyphMetrics, 128> m_glyph_metrics;
    SDL_Texture*        m_font_texture {nullptr};
    int                 m_zoom         {1};
};

#endif // RENDERER_HPP
