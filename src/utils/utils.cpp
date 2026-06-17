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

void readBackMemory(void* address, size_t size)
{
    uint8_t* ptr = reinterpret_cast<uint8_t*>(address);
    std::stringstream ss;
    for (int i = 0; i < size; i++)
    {
        ss << std::hex << (int)(uint8_t)*ptr << ' ';
        ptr++;
        if (i % 4 == 0 && i != 0)
        {
            LOG_DEBUG("mem: %s", ss.str().c_str());
            ss.clear();
            ss.str("");
        }
    }
    LOG_DEBUG("mem: %s", ss.str().c_str());
}

void installLossyJumpHook(void* target_func, void* hook_func)
{
    // Calculate page alignment for mprotect
    uintptr_t page_start = (uintptr_t)target_func & ~(PAGE_SIZE - 1);
    mprotect((void*)page_start, PAGE_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC);

    // Calculate the PC-relative offset
    int64_t offset = (int64_t)hook_func - (int64_t)target_func;

    // ARM64 B instruction encoding: 0x14000000 | (offset >> 2)
    // Note: The offset must be within +/- 128MB, not a problem for Growtopia, since the library itself is not that
    // large.
    uint32_t b_opcode = 0x14000000 | ((offset >> 2) & 0x03FFFFFF);

    // Overwrite the very first instruction of the target function
    *(volatile uint32_t*)target_func = b_opcode;

    // Clear instruction cache so the CPU sees the change
    __builtin___clear_cache((char*)target_func, (char*)target_func + 4);
}

void nopInstruction(void* address, int n)
{
    uint8_t* ptr = reinterpret_cast<uint8_t*>(address);
    for (int i = 0; i < n; i++)
    {
        writeMemoryPattern(ptr + (i * 4), "1F 20 03 D5");
    }
}

} // namespace utils