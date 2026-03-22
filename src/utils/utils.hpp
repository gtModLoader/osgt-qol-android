#pragma once
#include <android/log.h>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

#define LOG_TAG "GROWTOPIA-OSGT-QOL-ANDROID"
#define LOG_DEBUG(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOG_ERROR(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define RESOLVE_SYMBOL(name)                                                                                           \
    real::name = (name##_t)game.resolveSymbol(pattern::name);                                                          \
    if (real::name == nullptr)                                                                                         \
    {                                                                                                                  \
        LOG_DEBUG("WARN: Couldn't solve symbol name");                                                                 \
    }

// Taken from And64InlineHook
#include <sys/mman.h>
#define __intval(p) reinterpret_cast<intptr_t>(p)
#define __uintval(p) reinterpret_cast<uintptr_t>(p)
#define __ptr(p) reinterpret_cast<void*>(p)
#define __page_size 4096
#define __page_align(n) __align_up(static_cast<uintptr_t>(n), __page_size)
#define __ptr_align(x) __ptr(__align_down(reinterpret_cast<uintptr_t>(x), __page_size))
#define __align_up(x, n) (((x) + ((n) - 1)) & ~((n) - 1))
#define __align_down(x, n) ((x) & -(n))
#define __countof(x) static_cast<intptr_t>(sizeof(x) / sizeof((x)[0])) // must be signed
#define __atomic_increase(p) __sync_add_and_fetch(p, 1)
#define __sync_cmpswap(p, v, n) __sync_bool_compare_and_swap(p, v, n)
#define __predict_true(exp) __builtin_expect((exp) != 0, 1)
#define __flush_cache(c, n) __builtin___clear_cache(reinterpret_cast<char*>(c), reinterpret_cast<char*>(c) + n)
#define __make_rwx(p, n)                                                                                               \
    ::mprotect(__ptr_align(p),                                                                                         \
               __page_align(__uintval(p) + n) != __page_align(__uintval(p)) ? __page_align(n) + __page_size            \
                                                                            : __page_align(n),                         \
               PROT_READ | PROT_WRITE | PROT_EXEC)

using RawPattern = std::vector<uint16_t>;

namespace utils
{

// Parse an IDA-style memory pattern string. Returns a vector of bytes where std::nullopt
// represents a wildcard byte. Throws std::invalid_argument on invalid pattern.
RawPattern parsePattern(const std::string& pattern);

// Write buffer data to a memory address.
void writeMemoryBuffer(void* address, const std::vector<uint8_t> data);

// Write buffer data to a memory address.
void writeMemoryBuffer(void* address, const void* data, size_t size);

// Write IDA-style memory pattern to a memory address. The function will jump over wildcard bytes in
// the pattern.
void writeMemoryPattern(void* address, const std::string& pattern);

// Fill a memory region with a specific value.
void fillMemory(void* address, size_t size, uint8_t value);

// Shorthand for writeMemoryPattern(address, "1F 20 03 D5").
void nopInstruction(void* address, int n=1);

// Debug print out region of memory
void readBackMemory(void* address, size_t size);

template <typename T> inline T resolveAdrpAddToAddress(void* address);

} // namespace utils

template <typename T> inline T utils::resolveAdrpAddToAddress(void* address)
{
    // Resolves ADRP + ADD sequence used to calculate a pointer from 2x 32-bit instructions.
    uint8_t* ptr = reinterpret_cast<uint8_t*>(address);
    if (ptr == NULL)
        throw std::invalid_argument("Address does not point to valid memory.");

    uint32_t adrp_op, add_op;
    std::memcpy(&adrp_op, ptr, 4);
    std::memcpy(&add_op, ptr + 4, 4);

    // Check if it's an ADRP (bits 31, 28:24 must be 1 10000)
    bool isAdrp = (adrp_op & 0x9F000000) == 0x90000000;
    // Check if it's an ADD immediate (bits 31:22 must be 1 001000100)
    bool isAdd = (add_op & 0xFFC00000) == 0x91000000;

    if (!isAdrp || !isAdd)
        throw std::invalid_argument("Address does not point to ADRP+ADD instructions.");

    // Destination register of ADRP (bits 4:0)
    uint32_t adrp_rd = adrp_op & 0x1F;
    // Source register of ADD (bits 9:5)
    uint32_t add_rn = (add_op >> 5) & 0x1F;

    if (adrp_rd != add_rn)
        throw std::invalid_argument("ADRP destination does not match ADD source register.");

    // Resolve ADRP to a page
    // Extract immhi (bits 23:5) and immlo (bits 30:29)
    uint64_t immhi = (adrp_op >> 5) & 0x7FFFF;
    uint64_t immlo = (adrp_op >> 29) & 0x3;

    // Combine into a 21-bit signed immediate
    int64_t imm = (int64_t)((immhi << 2) | immlo);

    // Sign extend from 21 bits to 64 bits
    if (imm & 0x100000)
        imm |= ~0x1FFFFF;
    uint64_t page = (reinterpret_cast<uint64_t>(ptr) & ~0xFFFULL) + (imm << 12);

    // Get offset from ADD, extracted from 12-bit immediate (bits 21:10)
    uint32_t offset = (add_op >> 10) & 0xFFF;
    if ((add_op >> 22) & 1)
        offset <<= 12;

    // Add together page and offset and that's the destination.
    return reinterpret_cast<T>(page + offset);
}