#include "file_util.hpp"

std::vector<unsigned char> read_file(const char *const path)
{
    if (path == nullptr)
    {
        fprintf(stderr, "read_file(): path = nullptr\n");
        fflush(stderr);
        abort();
    }

    FILE *file = fopen(path, "rb");
    if (file == nullptr)
    {
        fprintf(stderr, "Failed to open '%s'\n", path);
        fflush(stderr);
        abort();
    }
    fseek(file, 0L, SEEK_END);
    long file_length = ftell(file);
    rewind(file);
    std::vector<unsigned char> data(file_length);
    fread(data.data(), file_length, 1, file);
    fclose(file);
    return data;
}

std::string get_string(const std::vector<unsigned char> &data, size_t offset, size_t length)
{
    std::string result(length, 0);
    for (size_t pos = 0; pos < length; pos++)
    {
        result[pos] = static_cast<char>(data[offset + pos]);
    }
    return result;
}
