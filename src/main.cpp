#include "game/game.hpp"
#include "patch/patch.hpp"
#include "utils/utils.hpp"
#include <jni.h>
#include <version.h>
#include <chrono>


REGISTER_GAME_FUNCTION(FocusInputComponentOnInput, "_ZN19FocusInputComponent7OnInputEP11VariantList", void, void*,
                       void*);
void setup(JavaVM* vm)
{
    LOG_DEBUG("Launching OSGT-QOL-ANDROID " OSGT_QOL_DISPLAY_VERSION);

    auto startTime = std::chrono::high_resolution_clock::now();

    auto& game = game::GameHarness::get();
    auto& patchMgr = patch::PatchManager::get();
    auto& optionsMgr = game::OptionsManager::get();
    auto& weatherMgr = game::WeatherManager::get();
    auto& events = game::EventsAPI::get();

    game.initialize(vm);
    game.resolveSharedSigs();

    // Do what we do on PC and kill one of the LogMsg spammers.
    RESOLVE_SYMBOL(FocusInputComponentOnInput);
    utils::nopInstruction((void*)(((int64_t)real::FocusInputComponentOnInput) + 48));
    optionsMgr.initialize();
    optionsMgr.addOptionPage("qol", "OSGT-QOL Settings");
    patchMgr.applyPatchesFromFile("patches.txt");
    events.initialize();
    weatherMgr.initialize();

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    LOG_DEBUG("Setup completed in %ld ms", duration);
}

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    setup(vm);
    return JNI_VERSION_1_6;
}