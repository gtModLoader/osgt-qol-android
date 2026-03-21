#include "game/game.hpp"
#include "game/signatures.hpp"
#include "game/struct/entity.hpp"
#include "patch/patch.hpp"

REGISTER_GAME_FUNCTION(SetFPSLimit, "_ZN7BaseApp11SetFPSLimitEf", void, void*, float);

static std::vector<std::string> g_fpsOptions = {"60 FPS", "90 FPS", "120 FPS", "240 FPS"};
static float g_fpsValues[4] = {60.0f, 90.0f, 120.0f, 240.0f};
class FramerateUnlockPatch : public patch::BasePatch
{
  public:
    void apply() const override
    {
        auto& game = game::GameHarness::get();
        // Hook SetFPSLimit.
        game.hookFunction<SetFPSLimit_t>(game.resolveSymbol(pattern::SetFPSLimit), SetFPSLimit, &real::SetFPSLimit);

        auto& options = game::OptionsManager::get();
        options.addMultiChoiceOption("qol", "Performance", "osgt_qol_fps_mode",
                                     "Max Framerate (may be capped by your device)", g_fpsOptions, &OnFPSChangeCallback,
                                     80.0f);
        options.addCheckboxOption("qol", "Performance", "osgt_qol_fps_show", "Show the FPS counter", &ToggleFPSCounter);
    }

    static void OnFPSChangeCallback(VariantList* pVariant)
    {
        // Update the weather index
        Entity* pClickedEnt = pVariant->Get(1).GetEntity();
        Variant* pOptVar = real::GetApp()->GetVar("osgt_qol_fps_mode");
        uint32_t idx = pOptVar->GetUINT32();
        if (pClickedEnt->GetName() == "back")
        {
            if (idx == 0)
                idx = (uint32_t)g_fpsOptions.size() - 1;
            else
                idx--;
        }
        else if (pClickedEnt->GetName() == "next")
        {
            if (idx >= g_fpsOptions.size() - 1)
                idx = 0;
            else
                idx++;
        }
        pOptVar->Set(idx);
        // Update the option label
        Entity* pTextLabel = pClickedEnt->GetParent()->GetEntityByName("txt");
        real::SetTextEntity(pTextLabel, g_fpsOptions[idx]);
        SetFPSLimit(nullptr, 60.0f);
    }

    static void ToggleFPSCounter(VariantList* pVariant)
    {
        Entity* pCheckbox = pVariant->Get(1).GetEntity();
        bool bChecked = pCheckbox->GetVar("checked")->GetUINT32() != 0;
        real::GetApp()->GetVar("osgt_qol_fps_show")->Set(uint32_t(bChecked));
        SetFPSLimit(nullptr, 60.0f);
    }

    static void SetFPSLimit(void* app, float fps)
    {
        real::GetApp()->m_bShowFPS = real::GetApp()->GetVar("osgt_qol_fps_show")->GetUINT32() == 1;
        Variant* pVar = real::GetApp()->GetVar("osgt_qol_fps_mode");
        if (pVar->GetType() == Variant::TYPE_UNUSED)
            pVar->Set(1U); // Default to 90 fps.
        real::SetFPSLimit(app, g_fpsValues[real::GetApp()->GetVar("osgt_qol_fps_mode")->GetUINT32()]);
    }
};
REGISTER_USER_GAME_PATCH(FramerateUnlockPatch, framerate_unlock);
