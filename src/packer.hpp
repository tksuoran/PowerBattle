#ifndef PACKER_HPP
#define PACKER_HPP

#include <algorithm>
#include <vector>

#include <fmt/format.h>
#include <gsl/gsl_assert>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "vec2.hpp"

class PackItem
{
public:
    PackItem(SDL_Surface* surface, uint16_t code)
        : m_surface(surface)
        , m_code(code)
    {
    }

    PackItem(const PackItem& other)
        : m_surface(other.m_surface)
        , m_code(other.m_code)
        , m_rect(other.m_rect)
    {
    }

    PackItem(PackItem&& other) : m_surface(nullptr)
    {
        m_surface = other.m_surface;
        m_code = other.m_code;
        m_rect = other.m_rect;
        other.m_surface = nullptr;
        other.m_code = 00;
    }

    PackItem& operator=(const PackItem& other)
    {
        m_surface = other.m_surface;
        m_code = other.m_code;
        m_rect = other.m_rect;
        return *this;
    }

    PackItem& operator=(PackItem&& other)
    {
        if (this != &other)
        {
            m_surface = other.m_surface;
            m_code = other.m_code;
            m_rect = other.m_rect;
        }
        return *this;
    }

    ivec2 get_extent() const
    {
        if (m_surface != nullptr)
        {
            return ivec2(m_surface->w, m_surface->h);
        }
        else
        {
            return ivec2(0.0f, 0.0f);
        }
    }

    void set_offset(ivec2 offset)
    {
        if (m_surface != nullptr)
        {
            m_rect.x = offset.x;
            m_rect.y = offset.y;
            m_rect.w = m_surface->w;
            m_rect.h = m_surface->w;
            //fprintf(stderr, "%d, %d, %d, %d\n", m_rect.x, m_rect.y, m_rect.w, m_rect.h);
            //fflush(stderr);
        }
    }

    SDL_Surface*    m_surface;
    uint16_t        m_code;
    SDL_Rect        m_rect;
};

class Packer
{
public:
    void add(PackItem&& packItem)
    {
        m_items.push_back(std::move(packItem));
    }

    bool try_pack(int atlas_size)
    {
        ivec2 position;
        int shelf_height = 0;
        for (auto& item : m_items)
        {
            if (item.m_surface == nullptr)
            {
                continue;
            }
            ivec2 extent = item.get_extent(); 
            if (position.x + extent.x < atlas_size)
            {
                if (position.y + extent.y >= atlas_size)
                {
                    return false;
                }
                item.set_offset(position);
                position.x += extent.x;
                shelf_height = std::max(shelf_height, extent.y);
            }
            else
            {
                if (position.y + extent.y >= atlas_size)
                {
                    return false;
                }
                position.y += shelf_height;
                position.x = 0;
                if (position.x + extent.x >= atlas_size)
                {
                    return false;
                }
                item.set_offset(position);
                position.x += extent.x;
                shelf_height = extent.y;
            }
        }
        return true;
    }

    void pack(SDL_Renderer* renderer)
    {
        int m_area_sum = 0;
        for (auto item : m_items)
        {
            m_area_sum += item.get_extent().x * item.get_extent().y;
        }

        int edge = static_cast<int>(std::sqrt(m_area_sum));

        for (;;)
        {
            bool ok = try_pack(edge);
            if (ok)
            {
                break;
            }
            edge += std::min(edge / 4, 1);
            if (edge > 9000)
            {
                fmt::print(stderr, "Packer::pack() failed.\n");
                fflush(stderr);
                abort();
            }
        }

        Expects(m_items.size() > 0U);
        SDL_Surface* first_surface = nullptr;
        for (auto& i : m_items)
        {
            if (i.m_surface != nullptr)
            {
                first_surface = i.m_surface;
                break;
            }
        }
        Expects(first_surface != nullptr);
        SDL_PixelFormat* format = first_surface->format;
        Uint32 format_enum = SDL_MasksToPixelFormatEnum(format->BitsPerPixel, format->Rmask, format->Gmask, format->Bmask, format->Amask);
        if (format_enum == SDL_PIXELFORMAT_UNKNOWN)
        {
            fmt::print(stderr, "Packer: SDL surface format not recognized.\n");
            fflush(stderr);
            abort();
        }
        SDL_Surface* atlas_surface = SDL_CreateRGBSurfaceWithFormat(0, edge, edge, 0, format_enum);
        if (atlas_surface == nullptr)
        {
            fmt::print(stderr, "Packer: SDL_CreateRGBSurfaceWithFormat() failed.\n");
            fflush(stderr);
            abort();
        }
        SDL_FillRect(atlas_surface, nullptr, SDL_MapRGBA(atlas_surface->format, 0U, 0U, 0U, 0U));

        for (auto& i : m_items)
        {
            SDL_BlitSurface(i.m_surface, nullptr, atlas_surface, &i.m_rect);
            SDL_FreeSurface(i.m_surface);
            i.m_surface = nullptr;
        }

        if (IMG_SavePNG(atlas_surface, "data/graphics/font.png") < 0)
        {
            const char *error_message = SDL_GetError();
            fmt::print(stderr, "IMG_SavePNG() failed: {}\n",
                       (error_message != nullptr) ? error_message : "");
            fflush(stderr);
            exit(EXIT_FAILURE);
        }

        m_texture = SDL_CreateTextureFromSurface(renderer, atlas_surface);

        SDL_FreeSurface(atlas_surface);
    }

    SDL_Texture* get_texture() const
    {
        return m_texture;
    }

    SDL_Rect get_rect(uint16_t code) const
    {
        for (auto i : m_items)
        {
            if (i.m_code == code)
            {
                return i.m_rect;
            }
        }
        return SDL_Rect{0, 0, 0, 0};
    }

    static bool compare_width(const PackItem& lhs, const PackItem rhs)
    {
        return lhs.get_extent().x < rhs.get_extent().x;
    }

    static bool compare_height(const PackItem& lhs, const PackItem rhs)
    {
        return lhs.get_extent().y < rhs.get_extent().y;
    }

    void sort_by_heights()
    {
        std::sort(m_items.begin(), m_items.end(), compare_height);
    }

    void sort_by_widths()
    {
        std::sort(m_items.begin(), m_items.end(), compare_width);
    }

private:
    std::vector<PackItem>   m_items;
    SDL_Texture*            m_texture;
};

#endif // PACKER_HPP
