#include "game/game.hpp"
#include "game/signatures.hpp"
#include "patch/patch.hpp"

REGISTER_GAME_FUNCTION(SetFPSLimit, "_ZN7BaseApp11SetFPSLimitEf", void, void*, float);
class FramerateUnlockPatch : public patch::BasePatch
{
  public:
    void apply() const override
    {
        auto& game = game::GameHarness::get();
        // Hook SetFPSLimit.
        game.hookFunction<SetFPSLimit_t>(game.resolveSymbol(pattern::SetFPSLimit), SetFPSLimit, &real::SetFPSLimit);
    }

    static void SetFPSLimit(void* app, float fps)
    {
        real::GetApp()->m_bShowFPS = true;
        // TODO: Find out how to query active refresh rate on Android.
        fps = 120;
        real::SetFPSLimit(app, fps);
    }
};
REGISTER_USER_GAME_PATCH(FramerateUnlockPatch, framerate_unlock);
