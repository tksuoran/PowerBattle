#ifndef STREAM_HPP
#define STREAM_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <fmt/format.h>
#include <gsl/pointers>    // for owner
#include "unit.hpp"


class FileWriteStream
{
public:
    FileWriteStream(const char *path)
    {
        m_file = fopen(path, "wb");
        if (!m_file) {
            fmt::print(stderr, "File open fail: {} - {}", path, strerror(errno));
            abort();
        }
    }

    ~FileWriteStream()
    {
        fclose(m_file);
    }

    void op(const uint8_t  &v) { fwrite(&v, 1, sizeof(uint8_t ), m_file); }
    void op(const uint16_t &v) { fwrite(&v, 1, sizeof(uint16_t), m_file); }
    void op(const uint32_t &v) { fwrite(&v, 1, sizeof(uint32_t), m_file); }
    void op(const int8_t   &v) { fwrite(&v, 1, sizeof(int8_t  ), m_file); }
    void op(const int16_t  &v) { fwrite(&v, 1, sizeof(int16_t ), m_file); }
    void op(const int32_t  &v) { fwrite(&v, 1, sizeof(int32_t ), m_file); }

    template<typename T>
    void op(const std::vector<T> &v)
    {
        size_t size = v.size();
        if (size > std::numeric_limits<uint32_t>::max())
        {
            fmt::print(stderr, "vector size exceed max size of uint32_t: {}", size);
            abort();
        }
        uint32_t size_u32 = static_cast<uint32_t>(size);
        op(size_u32);
        for (auto &element : v)
        {
            op(v);
        }
    }

private:
    gsl::owner<FILE *> m_file;
};

class FileReadStream
{
public:
    FileReadStream(const char *path)
    {
        m_file = fopen(path, "rb");
        if (!m_file)
        {
            fmt::print(stderr, "File open fail: {} - {}", path, strerror(errno));
            abort();
        }
    }

    ~FileReadStream()
    {
        fclose(m_file);
    }

    void op(uint8_t  &v) { fread(&v, 1, sizeof(uint8_t ), m_file); }
    void op(uint16_t &v) { fread(&v, 1, sizeof(uint16_t), m_file); }
    void op(uint32_t &v) { fread(&v, 1, sizeof(uint32_t), m_file); }
    void op(int8_t   &v) { fread(&v, 1, sizeof(int8_t  ), m_file); }
    void op(int16_t  &v) { fread(&v, 1, sizeof(int16_t ), m_file); }
    void op(int32_t  &v) { fread(&v, 1, sizeof(int32_t ), m_file); }

    template<typename T>
    void op(const std::vector<T> &v)
    {
        uint32_t size_u32;
        op(size_u32);
        v.resize(size_u32);
        v.shrink_to_fit();
        for (auto &element : v)
        {
            op(v);
        }
    }

private:
    gsl::owner<FILE *> m_file;
};

#endif // STREAM_HPP
