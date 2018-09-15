#include "color.hpp"
#include "game.hpp"
#include "hotbar.hpp"
#include "map.hpp"

void Hotbar::draw() const
{
    m_game->m_renderer.set_blend_mode(SDL_BLENDMODE_BLEND);
    m_game->m_renderer.draw_rect(Color(1.0F, 1.0F, 1.0F, 0.5F),
                                 Color(0.0F, 0.0F, 0.0F, 0.5F), get_sdl_rect());
    m_game->m_renderer.set_blend_mode(SDL_BLENDMODE_NONE);
}
