#include "hotbar.hpp"

#include "color.hpp"
#include "map.hpp"
#include "game.hpp"

void Hotbar::draw() const
{
    m_game->m_renderer.set_blend_mode(SDL_BLENDMODE_BLEND);
    m_game->m_renderer.draw_rect(Color(255U, 255U, 255U, 128U), Color(0U, 0U, 0U, 128U), get_sdl_rect());
    m_game->m_renderer.set_blend_mode(SDL_BLENDMODE_NONE);
}
