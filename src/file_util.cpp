#include <fmt/format.h>

#include "file_util.hpp"

std::vector<unsigned char> read_file(const char *const path)
{
    if (path == nullptr)
    {
        fmt::print(stderr, "read_file(): path = nullptr\n");
        std::fflush(stderr);
        std::abort();
    }

    FILE *file = std::fopen(path, "rb");
    if (file == nullptr)
    {
        fmt::print(stderr, "Failed to open '{}'\n", path);
        std::fflush(stderr);
        std::abort();
    }
    std::fseek(file, 0L, SEEK_END);
    long file_length = std::ftell(file);
    std::rewind(file);
    std::vector<unsigned char> data(file_length);
    std::fread(data.data(), file_length, 1, file);
    std::fclose(file);
    return data;
}

std::string read_file_string(const char *const path)
{
    if (path == nullptr)
    {
        fmt::print(stderr, "read_file(): path = nullptr\n");
        std::fflush(stderr);
        std::abort();
    }

    FILE *file = std::fopen(path, "rb");
    if (file == nullptr)
    {
        fmt::print(stderr, "Failed to open '{}'\n", path);
        std::fflush(stderr);
        std::abort();
    }
    std::fseek(file, 0L, SEEK_END);
    long file_length = std::ftell(file);
    std::rewind(file);
    std::vector<unsigned char> data(file_length);
    std::fread(data.data(), file_length, 1, file);
    std::fclose(file);
    return std::string(data.begin(), data.end());
}

void write_file(const char *const path, const unsigned char *buffer, size_t length)
{
    if (path == nullptr)
    {
        fmt::print(stderr, "write_file(): path = nullptr\n");
        std::fflush(stderr);
        std::abort();
    }

    FILE *file = std::fopen(path, "wb");
    if (file == nullptr)
    {
        fmt::print(stderr, "Failed to open '{}'\n", path);
        std::fflush(stderr);
        std::abort();
    }
    size_t res = std::fwrite(buffer, 1, length, file);
    if (res != length)
    {
        fmt::print(stderr, "Failed to write '{}'\n", path);
        std::fflush(stderr);
        std::abort();
    }
    std::fclose(file);
}

void write_file(const char *const path, std::string text)
{
    if (path == nullptr)
    {
        fmt::print(stderr, "write_file(): path = nullptr\n");
        std::fflush(stderr);
        std::abort();
    }

    FILE *file = std::fopen(path, "wb");
    if (file == nullptr)
    {
        fmt::print(stderr, "Failed to open '{}'\n", path);
        std::fflush(stderr);
        std::abort();
    }
    size_t res = std::fwrite(text.data(), 1, text.size(), file);
    if (res != text.size())
    {
        fmt::print(stderr, "Failed to write '{}'\n", path);
        std::fflush(stderr);
        std::abort();
    }
    std::fclose(file);
}

std::string get_string(const std::vector<unsigned char> &data, size_t offset, size_t length)
{
    std::string result(length, 0);
    for (size_t pos = 0U; pos < length; pos++)
    {
        result[pos] = static_cast<char>(data[offset + pos]);
    }
    return result;
}
