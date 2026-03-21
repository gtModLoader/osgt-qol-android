#pragma once
#include <string>

typedef void* (*UbiAllocCtor_t)(void* self);
typedef void* (*UbiStrCtor_t)(void* self, const char* str, void* alloc);
typedef void* (*UbiStrDtor_t)(void* self);

extern UbiAllocCtor_t g_ubi_alloc_ctor;
extern UbiStrCtor_t g_ubi_str_ctor;
extern UbiStrDtor_t g_ubi_str_dtor;

// Wrapper class for Ubiservices allocated strings. It looks like these things are ref-counted, so all we need is alloc
// ctor and str ctor.
class UbiString
{
    void* game_ptr = nullptr;

  public:
    UbiString() : game_ptr(nullptr) {}
    UbiString(const char* s)
    {
        if (!s)
            return;
        alignas(8) char temp_alloc = 0;
        g_ubi_alloc_ctor(&temp_alloc);
        g_ubi_str_ctor(&game_ptr, s, &temp_alloc);
    }
    UbiString(const std::string& s) : UbiString(s.c_str()) {}

    ~UbiString() {}

    UbiString& operator=(const char* str)
    {
        if (str)
        {
            alignas(8) char temp_alloc = 0;
            g_ubi_alloc_ctor(&temp_alloc);
            g_ubi_str_ctor(&game_ptr, str, &temp_alloc);
        }
        return *this;
    }

    std::string getString() const
    {
        const char* str = c_str();
        return str ? std::string(str) : std::string("");
    }

    operator std::string() const
    {
        return getString();
    }

    bool operator==(std::string rhs) { return getString() == rhs; }
    operator void*() { return (void*)this; }
    operator const void*() const { return (const void*)this; }
    const char* c_str() const { return (const char*)game_ptr; }
};