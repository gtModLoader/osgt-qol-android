#include "game/game.hpp"
#include "game/signatures.hpp"
#include "game/struct/entity.hpp"
#include "patch/patch.hpp"
#include <string>
#include <version.h>

REGISTER_GAME_FUNCTION(AboutMenuAddScrollContent, "_Z15AboutMenuCreateP6Entity", Entity*, Entity*);
class AboutMenuAttribution : public patch::BasePatch
{
  public:
    void apply() const override
    {
        auto& game = game::GameHarness::get();
        // Hook AboutMenuAddScrollContent.
        game.hookFunction<AboutMenuAddScrollContent_t>(game.resolveSymbol(pattern::AboutMenuAddScrollContent),
                                                       AboutMenuAddScrollContent, &real::AboutMenuAddScrollContent);
    }

    static Entity* AboutMenuAddScrollContent(Entity* pGUI)
    {
        // V4.03 passes on GUI entity, V3.02 passed on something else, strange.
        // Let about menu construct itself normally.
        Entity* pAboutMenu = real::AboutMenuAddScrollContent(pGUI);
        Entity* pScrollChild = pAboutMenu->GetEntityByName("scroll")->GetEntityByName("scroll_child");

        // We take over TextBox2 (the first Entity under children) and insert our own
        // attribution logic to it.
        Entity* pTextBox2 = pScrollChild->GetChildren()->front();
        EntityComponent* pTextComponent = pTextBox2->GetComponentByName("TextBoxRender");
        if (pTextComponent != nullptr)
        {
            Variant* pTextVariant = pTextComponent->GetShared()->GetVarIfExists("text");
            if (pTextVariant != nullptr)
            {
                // Set our attribution.
                real::SetTextEntity(pTextBox2, pTextVariant->GetString() + GetAttributionText());
                // Signal proton to resize the menu for our text to be visible.
                VariantList vl(pScrollChild->GetParent()->GetParent());
                real::ResizeScrollBounds(&vl);
            }
        }
        return pAboutMenu;
    }

    // Returns the attribution text to be appended to the about menu. Also includes a list of
    // applied user patches.
    static std::string GetAttributionText()
    {
        auto patches = patch::PatchManager::get().getAppliedUserPatchList();

        std::string firstLine = "\nOSGT-QOL-ANDROID " OSGT_QOL_DISPLAY_VERSION;
#ifdef OSGT_QOL_DEVELOPMENT
        firstLine += " (Development Build)\n";
#else
        firstLine += "\n";
#endif

        std::string text =
            firstLine +
            "`6Original PC mod created by `wCernodile`` and `whouz``. Ported to Android by `wCernodile``."
            "\nThere are currently `w" +
            toString(patches.size()) + "`` patches applied: ";

        // Create comma-separated list of patches.
        for (size_t i = 0; i < patches.size(); i++)
        {
            text += "`w" + patches[i] + "``";
            if (i < patches.size() - 1)
                text += ", ";
        }
        return text;
    }
};
REGISTER_CORE_GAME_PATCH(AboutMenuAttribution, about_menu_attrib);