#include "utils.hpp"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <string>

namespace utils
{

RawPattern parsePattern(const std::string& pattern)
{
    RawPattern out;
    std::istringstream ss(pattern);
    std::string byte;
    // Parse pattern in tokens.
    while (ss >> byte)
    {
        if (byte == "?" || byte == "??")
            out.emplace_back(UINT16_MAX);
        else
        {
            try
            {
                out.emplace_back(strtol(byte.c_str(), nullptr, 16));
            }
            catch (...)
            {
                // Return empty vector in case of an invalid pattern.
                throw std::invalid_argument("Invalid pattern byte: " + byte);
            }
        }
    }
    return out;
}

void writeMemoryBuffer(void* address, const std::vector<uint8_t> data)
{
    writeMemoryBuffer(address, data.data(), data.size());
}

void writeMemoryBuffer(void* address, const void* data, size_t size)
{
    __make_rwx(address, size);
    memcpy(address, data, size);
}

void writeMemoryPattern(void* address, const std::string& pattern)
{
    auto bytes = parsePattern(pattern);
    __make_rwx(address, bytes.size() * sizeof(uint16_t));
    // Perform special write that skips wildcard bytes.
    uint8_t* ptr = reinterpret_cast<uint8_t*>(address);
    for (size_t i = 0; i < bytes.size(); i++)
    {
        if (bytes[i] != UINT16_MAX)
            *ptr = (uint8_t)bytes[i];
        ptr++;
    }
}

void fillMemory(void* address, size_t size, uint8_t value)
{
    __make_rwx(address, size);
    memset(address, value, size);
}

void nopMemory(void* address) { writeMemoryPattern(address, "1F 20 03 D5"); }

} // namespace utils