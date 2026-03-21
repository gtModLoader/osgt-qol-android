#pragma once
#include "And64InlineHook.hpp"
#include <string>

// Utility macro for easy declaration of game functions. This will create a global function pointer
// alias called <name>_t, and create an instance of said function pointer in the real namespace.
// The pattern will be accessible in the pattern namespace. (e.g. pattern::name). Do not use this
// macro in header files to avoid namespace pollution.
#define REGISTER_GAME_FUNCTION(name, patt, ret, ...)                                                                   \
    using name##_t = ret (*)(__VA_ARGS__);                                                                             \
    namespace real                                                                                                     \
    {                                                                                                                  \
    name##_t name = nullptr;                                                                                           \
    }                                                                                                                  \
    namespace pattern                                                                                                  \
    {                                                                                                                  \
    std::string name = patt;                                                                                           \
    }
#define REGISTER_GAME_GLOBAL_VAR(name, patt, ret)                                                                      \
    namespace real                                                                                                     \
    {                                                                                                                  \
    ret name = nullptr;                                                                                                \
    }                                                                                                                  \
    namespace pattern                                                                                                  \
    {                                                                                                                  \
    std::string name = patt;                                                                                           \
    }

namespace game
{

// Responsible for interacting with internal game mechanics and memory.
class GameHarness
{
  public:
    // Get GameHarness singleton instance.
    static GameHarness& get();

    // Initialization function. This cannot be performed at construct-time due to the fact the game
    // itself is not fully initialized at that point (the constructor may be called during static
    // initialization of other objects).
    void initialize();

    // Temporarily used to store various function signature resolves that are used in
    // structs/classes. This should be probably reworked when an API-consumer model is created.
    void resolveSharedSigs();

    // Search game memory for a specific byte pattern. Returns a pointer to the first occurrence.
    // Throws std::invalid_argument on invalid pattern or std::runtime_error if failed to find.
    template <typename T = void*> T findMemoryPattern(const std::string& pattern);

    // Detours function located at target address to function located at detour address. Optionally,
    // a pointer might be passed in order to store the original (undetoured) function address.
    // Throws std::runtime_error on failure.
    template <typename F> void hookFunction(void* target, F detour, F* original, bool hasPriority = false);

    void* resolveSymbol(std::string& pattern);

    // Enables all created function hooks
    void finalizeInitialization();

    void* getGameHandle() const { return handle; };

    GameHarness(GameHarness const&) = delete;
    void operator=(GameHarness const&) = delete;

  private:
    // Image size (size of the game image in memory).
    size_t imageSize;

    void* handle;

    GameHarness() = default;
};

} // namespace game

///////////////////////////////////
// TEMPLATE FUNCTION DEFINITIONS //
///////////////////////////////////

template <typename F> void game::GameHarness::hookFunction(void* target, F detour, F* original, bool hasPriority)
{
    A64HookFunction(target, (void*)detour, (void**)original);
}