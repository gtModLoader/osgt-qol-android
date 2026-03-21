#include <jni.h>
#include <version.h>
#include "game/game.hpp"
#include "utils/utils.hpp"
#include "patch/patch.hpp"

void setup()
{
    LOG_DEBUG("Launching OSGT-QOL-ANDROID " OSGT_QOL_DISPLAY_VERSION);

    auto& game = game::GameHarness::get();
    auto& patchMgr = patch::PatchManager::get();

    game.initialize();
    game.resolveSharedSigs();
    patchMgr.applyPatchesFromFile("patches.txt");
}

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    setup();
    return JNI_VERSION_1_6;
}