#include <jni.h>
#include <version.h>
#include "game/game.hpp"
#include "utils/utils.hpp"
#include "patch/patch.hpp"

REGISTER_GAME_FUNCTION(FocusInputComponentOnInput, "_ZN19FocusInputComponent7OnInputEP11VariantList", void, void*, void*);
void setup()
{
    LOG_DEBUG("Launching OSGT-QOL-ANDROID " OSGT_QOL_DISPLAY_VERSION);

    auto& game = game::GameHarness::get();
    auto& patchMgr = patch::PatchManager::get();
    auto& optionsMgr = game::OptionsManager::get();

    game.initialize();
    game.resolveSharedSigs();

    // Do what we do on PC and kill one of the LogMsg spammers.
    RESOLVE_SYMBOL(FocusInputComponentOnInput);
    utils::nopMemory((void*)(((int64_t)real::FocusInputComponentOnInput) + 48));
    optionsMgr.initialize();
    optionsMgr.addOptionPage("qol", "OSGT-QOL Settings");
    patchMgr.applyPatchesFromFile("patches.txt");
}

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    setup();
    return JNI_VERSION_1_6;
}