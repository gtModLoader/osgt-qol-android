#pragma once
#include <android/log.h>
#include <vector>
#include <string>

#define LOG_TAG "GROWTOPIA-OSGT-QOL-ANDROID"
#define LOG_DEBUG(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define RESOLVE_SYMBOL(name) real::name = (name##_t)game.resolveSymbol(pattern::name); \
if (real::name == nullptr) { LOG_DEBUG("WARN: Couldn't solve symbol name"); }

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

// Shorthand for writeMemoryPattern(address, "1F 20 03 D5").
void nopMemory(void* address);

} // namespace utils
