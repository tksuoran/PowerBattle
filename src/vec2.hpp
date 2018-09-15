#ifndef VEC2_HPP
#define VEC2_HPP

#include <cassert>
#include <cstddef>

template<typename T>
class Vec2
{
public:
    T x;
    T y;

    Vec2() : x(T(0)), y(T(0))
    {
    }

    Vec2(T x, T y) : x(x), y(y)
    {
    }

    Vec2<T> operator+(const Vec2<T>& other) const
    {
        return Vec2(x + other, y + other.y);
    }

    Vec2<T> operator-(const Vec2<T>& other) const
    {
        return Vec2(x - other, y - other.y);
    }

    T& operator[](size_t index)
    {
        if (index == 0U)
        {
            return x;
        }
        assert(index == 1U);
        return y;
    }

    Vec2<T>& operator+=(const Vec2<T>& other)
    {
        this->x += other.x;
        this->y += other.y;
        return *this;
    }

    Vec2<T>& operator-=(const Vec2<T>& other)
    {
        this->x -= other.x;
        this->y -= other.y;
        return *this;
    }
};

template <typename T>
Vec2<T> operator*(T lhs, const Vec2<T>& rhs)
{
    return Vec2<T>(lhs * rhs.x, lhs * rhs.y);
}

template <typename T>
Vec2<T> operator/(const Vec2<T>& lhs, T rhs)
{
    return Vec2<T>(lhs.x / rhs, lhs * rhs.y);
}

typedef Vec2<bool> bvec2;
typedef Vec2<int> ivec2;
typedef Vec2<unsigned int> uivec2;
typedef Vec2<float> vec2;
typedef Vec2<double> dvec2;

#endif // VEC2_HPP
