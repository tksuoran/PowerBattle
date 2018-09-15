#include <fmt/format.h>

#include "packer.hpp"
#include "renderer.hpp"
#include "util.hpp"

void Renderer::init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) < 0)
    {
        fmt::print(stderr, "SDL_Init() failed\n");
        fflush(stderr);
        abort();
    }

    if (IMG_Init(IMG_INIT_PNG) < 0)
    {
        fmt::print(stderr, "IMG_Init() failed\n");
        fflush(stderr);
        abort();
    }

    if (TTF_Init() < 0)
    {
        fmt::print(stderr, "TTF_Init() failed\n");
        fflush(stderr);
        abort();
    }

    constexpr int default_window_width  = 800;
    constexpr int default_window_height = 600;

    m_window_width = default_window_width;
    m_window_height = default_window_height;

    m_window = SDL_CreateWindow("PB", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                m_window_width, m_window_height,
                                SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
    if (m_window == nullptr)
    {
        fmt::print(stderr, "SDL_CreateWindow() failed\n");
        fflush(stderr);
        abort();
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);
    if (m_renderer == nullptr)
    {
        fmt::print(stderr, "SDL_CreateRenderer() failed\n");
        fflush(stderr);
        abort();
    }

    load_font();
}

Renderer::~Renderer()
{
    deinit();
}

void Renderer::deinit()
{
    TTF_Quit();
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
    IMG_Quit();
    SDL_Quit();
    m_renderer = nullptr;
    m_window = nullptr;
}

void Renderer::load_font()
{
    //m_font = TTF_OpenFont("data/fonts/SourceSerifPro-Regular.otf", 32);
    //m_font = TTF_OpenFont("data/fonts/SourceCodePro-Semibold.otf", 15);
    m_font = TTF_OpenFont("data/fonts/SourceCodePro-Bold.otf", 16);
    if (m_font == nullptr)
    {
        fmt::print(stderr, "TTF_OpenFont() failed: %s\n", TTF_GetError());
        fflush(stderr);
        abort();
    }

    TTF_SetFontStyle(m_font, TTF_STYLE_NORMAL);
    TTF_SetFontKerning(m_font, SDL_TRUE);
    TTF_SetFontHinting(m_font, TTF_HINTING_LIGHT);
    TTF_SetFontOutline(m_font, SDL_FALSE);
	SDL_Color fg = { float_to_unorm8(1.0f), float_to_unorm8(1.0f), float_to_unorm8(1.0f), float_to_unorm8(1.0f) };
    std::vector<SDL_Surface*> m_glyphs;
    m_glyphs.resize(128);
    Packer packer;

	for (unsigned int i = 0U; i < 128U; i++)
	{
        auto &glyph_metric = m_glyph_metrics[i];
        packer.add(PackItem(TTF_RenderGlyph_Blended(m_font, i, fg),
                            uint16_t(i)));
        //m_glyphs[i] = TTF_RenderGlyph_Blended(m_font, i, fg);
        TTF_GlyphMetrics(m_font,
                         static_cast<Uint16>(i),
                         &glyph_metric.min_x,
                         &glyph_metric.max_x,
                         &glyph_metric.min_y,
                         &glyph_metric.max_y,
                         &glyph_metric.advance);
    }

    packer.pack(m_renderer);

    m_font_texture = packer.get_texture();
    if (m_font_texture == nullptr)
    {
        fmt::print(stderr, "Renderer::load_font(): packing failed\n");
        fflush(stderr);
        abort();
    }

    constexpr unsigned int glyph_count{128U};
	for (unsigned int i = 0U; i < glyph_count; i++)
	{
        m_glyph_metrics[i].rect = packer.get_rect(i);
    }
}

void Renderer::draw_text(int x, int y, const char *const text) const
{
    x *= m_zoom;
    y *= m_zoom;
    //y += TTF_FontAscent(m_font);
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    //rect.w = 10 * m_zoom;
    //rect.h = 10 * m_zoom;
    auto ptr = reinterpret_cast<const uint8_t*>(text);
    //SDL_RenderDrawRect(m_renderer, &rect);
    SDL_SetRenderDrawColor(m_renderer, float_to_unorm8(1.0F), float_to_unorm8(1.0F), float_to_unorm8(0.0F), float_to_unorm8(1.0F));
    while (uint8_t c = *ptr++)
    {
        rect.x = x + m_glyph_metrics[c].min_x;
        rect.y = y;
        rect.w = m_glyph_metrics[c].advance;
        rect.h = TTF_FontLineSkip(m_font);
        SDL_RenderCopy(m_renderer, m_font_texture, &m_glyph_metrics[c].rect, &rect);
        //SDL_RenderDrawRect(m_renderer, &rect);

        x += m_glyph_metrics[c].advance;
    }
}

void Renderer::window_event(const SDL_Event& e)
{
    if (e.type == SDL_WINDOWEVENT)
    {
        if (e.window.event == SDL_WINDOWEVENT_RESIZED)
        {
            resize(e.window.data1, e.window.data2);
            if (m_window_event_handler != nullptr)
            {
                m_window_event_handler->resize(m_window_width, m_window_height);
            }
        }
    }
    else if (e.type == SDL_QUIT)
    {
        if (m_window_event_handler != nullptr) {
            m_window_event_handler->quit();
        }
    }
}

void Renderer::key_event(const SDL_Event& e)
{
    if (m_key_event_handler != nullptr)
    {
        m_key_event_handler->key_event(e);
    }
}

void Renderer::mouse_event(const SDL_Event& e)
{
    if (m_mouse_event_handler != nullptr)
    {
        m_mouse_event_handler->mouse_event(e);
    }
}

void Renderer::set_window_event_handler(WindowEventHandler* handler)
{
    m_window_event_handler = handler;
}

void Renderer::set_key_event_handler(KeyEventHandler* handler)
{
    m_key_event_handler = handler;
}

void Renderer::set_mouse_event_handler(MouseEventHandler* handler)
{
    m_mouse_event_handler = handler;
}

void Renderer::handle_events()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        switch (e.type)
        {
            case SDL_WINDOWEVENT:
            case SDL_QUIT:
            {
                window_event(e);
                break;
            }

            case SDL_KEYDOWN:
            case SDL_KEYUP:
            {
                key_event(e);
                break;
            }

            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEBUTTONDOWN:
            {
                m_mouse_position.x = e.button.x;
                m_mouse_position.y = e.button.y;
                mouse_event(e);
                break;
            }

            case SDL_MOUSEMOTION:
            {
                m_mouse_position.x = e.motion.x;
                m_mouse_position.y = e.motion.y;
                mouse_event(e);
                break;
            }

            default:
            {
                break;
            }
        }
    }
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

void Renderer::set_texture_alpha_mode(SDL_Texture *texture, float alpha)
{
    SDL_SetTextureAlphaMod(texture, float_to_unorm8(alpha));
}

SDL_Texture *Renderer::load_texture(const char *const filename)
{
    SDL_Texture *result = IMG_LoadTexture(m_renderer, filename);
    if (result == nullptr)
    {
        const char *error_message = SDL_GetError();
        fmt::print(stderr,
                   "IMG_LoadTexture({}) failed: {}\n",
                   filename,
                   (error_message != nullptr) ? error_message : "");
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
