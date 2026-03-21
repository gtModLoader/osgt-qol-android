#include "game.hpp"
#include <dlfcn.h>

namespace game
{

GameHarness& GameHarness::get()
{
    static GameHarness instance;
    return instance;
}

void GameHarness::initialize()
{
    handle = dlopen("libgrowtopia.so", RTLD_NOW | RTLD_NOLOAD);
    if (!handle)
    {
        handle = dlopen("libgrowtopia.so", RTLD_NOW);
    }
}

void* GameHarness::resolveSymbol(std::string& pattern) { return dlsym(handle, pattern.c_str()); }

} // namespace game