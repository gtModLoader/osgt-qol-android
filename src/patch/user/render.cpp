#include "game/game.hpp"
#include "game/signatures.hpp"
#include "game/struct/components/mapbg.hpp"
#include "game/struct/entity.hpp"
#include "game/struct/graphics/background_blood.hpp"
#include "game/struct/graphics/background_default.hpp"
#include "game/struct/renderutils.hpp"
#include "game/struct/rtrect.hpp"
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
REGISTER_GAME_FUNCTION(DrawFilledBitmapRect, "_Z20DrawFilledBitmapRectR6rtRectjjP11SurfaceAnimb", void, rtRectf&,
                       uint32_t, uint32_t, void*, bool);

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
        // Any custom weathers
        auto& weatherMgr = game::WeatherManager::get();
        for (auto it = weatherMgr.weathers.begin(); it != weatherMgr.weathers.end(); ++it)
        {
            // Signify custom weathers with golden colour
            displayNames.push_back("`6" + it->first);
        }
        weatherMgr.m_sig_eventSubscribe.connect(&customWeatherEvent);
        // Register the Multi-Choice option
        optionsMgr.addMultiChoiceOption("qol", "UI", "osgt_qol_title_bg", "Title Screen Background", displayNames,
                                        &TitleBackgroundOnSelect, 80.0f);
    }

    static void customWeatherEvent(game::WeatherManager::CustomWeatherEvent* pCustomWeather)
    {
        // Signify custom weathers with golden colour
        displayNames.push_back("`6" + pCustomWeather->m_prettyName);
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

        uint32_t weatherIdx = real::GetApp()->GetVar("osgt_qol_title_bg")->GetUINT32();
        Background* pNewBG;
        if (weatherIdx > vanillaWeatherBound)
        {
            // Get custom weather from weather manager - if we don't have it, most likely the mod
            // has been uninstalled or save.dat is manually tampered.
            auto& weatherMgr = game::WeatherManager::get();
            auto pair = weatherMgr.weathers.find(real::GetApp()->GetVar("osgt_qol_title_bg_modded")->GetString());
            if (pair == weatherMgr.weathers.end())
            {
                pNewBG = new Background_Default();
                real::GetApp()->GetVar("osgt_qol_title_bg_modded")->Reset();
                real::GetApp()->GetVar("osgt_qol_title_bg")->Set(0U);
            }
            else
            {
                pNewBG = pair->second.callback();
            }
        }
        else
        {
            switch (weatherIdx)
            {
            case 0:
            {
                pNewBG = new Background_Default();
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
                pNewBG = new Background_Default();
                ((Background_Default*)pNewBG)->m_springInstead = true;
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
                pNewBG = new Background_Default();
                LOG_DEBUG("Unhandled weather %d", weatherIdx);
                break;
            }
        }

        if (pNewBG != nullptr)
        {
            // Initialize the weather with bInWorld as false.
            pNewBG->Init(false);
            // Do an update as well before anything renders it. This is needed in rare cases such as
            // Background_Treasure where one specific variable doesn't have an initial value, so it
            // gets trashed and causes a game crash.
            pNewBG->Update();
            // Save our old one to delete in a brief second.
            Background* pOriginalWeather = (Background*)pMapBGComponent->m_pBackground;
            // Assign our new one in place.
            pMapBGComponent->m_pBackground = pNewBG;
            // Discard the original.
            delete pOriginalWeather;
        }
    }
};
REGISTER_USER_GAME_PATCH(CustomizedTitleScreen, customized_title_screen);

class BubbleOpacityBackport : public patch::BasePatch
{
  public:
    void apply() const override
    {
        auto& game = game::GameHarness::get();

        // Add the slider back into options. Normally it's between the music/sfx/gfx sliders, but
        // it'd be too messy to ram it between them. OptionsManager will move it to dedicated
        // "OSGT-QOL Options" area.
        auto& optionsMgr = game::OptionsManager::get();
        optionsMgr.addSliderOption("qol", "UI", "speech_bubble_opacity", "Bubble Opacity",
                                   &BubbleOpacitySliderCallback);

        // Hook
        game.hookFunction<DrawFilledBitmapRect_t>(game.resolveSymbol(pattern::DrawFilledBitmapRect),
                                                  DrawFilledBitmapRect, &real::DrawFilledBitmapRect);
    }

    static void BubbleOpacitySliderCallback(Variant* pVariant)
    {
        real::GetApp()->GetVar("speech_bubble_opacity")->Set(pVariant->GetFloat());
    }

    static void DrawFilledBitmapRect(rtRectf& r, uint32_t middleColor, uint32_t borderColor, void* pSurf,
                                     bool bFillMiddleCloserToEdges)
    {
        // Bubble opacity is a vanilla feature, albeit from future version, it should go in save.dat
        Variant* pVariant = real::GetApp()->GetVar("speech_bubble_opacity");
        if (pVariant->GetType() != Variant::TYPE_FLOAT)
            pVariant->Set(1.00f);
        float opacity = pVariant->GetFloat();
        middleColor = ModAlpha(middleColor, (((float)GET_ALPHA(middleColor)) / 255.0f) * opacity);
        borderColor = ModAlpha(borderColor, (((float)GET_ALPHA(borderColor)) / 255.0f) * opacity);
        real::DrawFilledBitmapRect(r, middleColor, borderColor, pSurf, bFillMiddleCloserToEdges);
    }
};
REGISTER_USER_GAME_PATCH(BubbleOpacityBackport, bubble_opacity_backport);

class BloodMoonDemoWeather : public patch::BasePatch
{
  public:
    void apply() const override
    {
        // This is a demo mod showcasing how to register a custom weather.
        // See src/game/struct/graphics/backgound_blood.cpp for the weather implementation.
        auto& weatherMgr = game::WeatherManager::get();
        weatherMgr.registerWeather("blood_moon", &onWeatherCreate);
    }

    static Background* onWeatherCreate() { return new Background_Blood(); }
};
REGISTER_USER_GAME_PATCH(BloodMoonDemoWeather, blood_moon_demo_weather);