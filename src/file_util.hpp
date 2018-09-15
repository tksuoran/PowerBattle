#ifndef FILE_UTIL_HPP
#define FILE_UTIL_HPP

#include <vector>
#include <string>

std::vector<unsigned char> read_file(const char *const path);

std::string read_file_string(const char *const path);

void write_file(const char *const path, const unsigned char *buffer, size_t length);

void write_file(const char *const path, std::string text);

std::string get_string(const std::vector<unsigned char> &data, size_t offset, size_t length);

#endif // FILE_UTIL_HPP
