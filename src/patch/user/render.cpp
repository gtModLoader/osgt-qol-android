#include "game/game.hpp"
#include "game/signatures.hpp"
#include "game/struct/components/mapbg.hpp"
#include "game/struct/entity.hpp"
#include "patch/patch.hpp"
#include "version.h"

REGISTER_GAME_FUNCTION(MainMenuCreate, "_Z14MainMenuCreateP6Entityb", void, Entity*, bool);
class CustomizedTitleScreen : public patch::BasePatch
{
  public:
    void apply() const override
    {
        auto& game = game::GameHarness::get();
        game.hookFunction<MainMenuCreate_t>(game.resolveSymbol(pattern::MainMenuCreate), MainMenuCreate,
                                            &real::MainMenuCreate);
    }

    static void MainMenuCreate(Entity* pEnt, bool unk2)
    {
        // Let the game construct main menu for us.
        real::MainMenuCreate(pEnt, unk2);
        // After which we can change the weather without issue.
        ChangeMainMenuWeather(pEnt);

        // Lets also add modloader version label to the menuscreen to the opposite of version
        Entity* pVerLabel = pEnt->GetEntityByName("MainMenu")->GetEntityByName("version");
        CL_Vec2f m_verLabelPos = pVerLabel->GetVar("pos2d")->GetVector2();
        CL_Vec2f m_verLabelSize = pVerLabel->GetVar("size2d")->GetVector2();

        std::string versionText = "`wOSGT-QOL-ANDROID " OSGT_QOL_DISPLAY_VERSION "``";
        Entity* pTextLabel =
            real::CreateTextLabelEntity(pEnt->GetEntityByName("MainMenu"), UbiString("mltxt"), real::iPhoneMapX(5.0f),
                                        m_verLabelPos.y - m_verLabelSize.y, UbiString(versionText));

        // Retrieve fontscale and scale created entity
        uint32_t fontID;
        float fontScale;
        real::GetFontAndScaleToFitThisLinesPerScreenY(fontID, fontScale, 20);
        real::SetupTextEntity(pTextLabel, fontID, fontScale);
        // Fade in at approximately same time as version text
        real::FadeInEntity(pTextLabel, true, 600, 400, 1.0, true);
    }

    static void ChangeMainMenuWeather(Entity* pGUIEnt)
    {
        // Lets retrieve MapBGComponent from GUI -> MainMenu
        MapBGComponent* pMapBGComponent = reinterpret_cast<MapBGComponent*>(
            pGUIEnt->GetEntityByName("MainMenu")->GetComponentByName("MapBGComponent"));
        Background* pBG = pMapBGComponent->m_pBackground;
        *(bool*)(((int64_t)pBG) + 0x9c) = true;
    }
};
REGISTER_USER_GAME_PATCH(CustomizedTitleScreen, customized_title_screen);
