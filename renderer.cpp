#include "renderer.hpp"

Renderer::Renderer()
{
}

void Renderer::init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) < 0)
    {
        fprintf(stderr, "SDL_Init() failed\n");
        fflush(stderr);
        abort();
    }

    if (IMG_Init(IMG_INIT_PNG) < 0)
    {
        fprintf(stderr, "IMG_Init() failed\n");
        fflush(stderr);
        abort();
    }

    m_window_width = 800;
    m_window_height = 600;

    m_window = SDL_CreateWindow("PB", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                m_window_width, m_window_height,
                                SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
    if (m_window == nullptr)
    {
        fprintf(stderr, "SDL_CreateWindow() failed\n");
        fflush(stderr);
        abort();
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);
    if (m_renderer == nullptr)
    {
        fprintf(stderr, "SDL_CreateRenderer() failed\n");
        fflush(stderr);
        abort();
    }
}

Renderer::~Renderer()
{
    deinit();
}

void Renderer::deinit()
{
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
    IMG_Quit();
    SDL_Quit();
    m_renderer = nullptr;
    m_window = nullptr;
}

void Renderer::mouse(int x, int y)
{
    m_mouse_position.x = x;
    m_mouse_position.y = y;
}

Coordinate Renderer::mouse_position()
{
    return Coordinate(m_mouse_position.x / m_zoom,
                      m_mouse_position.y / m_zoom);
}

void Renderer::resize(int width, int height)
{
    m_window_width = width;
    m_window_height = height;
}

void Renderer::set_texture_alpha_mode(SDL_Texture *texture, uint8_t alpha)
{
    SDL_SetTextureAlphaMod(texture, alpha);
}

SDL_Texture *Renderer::load_texture(const char *const filename)
{
    SDL_Texture *result = IMG_LoadTexture(m_renderer, filename);
    if (result == nullptr)
    {
        const char *error_message = SDL_GetError();
        fprintf(stderr,
                "IMG_LoadTexture(%s) failed: %s\n",
                filename,
                error_message ? error_message : "");
        fflush(stderr);
        abort();
    }

    return result;
}

void Renderer::clear(const Color &color)
{
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(m_renderer);
}

void Renderer::set_blend_mode(SDL_BlendMode blend_mode)
{
    SDL_SetRenderDrawBlendMode(m_renderer, blend_mode);
}

void Renderer::draw_rect(const Color &color, SDL_Rect rect)
{
    rect.x *= m_zoom;
    rect.y *= m_zoom;
    rect.w *= m_zoom;
    rect.h *= m_zoom;
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(m_renderer, &rect);
}

void Renderer::draw_rect(const Color &outline_color, const Color &fill_color, SDL_Rect rect)
{
    rect.x *= m_zoom;
    rect.y *= m_zoom;
    rect.w *= m_zoom;
    rect.h *= m_zoom;
    SDL_SetRenderDrawColor(m_renderer, fill_color.r, fill_color.g, fill_color.b, fill_color.a);
    SDL_RenderFillRect(m_renderer, &rect);
    SDL_SetRenderDrawColor(m_renderer, outline_color.r, outline_color.g, outline_color.b, outline_color.a);
    SDL_RenderDrawRect(m_renderer, &rect);
}

void Renderer::blit(SDL_Texture *texture, SDL_Rect src_rect, SDL_Rect dst_rect) const
{
    dst_rect.x *= m_zoom;
    dst_rect.y *= m_zoom;
    dst_rect.w *= m_zoom;
    dst_rect.h *= m_zoom;
    SDL_RenderCopy(m_renderer, texture, &src_rect, &dst_rect);
}

void Renderer::present()
{
    SDL_RenderPresent(m_renderer);
}

void Renderer::zoom(int zoom_mod)
{
    m_zoom += zoom_mod;
    if (m_zoom < 1)
    {
        m_zoom = 1;
    }
}

int Renderer::get_width() const
{
    return m_window_width / m_zoom;    
}

int Renderer::get_height() const
{
    return m_window_height / m_zoom;
}
