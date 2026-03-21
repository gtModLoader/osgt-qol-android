#include "game/game.hpp"
#include "game/signatures.hpp"
#include "game/struct/components/mapbg.hpp"
#include "game/struct/entity.hpp"
#include "patch/patch.hpp"
#include "version.h"

REGISTER_GAME_FUNCTION(MainMenuCreate, "_Z14MainMenuCreateP6Entityb", void, Entity*, bool);

// Weather ctors.
REGISTER_GAME_FUNCTION(BackgroundSunset, "_ZN17Background_SunsetC2Ev", Background*, void*);
REGISTER_GAME_FUNCTION(BackgroundNight, "_ZN16Background_NightC2Ei", Background*, void*, int);
REGISTER_GAME_FUNCTION(BackgroundDesert, "_ZN17Background_DesertC2Ev", Background*, void*);
REGISTER_GAME_FUNCTION(BackgroundHarvest, "_ZN18Background_HarvestC2Ev", Background*, void*);
REGISTER_GAME_FUNCTION(BackgroundMars, "_ZN15Background_MarsC2Ev", Background*, void*);
REGISTER_GAME_FUNCTION(BackgroundBlank, "_ZN16Background_BlankC2Ev", Background*, void*);
REGISTER_GAME_FUNCTION(BackgroundUndersea, "_ZN19Background_UnderseaC2Ev", Background*, void*);
REGISTER_GAME_FUNCTION(BackgroundWarp, "_ZN15Background_WarpC2Ei", Background*, void*, int);
REGISTER_GAME_FUNCTION(BackgroundWolf, "_ZN15Background_WolfC2Ev", Background*, void*);
REGISTER_GAME_FUNCTION(BackgroundPagoda, "_ZN17Background_PagodaC2Ei", Background*, void*);
REGISTER_GAME_FUNCTION(BackgroundMonochrome, "_ZN21Background_MonochromeC2Ev", Background*, void*);
REGISTER_GAME_FUNCTION(BackgroundTreasure, "_ZN19Background_TreasureC2Ev", Background*, void*);
REGISTER_GAME_FUNCTION(BackgroundSurgery, "_ZN18Background_SurgeryC2Ev", Background*, void*);
REGISTER_GAME_FUNCTION(BackgroundBountiful, "_ZN20Background_BountifulC2Ev", Background*, void*);

static std::vector<std::string> displayNames;
static uint32_t vanillaWeatherBound = 16;
class CustomizedTitleScreen : public patch::BasePatch
{
  public:
    void apply() const override
    {
        auto& game = game::GameHarness::get();

        // Resolve functions
        RESOLVE_SYMBOL(BackgroundSunset);
        RESOLVE_SYMBOL(BackgroundNight);
        RESOLVE_SYMBOL(BackgroundDesert);
        RESOLVE_SYMBOL(BackgroundHarvest);
        RESOLVE_SYMBOL(BackgroundMars);
        RESOLVE_SYMBOL(BackgroundBlank);
        RESOLVE_SYMBOL(BackgroundUndersea);
        RESOLVE_SYMBOL(BackgroundWarp);
        RESOLVE_SYMBOL(BackgroundWolf);
        RESOLVE_SYMBOL(BackgroundPagoda);
        RESOLVE_SYMBOL(BackgroundMonochrome);
        RESOLVE_SYMBOL(BackgroundTreasure);
        RESOLVE_SYMBOL(BackgroundSurgery);
        RESOLVE_SYMBOL(BackgroundBountiful);

        game.hookFunction<MainMenuCreate_t>(game.resolveSymbol(pattern::MainMenuCreate), MainMenuCreate,
                                            &real::MainMenuCreate);

        // We will allow the end-user to change their title screen weather preference
        auto& optionsMgr = game::OptionsManager::get();
        // Populate our options
        displayNames = {"Sunny",       "Sunset",     "Night",      "Desert",       "Harvest",  "Mars",
                        "Nothingness", "Undersea",   "Warp Speed", "Comet (Blue)", "Spring",   "Howling",
                        "Pagoda",      "Monochrome", "Treasure",   "SurgWorld",    "Bountiful"};

        // Register the Multi-Choice option
        optionsMgr.addMultiChoiceOption("qol", "UI", "osgt_qol_title_bg", "Title Screen Background", displayNames,
                                        &TitleBackgroundOnSelect, 80.0f);
    }

    static void TitleBackgroundOnSelect(VariantList* pVariant)
    {
        // Update the weather index
        Entity* pClickedEnt = pVariant->Get(1).GetEntity();
        Variant* pOptVar = real::GetApp()->GetVar("osgt_qol_title_bg");
        uint32_t idx = pOptVar->GetUINT32();
        if (pClickedEnt->GetName() == "back")
        {
            if (idx == 0)
                idx = (uint32_t)displayNames.size() - 1;
            else
                idx--;
        }
        else if (pClickedEnt->GetName() == "next")
        {
            if (idx >= displayNames.size() - 1)
                idx = 0;
            else
                idx++;
        }
        pOptVar->Set(idx);
        if (idx > vanillaWeatherBound)
        {
            // Custom weather, save the pretty name. These may shift around, but won't affect any
            // already applied weathers on subsequent boots.
            real::GetApp()->GetVar("osgt_qol_title_bg_modded")->Set(displayNames[idx].substr(2));
        }
        else
        {
            real::GetApp()->GetVar("osgt_qol_title_bg_modded")->Reset();
        }
        // Update the option label
        Entity* pTextLabel = pClickedEnt->GetParent()->GetEntityByName("txt");
        real::SetTextEntity(pTextLabel, displayNames[idx]);
        Entity* pGUIEnt = real::GetApp()->m_pEntityRoot->GetEntityByName("GUI");
        if (pGUIEnt->GetEntityByName("MainMenu"))
            ChangeMainMenuWeather(pGUIEnt);
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
            real::CreateTextLabelEntity(pEnt->GetEntityByName("MainMenu"), "mltxt", real::iPhoneMapX(5.0f),
                                        m_verLabelPos.y - m_verLabelSize.y, versionText);

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
        // Are we launching OSGT-QOL for the first time? If so, set a default other than Sunny.
        Variant* pVariant = real::GetApp()->GetVar("osgt_qol_title_bg");
        if (pVariant->GetType() == Variant::TYPE_UNUSED)
            pVariant->Set(10U);
        // Lets retrieve MapBGComponent from GUI -> MainMenu
        MapBGComponent* pMapBGComponent = reinterpret_cast<MapBGComponent*>(
            pGUIEnt->GetEntityByName("MainMenu")->GetComponentByName("MapBGComponent"));
        Background* pBG = pMapBGComponent->m_pBackground;

        Background* pNewBG;
        uint32_t weatherIdx = real::GetApp()->GetVar("osgt_qol_title_bg")->GetUINT32();
        switch (weatherIdx)
        {
        case 0:
        {
            pNewBG = pBG;
            break;
        }
        case 1:
        {
            // For the weathers we don't have a reversed class for, create a raw buffer.
            // In case of Background_Sunset, it should be 0x1A8 in size.
            pNewBG = (Background*)operator new(0x1A8);
            real::BackgroundSunset(pNewBG);
            break;
        }
        case 2:
        {
            // Background_Night takes an additional value in constructor, namely the weather ID
            // which dictates if it should render it as normal night or a comet weather.
            pNewBG = (Background*)operator new(0x258);
            real::BackgroundNight(pNewBG, 2);
            break;
        }
        case 3:
        {
            // Desert / Arid
            pNewBG = (Background*)operator new(0x1B8);
            real::BackgroundDesert(pNewBG);
            break;
        }
        case 4:
        {
            // Harvest Blast
            pNewBG = (Background*)operator new(0x1E0);
            real::BackgroundHarvest(pNewBG);
            break;
        }
        case 5:
        {
            // Mars
            pNewBG = (Background*)operator new(0x1D0);
            real::BackgroundMars(pNewBG);
            break;
        }
        case 6:
        {
            // Blank (Nothingness)
            pNewBG = (Background*)operator new(0xA0);
            real::BackgroundBlank(pNewBG);
            break;
        }
        case 7:
        {
            // Undersea
            pNewBG = (Background*)operator new(0x5F8);
            real::BackgroundUndersea(pNewBG);
            break;
        }
        case 8:
        {
            // Warp Speed
            // 15 for Warp Speed, anything else for Stargazing
            pNewBG = (Background*)operator new(0x168);
            real::BackgroundWarp(pNewBG, 15);
            break;
        }
        case 9:
        {
            // Comet (Blue/Weather)
            pNewBG = (Background*)operator new(0x258);
            real::BackgroundNight(pNewBG, 17);
            break;
        }
        case 10:
        {
            // Spring
            pNewBG = pBG;
            *(bool*)(((int64_t)pBG) + 0x9c) = true;
            break;
        }
        case 11:
        {
            // Wolf
            pNewBG = (Background*)operator new(0x248);
            real::BackgroundWolf(pNewBG);
            break;
        }
        case 12:
        {
            // Pagoda
            pNewBG = (Background*)operator new(0x1D0);
            real::BackgroundPagoda(pNewBG);
            break;
        }
        case 13:
        {
            // Monochrome
            pNewBG = (Background*)operator new(0x110);
            real::BackgroundMonochrome(pNewBG);
            break;
        }
        case 14:
        {
            // Treasure
            pNewBG = (Background*)operator new(0x1A0);
            real::BackgroundTreasure(pNewBG);
            break;
        }
        case 15:
        {
            // Surgery
            pNewBG = (Background*)operator new(0xF0);
            real::BackgroundSurgery(pNewBG);
            break;
        }
        case 16:
        {
            // Bountiful
            pNewBG = (Background*)operator new(0x110);
            real::BackgroundBountiful(pNewBG);
            break;
        }
        default:
            pNewBG = pBG;
            LOG_DEBUG("Unhandled weather %d", weatherIdx);
            break;
        }

        if (pNewBG != nullptr && pBG != pNewBG)
        {
            // Initialize the weather with bInWorld as false.
            pNewBG->Init(false);
            // Do an update as well before anything renders it. This is needed in rare cases such as
            // Background_Treasure where one specific variable doesn't have an initial value, so it
            // gets trashed and causes a game crash.
            pNewBG->Update();
            // Save our old one to delete in a brief second.
            Background* pOriginalWeather = pBG;
            // Assign our new one in place.
            pMapBGComponent->m_pBackground = pNewBG;
            // Discard the original.
            delete pOriginalWeather;
        }
    }
};
REGISTER_USER_GAME_PATCH(CustomizedTitleScreen, customized_title_screen);
