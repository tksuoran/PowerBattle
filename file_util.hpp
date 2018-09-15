#ifndef FILE_UTIL_HPP
#define FILE_UTIL_HPP

#include <vector>
#include <string>

std::vector<unsigned char> read_file(const char *const path);

std::string get_string(const std::vector<unsigned char> &data, size_t offset, size_t length);

#endif // FILE_UTIL_HPP
