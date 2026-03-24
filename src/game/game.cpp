#include "game.hpp"
#include <dlfcn.h>
#include "utils/utils.hpp"

namespace game
{

GameHarness& GameHarness::get()
{
    static GameHarness instance;
    return instance;
}

void GameHarness::initialize(JavaVM* vm)
{
    handle = dlopen("libgrowtopia.so", RTLD_NOW | RTLD_NOLOAD);
    if (!handle)
    {
        handle = dlopen("libgrowtopia.so", RTLD_NOW);
    }
    jVM = vm;
}

void* GameHarness::resolveSymbol(std::string& pattern) {
    void* sym = dlsym(handle, pattern.c_str());
    if (sym == nullptr)
        LOG_ERROR("Failed to resolve pattern %s", pattern.c_str());
    return sym;
}

} // namespace game