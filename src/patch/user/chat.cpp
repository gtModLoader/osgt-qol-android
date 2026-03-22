#include "game/game.hpp"
#include "game/signatures.hpp"
#include "game/struct/entity.hpp"
#include "patch/patch.hpp"
#include "utils/utils.hpp"
#include <cstdint>
#include <string>

REGISTER_GAME_FUNCTION(TabComponentAddTabButton,
                       "_ZN12TabComponent12AddTabButtonEP6EntityR7CL_Vec2IfERKS3_RKSsRK14TabOverlayData", int64_t,
                       void*, void*, float*, void*, void*, void*);
REGISTER_GAME_FUNCTION(LogToConsoleSafe, "_Z16LogToConsoleSafeSs", void, std::string const&);
REGISTER_GAME_FUNCTION(GetChatSettings, "_Z14GetChatSettingRSs", void, std::string const&);
REGISTER_GAME_FUNCTION(UpdateLogTextOffset, "_Z19UpdateLogTextOffsetf", void, float);
class LegacyChatPatch : public patch::BasePatch
{
  public:
    void apply() const override
    {
        auto& game = game::GameHarness::get();
        game.hookFunction<TabComponentAddTabButton_t>(game.resolveSymbol(pattern::TabComponentAddTabButton),
                                                      TabComponentAddTabButton, &real::TabComponentAddTabButton);
        game.hookFunction<LogToConsoleSafe_t>(game.resolveSymbol(pattern::LogToConsoleSafe), LogToConsoleSafe,
                                              &real::LogToConsoleSafe);

        // Patch out the [S] chat indicators when user enables the mod using this address as an anchor.
        RESOLVE_SYMBOL(GetChatSettings);
        m_systemMsgPtr = utils::resolveAdrpAddToAddress<char*>((void*)(((int64_t)real::GetChatSettings) + 308));

        // Used in Patch/Unpatch helper, more comments there.
        RESOLVE_SYMBOL(UpdateLogTextOffset);

        auto& optionsMgr = game::OptionsManager::get();
        optionsMgr.addCheckboxOption("qol", "UI", "osgt_qol_legacy_chat_kill", "Re-enable chat tabs", &ToggleChatTabs);
    }

    static void ToggleChatTabs(VariantList* pVariant)
    {
        Entity* pCheckbox = pVariant->Get(1).GetEntity();
        bool bChecked = pCheckbox->GetVar("checked")->GetUINT32() != 0;
        real::GetApp()->GetVar("osgt_qol_legacy_chat_kill")->Set(uint32_t(bChecked));
        m_bUsingTabs = bChecked;
        PatchOrUnpatchTabs(!m_bUsingTabs);
    }

    static void PatchOrUnpatchTabs(bool bPatch)
    {
        if (bPatch)
        {
            // Null-terminate the string so it won't get prepended at all.
            utils::fillMemory(m_systemMsgPtr, 1, 0);
            // Overwrite a b.le (jump if less than or equal) instruction to b (unconditional jump) when game checks for
            // TabComponentin UpdateLogTextOffset. Since we killed off tabs from showing, we'll also get rid of the
            // margin tabs add.
            utils::writeMemoryPattern((void*)(((int64_t)real::UpdateLogTextOffset) + 436), "08 00 00 14");
        }
        else
        {
            // Restore the ` character.
            utils::fillMemory(m_systemMsgPtr, 1, 0x60);
            // Restore the b.le instruction that was there originally.
            utils::writeMemoryPattern((void*)(((int64_t)real::UpdateLogTextOffset) + 436), "0D 01 00 54");
        }
    }

    static void LogToConsoleSafe(std::string const& text)
    {
        if (!m_bLoadVar)
        {
            m_bLoadVar = true;
            m_bUsingTabs = real::GetApp()->GetVar("osgt_qol_legacy_chat_kill")->GetUINT32() == 1;
            PatchOrUnpatchTabs(!m_bUsingTabs);
        }
        if (m_bUsingTabs)
        {
            real::LogToConsoleSafe(text);
            return;
        }
        // Re-implement pre-chat update LogToConsoleSafe function.
        // LogDisplay Component is located in Root -> ConsoleLogParent -> LogRectBG -> LogScroller
        // -> scroll_child -> LogDisplayEntity -> LogDisplay
        // nit: This is technically more performant than original since our Entity funct doesn't do
        // recursive search, but this is ugly at the same time.
        Entity* pEntityRoot = real::GetApp()->m_pEntityRoot;
        Entity* pLogParent = pEntityRoot->GetEntityByName("ConsoleLogParent");
        if (pLogParent == nullptr)
            return;
        Entity* pLogRectBG = pLogParent->GetEntityByName("LogRectBG");
        if (pLogRectBG == nullptr)
            return;
        Entity* pLogScroller = pLogRectBG->GetEntityByName("LogScroller");
        if (pLogScroller == nullptr)
            return;
        Entity* pScrollChild = pLogScroller->GetEntityByName("scroll_child");
        if (pScrollChild == nullptr)
            return;
        Entity* pLogDisplayEnt = pScrollChild->GetEntityByName("LogDisplayEntity");
        if (pLogDisplayEnt == nullptr)
            return;
        EntityComponent* pLogDisplayComp = pLogDisplayEnt->GetComponentByName("LogDisplay");
        if (pLogDisplayComp == nullptr)
            return;
        // todo: Replace CT:[W]_ stuff.
        std::string line = "`o" + text + "``";
        VariantList vl;
        vl.Get(0).Set(line);
        pLogDisplayComp->GetShared()->CallFunctionIfExists("AddLine", &vl);
    }

    static int64_t TabComponentAddTabButton(void* this_, void* entity, float* xCoord, void* unk4, void* unk5,
                                            void* unk6)
    {
        // Tabs pass on xCoord parameter to continiously align themselves after one another.
        // We can force this to always be off-screen, essentially removing them from visible
        // game area.
        if (!m_bUsingTabs)
            *xCoord = -500.0f;
        return real::TabComponentAddTabButton(this_, entity, xCoord, unk4, unk5, unk6);
    }

  private:
    static bool m_bLoadVar;
    static bool m_bUsingTabs;
    static char* m_systemMsgPtr;
};
REGISTER_USER_GAME_PATCH(LegacyChatPatch, enable_legacy_chat);
bool LegacyChatPatch::m_bLoadVar = false;
bool LegacyChatPatch::m_bUsingTabs = false;
char* LegacyChatPatch::m_systemMsgPtr = 0;