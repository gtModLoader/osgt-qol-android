#include "game/game.hpp"
#include "game/signatures.hpp"
#include "game/struct/component.hpp"
#include "game/struct/components/gamelogic.hpp"
#include "game/struct/components/mapbg.hpp"
#include "game/struct/entity.hpp"
#include "game/struct/graphics/background_blood.hpp"
#include "game/struct/graphics/background_default.hpp"
#include "game/struct/miscutils.hpp"
#include "game/struct/netavatar/netavatar.hpp"
#include "game/struct/renderutils.hpp"
#include "game/struct/rtrect.hpp"
#include "game/struct/world/world.hpp"
#include "game/struct/world/worldcamera.hpp"

#include "patch/patch.hpp"
#include "version.h"

#include <fstream>
#include <iostream>

#include <lz-string.hpp>

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
REGISTER_GAME_FUNCTION(AvatarDataGetSetAsUint16, "_ZN10AvatarData22GetClothingSetAsUint16EPt", void, void*, uint16_t*);
REGISTER_GAME_FUNCTION(WorldRendererTileLineOfSight, "_ZN13WorldRenderer15TileLineOfSightEP4TileS1_fPf", bool, void*,
                       Tile*, Tile*, float, float*);
REGISTER_GAME_FUNCTION(WorldRendererDrawTiles, "_ZN13WorldRenderer9DrawTilesERSt6vectorIP4TileSaIS2_EEi", void, void*,
                       std::vector<Tile*>*, int);
REGISTER_GAME_FUNCTION(WorldCameraOnUpdate, "_ZN11WorldCamera8OnUpdateE7CL_Vec2IfES1_", void, WorldCamera*, CL_Vec2f*,
                       CL_Vec2f*);
REGISTER_GAME_FUNCTION(WorldCameraGetCamWorldPos, "_ZN11WorldCamera14GetCamWorldPosEv", CL_Vec2f*, WorldCamera*);
REGISTER_GAME_FUNCTION(AddTool, "_Z7AddTooliP6Entity", void, int, Entity*);
REGISTER_GAME_FUNCTION(InventoryMenuCreate, "_Z19InventoryMenuCreateP6Entity", void, Entity*);
REGISTER_GAME_FUNCTION(UpdateTouchControlPositions, "_Z27UpdateTouchControlPositionsv", void);
REGISTER_GAME_FUNCTION(TradeMenuOnInventoryMoved, "_ZN9TradeMenu16OnInventoryMovedEv", void, void*);
REGISTER_GAME_FUNCTION(GameLogicComponentGetQuickToolInSlot, "_ZN18GameLogicComponent18GetQuickToolInSlotEi", int,
                       GameLogicComponent*, int);
REGISTER_GAME_FUNCTION(PlayerItemsSetQuickSlotItem, "_ZN11PlayerItems16SetQuickSlotItemEii", void, void*, int, short);
REGISTER_GAME_FUNCTION(PlayerItemsUpdateQuickSlotsWithUsedItem,
                       "_ZN11PlayerItems28UpdateQuickSlotsWithUsedItemE7eItemID", void, PlayerItems*, int);
REGISTER_GAME_FUNCTION(PlayerItemsRemoveFromQuickSlots, "_ZN11PlayerItems20RemoveFromQuickSlotsEi", void, PlayerItems*,
                       int);
REGISTER_GAME_FUNCTION(PlayerItemsFillBlankQuickToolSlotsWithStuff,
                       "_ZN11PlayerItems32FillBlankQuickToolSlotsWithStuffEv", void, PlayerItems*);
REGISTER_GAME_FUNCTION(WorldRendererDrawBackgroundTiles,
                       "_ZN13WorldRenderer19DrawBackgroundTilesERSt6vectorIP4TileSaIS2_EE", void, WorldRenderer*,
                       std::vector<Tile*>*);
REGISTER_GAME_FUNCTION(WorldRendererDrawWater, "_ZN13WorldRenderer9DrawWaterERSt6vectorIP4TileSaIS2_EE", void,
                       WorldRenderer*, std::vector<Tile*>*);
REGISTER_GAME_FUNCTION(WorldTileMapGetTileSafe, "_ZN12WorldTileMap11GetTileSafeEii", Tile*, WorldTileMap*, int, int);
REGISTER_GAME_FUNCTION(WorldTileMapChooseVisual, "_ZN12WorldTileMap12ChooseVisualEP4Tile", void, WorldTileMap*, Tile*);
REGISTER_GAME_FUNCTION(WorldTileMapChooseVisual_Flag, "_ZN12WorldTileMap17ChooseVisual_FlagEiii", int, WorldTileMap*,
                       int, int, int);
REGISTER_GAME_FUNCTION(WorldTileMapChooseVisual_SmartEdge,
                       "_ZN12WorldTileMap22ChooseVisual_SmartEdgeEP4Tile17eChooseVisualType", int, WorldTileMap*, Tile*,
                       int);
REGISTER_GAME_FUNCTION(DrawTile, "_ZN13WorldRenderer8DrawTileEti7CL_Vec2IfEjP4Tilebb", void, WorldRenderer* param_1,
                       unsigned short param_2, int param_3, CL_Vec2f* param_4, unsigned int param_5, Tile* param_6,
                       uint8_t param_7, char param_8);
REGISTER_GAME_FUNCTION(TextManagerAddTextByTile,
                       "_ZN11TextManager13AddTextByTileEP4TileRKSsbbN8TextItem13eTimingMethodEb", TextObject*, void*,
                       Tile* pTile, std::string msg, bool bPushToFront, bool bOnlyAddIfTileBlank, int timing,
                       bool bIsBillboard);
REGISTER_GAME_FUNCTION(NetAvatarProcessTileWeAreDirectlyOver, "_ZN9NetAvatar28ProcessTileWeAreDirectlyOverEv", void,
                       void*);
REGISTER_GAME_GLOBAL_VAR(g_fireBatcher, "g_fireBatcher", void*);
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

class DaVinciFakeID : public patch::BasePatch
{
  public:
    void apply() const override
    {
        // Modifies avatars to portray old Da Vinci Wings ID as new one.
        // Cosmetic issue with this approach is that it shows as unequipped in inventory.
        auto& game = game::GameHarness::get();
        game.hookFunctionPattern<AvatarDataGetSetAsUint16_t>(pattern::AvatarDataGetSetAsUint16,
                                                             AvatarDataGetSetAsUint16, &real::AvatarDataGetSetAsUint16);
    }

    static void AvatarDataGetSetAsUint16(void* this_, uint16_t* set)
    {
        real::AvatarDataGetSetAsUint16(this_, set);
        if (set[6] == 3308)
            set[6] = 8286;
    }
};
REGISTER_USER_GAME_PATCH(DaVinciFakeID, fake_davinci_id);

static bool g_bLightSourcesOptimized = true;
static int g_fLightSourceOptimizeLevel = 5;
class LightSourceOptimizer : public patch::BasePatch
{
  public:
    void apply() const override
    {
        // The way Growtopia calculates where light should spread is very inefficient - for every
        // light source, it will scan the entire tilemap in your viewport. That is rather expensive
        // with a ton of light emitters on a large zoom (such as mod zoom).
        //
        // This mod partially rewrites that logic by instead doing a maximum 5x5 rectangular search
        // for darkened tiles and applies light calculation to them only.
        //
        // From the PC version, performance metrics:
        // On a machine using Ryzen 7 8845HS with Radeon 780M - the performance difference on a
        // 100x56 area full of Chandelier blocks. For control to see efficiency of the light
        // calculations, a reference world with same parameters was created, but instead of
        // Chandelier (a light emitter, animated block), it was filled with Party Cacti (non-light
        // emitter, animated block).
        //
        // Unpatched: 10-11 FPS
        // Patched: 158-172 FPS
        // Reference world: 168-178 FPS
        // Frame cap: 200 FPS (240 Hz)

        auto& game = game::GameHarness::get();

        game.hookFunctionPattern<WorldRendererDrawTiles_t>(pattern::WorldRendererDrawTiles, WorldRendererDrawTiles,
                                                           &real::WorldRendererDrawTiles);

        RESOLVE_SYMBOL(WorldRendererTileLineOfSight);

        auto& optionsMgr = game::OptionsManager::get();
        optionsMgr.addCheckboxOption("qol", "Performance", "osgt_qol_lightopt_enabled", "Optimize Light Emitters",
                                     &LightOptimizerToggle);
        optionsMgr.addSliderOption("qol", "Performance", "osgt_qol_lightopt_rad", "Light Emitter Strength",
                                   &LightOptimizerSlider, "`5(See more at expense of fps)``");

        auto& events = game::EventsAPI::get();
        events.m_sig_postInit.connect(&applyPostInit);
    }

    static void applyPostInit()
    {
        Variant* pVariant = real::GetApp()->GetVar("osgt_qol_lightopt_rad");
        if (pVariant->GetType() != Variant::TYPE_FLOAT)
            pVariant->Set(1.00f);
        else
            g_fLightSourceOptimizeLevel = (int)(pVariant->GetFloat() * 5.0f);

        pVariant = real::GetApp()->GetVar("osgt_qol_lightopt_enabled");
        if (pVariant->GetType() != Variant::TYPE_UINT32)
            pVariant->Set(uint32_t(1));
        else
            g_bLightSourcesOptimized = pVariant->GetUINT32() == 1;
    }

    static void LightOptimizerSlider(Variant* pVariant)
    {
        // We scale float 0.0-1.0 to 0-5 radius during light calculation.
        float origLvl = pVariant->GetFloat();
        if (origLvl >= 1.0f)
        {
            g_fLightSourceOptimizeLevel = 5;
            real::GetApp()->GetVar("osgt_qol_lightopt_rad")->Set(origLvl);
        }
        else
        {
            float radLvl = float(uint32_t(origLvl * 5.0f)) / 5.0f;
            g_fLightSourceOptimizeLevel = (int)(radLvl * 5.0f);
            real::GetApp()->GetVar("osgt_qol_lightopt_rad")->Set(radLvl);
        }
    }

    static void LightOptimizerToggle(VariantList* pVariant)
    {
        Entity* pCheckbox = pVariant->Get(1).GetEntity();
        bool bChecked = pCheckbox->GetVar("checked")->GetUINT32() != 0;
        g_bLightSourcesOptimized = bChecked;
        real::GetApp()->GetVar("osgt_qol_lightopt_enabled")->Set(uint32_t(bChecked));
    }

    static void WorldRendererDrawTiles(void* this_, std::vector<Tile*>* tiles, int unk3)
    {
        // Disarm original light calc logic by tricking original renderer.
        // We will be doing our own "light pass" before calling DrawTiles itself.
        if (unk3 == 0 || !g_bLightSourcesOptimized)
        {
            real::WorldRendererDrawTiles(this_, tiles, unk3);
            return;
        }
        // This value happens to dictate if DrawTiles will calculate light levels, so we can
        // temporarily assign it to a value where it won't do that, we will be doing the light
        // calculation pass instead.
        float original = *(float*)(((__int64_t)this_) + 0x15c);
        *(float*)(((__int64_t)this_) + 0x15c) = 0.1f;

        std::vector<ItemInfo>& items = real::GetApp()->GetItemInfoManager()->m_itemInfo;
        int w = real::GetApp()->GetGameLogic()->GetTileWidth();
        int h = real::GetApp()->GetGameLogic()->GetTileHeight();
        WorldTileMap* tilemap = (WorldTileMap*)real::GetApp()->GetGameLogic()->GetTileMap();
        size_t max = tiles->size();

        int r = g_fLightSourceOptimizeLevel;

        int m_timeMS = real::GetApp()->m_gameTimer.m_timeMS;

        for (size_t i = 0; i < max; i++)
        {
            Tile* tile = (*tiles)[i];
            int vis = items[tile->m_itemID].m_visualEffect;
            // Lightsource / Lightsource Pulse / Lightsource If On
            if ((vis == 34 || vis == 45) || (vis == 35 && (tile->m_flags & 64)))
            {
                int x = tile->x;
                int y = tile->y;

                // Interpolation logic same as original.
                float lightStrength = SinPulseByCustomTimerMS(4000, (x + y) * 100 + m_timeMS);
                lightStrength = (lightStrength * 0.25f + 1.0f) * 128.0f;

                int minX = x - r;
                if (minX < 0)
                    minX = 0;
                int maxX = x + r;
                if (maxX > w)
                    maxX = w;

                int minY = y - r;
                if (minY < 0)
                    minY = 0;
                int maxY = y + r;
                if (maxY > h)
                    maxY = h;

                Tile* target = nullptr;
                for (x = minX; x < maxX; x++)
                {
                    for (y = minY; y < maxY; y++)
                    {
                        target = &tilemap->m_tiles[x + (y * w)];
                        vis = items[target->m_itemBGID].m_visualEffect;
                        // Only do light calculations if we're on a darkened tile.
                        if (vis == 33 || vis == 41)
                        {
                            // If we are on a light source, we are already on light level 0.0, skip
                            // a no-op.
                            if (target->m_lightLevel > 0.0f)
                            {
                                float rayPower;
                                if (real::WorldRendererTileLineOfSight(this_, tile, target, lightStrength, &rayPower))
                                {
                                    // Game resets tile->m_lightLevel across the board on each
                                    // render cycle, so we don't need to worry about perma-stuck
                                    // light tiles.
                                    float light = rayPower / lightStrength;
                                    if (light < target->m_lightLevel)
                                        target->m_lightLevel = light;
                                }
                            }
                        }
                    }
                }
                tile->m_lightLevel = 0.0;
            }
        }
        real::WorldRendererDrawTiles(this_, tiles, unk3);
        *(float*)(((__int64_t)this_) + 0x15c) = original;
    }
};
REGISTER_USER_GAME_PATCH(LightSourceOptimizer, lightsource_optimized);

class InstantWorldButtonsPatch : public patch::BasePatch
{
  public:
    void apply() const override
    {
        auto& game = game::GameHarness::get();

        // This effectively does almost same thing as the PC version, but we put "1" value directly into params which
        // get loaded into appropriate registers next and effectively make world buttons instant.
        void* floaterAddr = (void*)dlsym(game.getGameHandle(),
                                         "_Z10AddFloateriP6EntityRfS1_8CL_RectfSsRKSsifjRSt6vectorIS0_SaIS0_EERiRb");
        utils::writeMemoryPattern((void*)((int64_t)floaterAddr + 1788), "20 00 80 52");
        utils::nopInstruction((void*)((int64_t)floaterAddr + 1792), 2);

        utils::writeMemoryPattern((void*)((int64_t)floaterAddr + 1804), "20 00 80 52");
        utils::nopInstruction((void*)((int64_t)floaterAddr + 1808), 2);
    }
};
REGISTER_USER_GAME_PATCH(InstantWorldButtonsPatch, instant_world_buttons);

class AnchorCameraToPlayerPatch : public patch::BasePatch
{
  public:
    void apply() const override
    {
        auto& game = game::GameHarness::get();

        real::WorldCameraOnUpdate = (WorldCameraOnUpdate_t)game.resolveSymbol(pattern::WorldCameraOnUpdate);

        // Nop out the original WorldCamera::OnUpdate clamping logic. We'll add our own toggle for
        // it.
        utils::nopInstruction((void*)(((int64_t)real::WorldCameraOnUpdate) + 288), 29);

        // WorldCamera::OnUpdate hook for toggling between the anchoring.
        game.hookFunction<WorldCameraOnUpdate_t>(game.resolveSymbol(pattern::WorldCameraOnUpdate), WorldCameraOnUpdate,
                                                 &real::WorldCameraOnUpdate);
        // WorldCamera::GetCamWorldPos for clamping weathers separately while using anchored view,
        // prevents glitched out weathers.
        // TODO: This doesn't really function well, function is really small and expects a in_x8 register return.
        // game.hookFunction<WorldCameraGetCamWorldPos_t>(game.resolveSymbol(pattern::WorldCameraGetCamWorldPos),
        //                                               WorldCameraGetCamWorldPos, &real::WorldCameraGetCamWorldPos);

        // Our options, maybe they're better grouped together, but by categorization they should be
        // in UI and Input respectively.
        auto& optionsMgr = game::OptionsManager::get();
        optionsMgr.addCheckboxOption("qol", "UI", "osgt_qol_camera_clamp",
                                     "Always center camera on player (shows out of bounds)", &OnClampCallback);
        // Subscribe to events for hotkeys and renderer access
        auto& events = game::EventsAPI::get();

        // WorldRenderer::OnRender for rendering out-of-bounds greyed out area, otherwise it's not
        // really obvious where the world ends.
        events.m_sig_worldRendererOnRender.connect(&WorldRendererOnRender);
        events.m_sig_loadFromMem.connect(&loadFromMemCallback);
    }

    static void loadFromMemCallback()
    {
        // These will default to 0 on new vars.
        m_centerCameraOnPlayer = real::GetApp()->GetVar("osgt_qol_camera_clamp")->GetUINT32();
    }

    // Option callbacks
    static void OnClampCallback(VariantList* pVariant)
    {
        Entity* pCheckbox = pVariant->Get(1).GetEntity();
        m_centerCameraOnPlayer = pCheckbox->GetVar("checked")->GetUINT32() != 0;
        real::GetApp()->GetVar("osgt_qol_camera_clamp")->Set(uint32_t(m_centerCameraOnPlayer));
        // Camera update is called anyway each update cycle, so we don't really need to do anything
        // here.
    }

    // Helper functions for amount of pixels drawn out of world bounds
    static float getClampedRightBorder(WorldCamera& pCamera, float& clampX)
    {
        return pCamera.m_vScreenSize.x - ((pCamera.m_vCamWorldPosUpperLeft.x - clampX) * pCamera.m_vScale.x);
    }
    static float getClampedLeftBorder(WorldCamera& pCamera)
    {
        return std::abs(pCamera.m_vCamWorldPosUpperLeft.x) * pCamera.m_vScale.x;
    }
    static float getClampedTopBorder(WorldCamera& pCamera)
    {
        return std::abs(pCamera.m_vCamWorldPosUpperLeft.y) * pCamera.m_vScale.y;
    }
    static float getClampedBottomBorder(WorldCamera& pCamera, float& clampY)
    {
        return pCamera.m_vScreenSize.y - ((pCamera.m_vCamWorldPosUpperLeft.y - clampY) * pCamera.m_vScale.y);
    }

    // Camera & render tweaks
    static void WorldRendererOnRender(void* this__, CL_Vec2f* p2)
    {
        WorldRenderer* this_ = (WorldRenderer*)this__;
        if (m_centerCameraOnPlayer)
        {
            // We will shadow out the out of bounds areas to prevent confusion with centered camera.
            // For left and top sides, it's simple, we only need to use current camera position
            // (this is not directly relative to NetAvatar) and multiply it by the zoom level.
            // For right and botttom sides, we have to calculate against the tilemap clamping to get
            // appropriate x/y level.
            WorldTileMap* pTileMap = real::GetApp()->GetGameLogic()->GetTileMap();
            float xMax = (float)pTileMap->m_sizeX * 32.0f;
            float yMax = (float)pTileMap->m_sizeY * 32.0f;
            float clampX = xMax - this_->m_camera.m_vWorldSizeViewableInScreen.x;
            float clampY = yMax - this_->m_camera.m_vWorldSizeViewableInScreen.y;

            Rectf rect;              // shared rect struct for all the sides
            CL_Vec2f unk4(0.0, 0.0); // bgfx related, mandatory

            // Left side of world
            if (this_->m_camera.m_vCamWorldPosUpperLeft.x < 0.0)
            {
                // Draw from left (0) to right (clamp point) on entire height.
                rect.right = getClampedLeftBorder(this_->m_camera);
                rect.bottom = this_->m_camera.m_vScreenSize.y;
                rect.ceil();
                real::DrawFilledRect(rect, 0xAA, 0.0f, &unk4);
            }
            // Right side of world
            if (clampX < this_->m_camera.m_vCamWorldPosUpperLeft.x)
            {
                // Draw from left (clamp point) to edge of screen on entire height.
                rect.left = getClampedRightBorder(this_->m_camera, clampX);
                rect.right = this_->m_camera.m_vScreenSize.x;
                rect.top = 0;
                rect.bottom = this_->m_camera.m_vScreenSize.y;
                rect.ceil();
                real::DrawFilledRect(rect, 0xAA, 0.0f, &unk4);
            }
            // Top side of world
            if (this_->m_camera.m_vCamWorldPosUpperLeft.y < 0.0)
            {
                rect.right = rect.left = 0;
                // Exclude world sides on clamp points so we don't overlap.
                if (this_->m_camera.m_vCamWorldPosUpperLeft.x < 0.0)
                    rect.left = getClampedLeftBorder(this_->m_camera);
                if (clampX < this_->m_camera.m_vCamWorldPosUpperLeft.x)
                    rect.right = getClampedRightBorder(this_->m_camera, clampX);
                else
                    rect.right = this_->m_camera.m_vScreenSize.x;

                // Draw from top (0) to bottom (clamp point) on entire non-overlapping width.
                rect.bottom = getClampedTopBorder(this_->m_camera);
                rect.ceil();
                real::DrawFilledRect(rect, 0xAA, 0.0f, &unk4);
            }
            // Bottom side of world
            if (clampY < this_->m_camera.m_vCamWorldPosUpperLeft.y)
            {
                rect.left = rect.right = 0;
                // Exclude world sides on clamp points so we don't overlap.
                if (this_->m_camera.m_vCamWorldPosUpperLeft.x < 0.0)
                    rect.left = getClampedLeftBorder(this_->m_camera);
                if (clampX < this_->m_camera.m_vCamWorldPosUpperLeft.x)
                    rect.right = getClampedRightBorder(this_->m_camera, clampX);
                else
                    rect.right = this_->m_camera.m_vScreenSize.x;

                // Draw from top (clamp point) to bottom of screen on entire non-overlapping width.
                rect.top = getClampedBottomBorder(this_->m_camera, clampY);
                rect.bottom = this_->m_camera.m_vScreenSize.y;
                rect.ceil();
                real::DrawFilledRect(rect, 0xAA, 0.0f, &unk4);
            }
        }
    }

    static CL_Vec2f* WorldCameraGetCamWorldPos(WorldCamera* this_)
    {
        CL_Vec2f* res = real::WorldCameraGetCamWorldPos(this_);
        // Lock the background position on out-of-bounds
        if (m_centerCameraOnPlayer)
        {
            // Idiomatic way would be to retrieve TileMap through parent WorldRenderer of
            // WorldCamera, but this will suffice.
            WorldTileMap* pTileMap = real::GetApp()->GetGameLogic()->GetTileMap();
            if (this_->m_vCamWorldPosUpperLeft.x <= 0.0 && this_->m_vCamWorldPosUpperLeft.x != 0.0)
                res->x = 0.0f;
            if (this_->m_vCamWorldPosUpperLeft.y <= 0.0 && this_->m_vCamWorldPosUpperLeft.y != 0.0)
                res->y = 0.0;

            // Only clamp right side if we haven't clamped left side. This makes weathers not fall
            // apart when both sides are visible.
            if (res->x != 0.0f)
            {
                float clampX = (float)pTileMap->m_sizeX * 32.0f - this_->m_vWorldSizeViewableInScreen.x;
                if (clampX < this_->m_vCamWorldPosUpperLeft.x)
                    res->x = clampX;
            }
            if (res->y != 0.0f)
            {
                float clampY = (float)pTileMap->m_sizeY * 32.0f - this_->m_vWorldSizeViewableInScreen.y;
                if (clampY < this_->m_vCamWorldPosUpperLeft.y)
                    res->y = clampY;
            }
        }
        return res;
    }

    static void WorldCameraOnUpdate(WorldCamera* this_, CL_Vec2f* screenSize, CL_Vec2f* zoom)
    {
        real::WorldCameraOnUpdate(this_, screenSize, zoom);
        if (!m_centerCameraOnPlayer)
        {
            // Restores the vanilla client clamping logic if we're not using the modification.
            WorldTileMap* pTileMap = real::GetApp()->GetGameLogic()->GetTileMap();
            if (this_->m_vCamWorldPosUpperLeft.x <= 0.0 && this_->m_vCamWorldPosUpperLeft.x != 0.0)
                this_->m_vCamWorldPosUpperLeft.x = 0.0;
            if (this_->m_vCamWorldPosUpperLeft.y <= 0.0 && this_->m_vCamWorldPosUpperLeft.y != 0.0)
                this_->m_vCamWorldPosUpperLeft.y = 0.0;

            float clampX = (float)pTileMap->m_sizeX * 32.0f - this_->m_vWorldSizeViewableInScreen.x;
            if (clampX < this_->m_vCamWorldPosUpperLeft.x)
                this_->m_vCamWorldPosUpperLeft.x = clampX;
            float clampY = (float)pTileMap->m_sizeY * 32.0f - this_->m_vWorldSizeViewableInScreen.y;
            if (clampY < this_->m_vCamWorldPosUpperLeft.y)
                this_->m_vCamWorldPosUpperLeft.y = clampY;
        }
    }

  private:
    static bool m_centerCameraOnPlayer;
};
bool AnchorCameraToPlayerPatch::m_centerCameraOnPlayer = false;
REGISTER_USER_GAME_PATCH(AnchorCameraToPlayerPatch, anchor_camera_to_player);

class HotbarExpanded : public patch::BasePatch
{
  public:
    void apply() const override
    {
        // This patch attempts to find every place where hotbar items are limited and expands on
        // them to allow us create an expanded quicktools hotbar.
        // The mod allows the player to add up to 10 extra quickslots
        // Currently we are up to 6 extra quicktools (total of 9 useable items after fist).

        auto& game = game::GameHarness::get();
        // Hooks
        game.hookFunctionPattern<PlayerItemsFillBlankQuickToolSlotsWithStuff_t>(
            pattern::PlayerItemsFillBlankQuickToolSlotsWithStuff, PlayerItemsFillBlankQuickToolSlotsWithStuff,
            &real::PlayerItemsFillBlankQuickToolSlotsWithStuff);
        game.hookFunctionPattern<PlayerItemsRemoveFromQuickSlots_t>(pattern::PlayerItemsRemoveFromQuickSlots,
                                                                    PlayerItemsRemoveFromQuickSlots,
                                                                    &real::PlayerItemsRemoveFromQuickSlots);
        game.hookFunctionPattern<PlayerItemsUpdateQuickSlotsWithUsedItem_t>(
            pattern::PlayerItemsUpdateQuickSlotsWithUsedItem, PlayerItemsUpdateQuickSlotsWithUsedItem,
            &real::PlayerItemsUpdateQuickSlotsWithUsedItem);
        game.hookFunctionPattern<PlayerItemsSetQuickSlotItem_t>(
            pattern::PlayerItemsSetQuickSlotItem, PlayerItemsSetQuickSlotItem, &real::PlayerItemsSetQuickSlotItem);
        game.hookFunctionPattern<GameLogicComponentGetQuickToolInSlot_t>(pattern::GameLogicComponentGetQuickToolInSlot,
                                                                         GameLogicComponentGetQuickToolInSlot,
                                                                         &real::GameLogicComponentGetQuickToolInSlot);
        game.hookFunctionPattern<UpdateTouchControlPositions_t>(
            pattern::UpdateTouchControlPositions, UpdateTouchControlPositions, &real::UpdateTouchControlPositions);
        game.hookFunctionPattern<InventoryMenuCreate_t>(pattern::InventoryMenuCreate, InventoryMenuCreate,
                                                        &real::InventoryMenuCreate);

        // Function resolves
        RESOLVE_SYMBOL(AddTool);
        RESOLVE_SYMBOL(TradeMenuOnInventoryMoved);

        // Options
        auto& events = game::EventsAPI::get();
        events.m_sig_loadFromMem.connect(&loadFromMemCallback);

        auto& optionsMgr = game::OptionsManager::get();
        m_optionNames.push_back("4 total slots (vanilla)");
        for (int i = 5; i <= 10; i++)
            m_optionNames.push_back(toString(i) + " total slots");
        optionsMgr.addMultiChoiceOption("qol", "UI", "osgt_qol_hotbar_size", "Hotbar slots", m_optionNames,
                                        &OnInventoryResize, 80.0f);
    }

    static void loadFromMemCallback()
    {
        // Preferences
        Variant* pVariant = real::GetApp()->GetVar("osgt_qol_hotbar_size");
        if (pVariant->GetType() == Variant::TYPE_UINT32)
            m_extraSlots = pVariant->GetUINT32();
    }

    // Option callbacks
    static void OnInventoryResize(VariantList* pVariant)
    {
        // Update the weather index
        Entity* pClickedEnt = pVariant->Get(1).GetEntity();
        Variant* pOptVar = real::GetApp()->GetVar("osgt_qol_hotbar_size");
        uint32_t idx = pOptVar->GetUINT32();
        if (pClickedEnt->GetName() == "back")
        {
            if (idx == 0)
                idx = (uint32_t)m_optionNames.size() - 1;
            else
                idx--;
        }
        else if (pClickedEnt->GetName() == "next")
        {
            if (idx >= m_optionNames.size() - 1)
                idx = 0;
            else
                idx++;
        }
        pOptVar->Set(idx);
        // Update the option label
        Entity* pTextLabel = pClickedEnt->GetParent()->GetEntityByName("txt");
        real::SetTextEntity(pTextLabel, m_optionNames[idx]);
        m_extraSlots = idx;
        updateQuickToolsForNewSize(idx);

        // Also restore touch paddings if we've removed all extra slots
        if (m_extraSlots == 0)
        {
            Entity* pGameMenu = real::GetApp()->m_pEntityRoot->GetEntityByNameRecursively("GameMenu");
            if (!pGameMenu)
                return;
            Entity* pTouchEnt = pGameMenu->GetEntityByName("TouchControlsBG");
            if (!pTouchEnt)
                return;
            restoreTouchButtonPaddings(pTouchEnt);
        }
    }

    // Hooks
    static void PlayerItemsFillBlankQuickToolSlotsWithStuff(PlayerItems* pPlayerItems)
    {
        if (m_extraSlots == 0)
        {
            real::PlayerItemsFillBlankQuickToolSlotsWithStuff(pPlayerItems);
            return;
        }
        // Clear our custom slots
        for (int i = 0; i < m_extraSlots; i++)
            m_extendedSlots[i] = 0;
        int nextSlot = 0;
        for (auto it = pPlayerItems->m_items.begin(); it != pPlayerItems->m_items.end(); it++)
        {
            ItemInfo* pItem = &real::GetApp()->GetItemInfoManager()->m_itemInfo[(*it).m_itemID];
            if (pItem->m_type != 20 && pItem->m_type != 107 && pItem->m_type != 1)
            {
                if (nextSlot < 4)
                    pPlayerItems->m_quickSlots[nextSlot++] = pItem->m_itemID;
                else if (nextSlot < 4 + m_extraSlots)
                    m_extendedSlots[nextSlot++ - 4] = pItem->m_itemID;
                else
                    break;
            }
        }
    }

    static void PlayerItemsRemoveFromQuickSlots(PlayerItems* pPlayerItems, int itemID)
    {
        for (int i = 0; i < m_extraSlots; i++)
        {
            if (m_extendedSlots[i] == itemID)
                m_extendedSlots[i] = 0;
        }
        real::PlayerItemsRemoveFromQuickSlots(pPlayerItems, itemID);
    }

    static void PlayerItemsUpdateQuickSlotsWithUsedItem(PlayerItems* pPlayerItems, int itemID)
    {
        if (m_extraSlots == 0)
        {
            real::PlayerItemsUpdateQuickSlotsWithUsedItem(pPlayerItems, itemID);
            return;
        }
        // Replicate vanilla client behaviour here.
        if (itemID == 0 || itemID == 112 || itemID == 980 || itemID == 9186)
            return;
        if (itemID == 18 || itemID == 32)
        {
            pPlayerItems->m_quickSlots[0] = itemID;
            return;
        }
        ItemInfo* pItem = &real::GetApp()->GetItemInfoManager()->m_itemInfo[itemID];
        if (itemID != 6336 && pItem->m_type != 20 && pItem->m_type != 107 && pItem->m_type != 37 &&
            pItem->m_type != 114 && pItem->m_type != 129 && pItem->m_type != 64)
        {
            int slot = -1;
            for (int i = 0; i < 4; i++)
            {
                if (pPlayerItems->m_quickSlots[i] == itemID)
                {
                    slot = i;
                    break;
                }
            }
            if (slot == -1)
            {
                // Check our own extra slots separately
                for (int i = 0; i < m_extraSlots; i++)
                {
                    if (m_extendedSlots[i] == itemID)
                    {
                        slot = i;
                        break;
                    }
                }
            }
            if (slot == -1)
            {
                // Try find empty slots first on vanilla slots
                for (int i = 0; i < 4; i++)
                {
                    if (pPlayerItems->m_quickSlots[i] == 0)
                    {
                        pPlayerItems->m_quickSlots[i] = itemID;
                        return;
                    }
                }
                // Try our custom slots instead
                for (int i = 0; i < m_extraSlots; i++)
                {
                    if (m_extendedSlots[i] == 0)
                    {
                        m_extendedSlots[i] = itemID;
                        return;
                    }
                }
                // Nothing was free, shift everything to make room
                short moddedFirstItem = m_extendedSlots[0];
                for (int i = 0; i < m_extraSlots - 1; i++)
                    m_extendedSlots[i] = m_extendedSlots[i + 1];
                for (int i = 1; i < 3; i++)
                    pPlayerItems->m_quickSlots[i] = pPlayerItems->m_quickSlots[i + 1];
                pPlayerItems->m_quickSlots[3] = moddedFirstItem;
                m_extendedSlots[m_extraSlots - 1] = itemID;
            }
        }
    }

    static void PlayerItemsSetQuickSlotItem(void* pPlayerItems, int slot, short itemID)
    {
        if (slot < 4)
            real::PlayerItemsSetQuickSlotItem(pPlayerItems, slot, itemID);
        else if (slot < (4 + m_extraSlots))
            m_extendedSlots[slot - 4] = itemID;
    }

    static int GameLogicComponentGetQuickToolInSlot(GameLogicComponent* pGameLogic, int slot)
    {
        if (slot < 4)
            return real::GameLogicComponentGetQuickToolInSlot(pGameLogic, slot);
        else if (slot < (4 + m_extraSlots))
            return m_extendedSlots[slot - 4];
        return 0;
    }

    static void fixTouchButtonAlignment(Entity* pEnt, float yPos)
    {
        CL_Vec2f vNewPos = {pEnt->GetVar("pos2d")->GetVector2().x, yPos};
        pEnt->GetVar("pos2d")->Set(vNewPos);
    }

    static void setTouchButtonsPaddingPerElement(Entity* pEnt, Rectf& padding, bool bRevert)
    {
        if (bRevert)
        {
            if (pEnt->GetVar("touchPaddingOld")->GetType() != Variant::TYPE_UNUSED)
            {
                pEnt->GetVar("touchPadding")->Set(pEnt->GetVar("touchPaddingOld")->GetRect());
                pEnt->GetShared()->DeleteVar("touchPaddingOld");
            }
        }
        else
        {
            Variant* pTouchPadOld = pEnt->GetVar("touchPaddingOld");
            Variant* pTouchPad = pEnt->GetVar("touchPadding");
            // Save the old padding so we can restore it later
            if (pTouchPadOld->GetType() == Variant::TYPE_UNUSED)
                pTouchPadOld->Set(pTouchPad->GetRect());
            if (pTouchPad->GetRect() == pTouchPadOld->GetRect())
                pEnt->GetVar("touchPadding")->Set(padding);
        }
    }

    static bool doTouchButtonsNeedPaddings(Entity* pEnt)
    {
        Variant* pTouchPadOld = pEnt->GetVar("touchPaddingOld");
        Variant* pTouchPad = pEnt->GetVar("touchPadding");
        // Save the old padding so we can restore it later
        if (pTouchPadOld->GetType() == Variant::TYPE_UNUSED)
            return true;
        return pTouchPad->GetRect() == pTouchPadOld->GetRect();
    }

    static void restoreTouchButtonPaddings(Entity* pTouchEnt)
    {
        // Use a dummy rect for restoring
        Rectf padding;
        std::list<Entity*>* children = pTouchEnt->GetChildren();
        for (auto it = children->begin(); it != children->end(); it++)
            setTouchButtonsPaddingPerElement(*it, padding, true);
    }

    static void UpdateTouchControlPositions()
    {
        // No extra slots? Use vanilla logic.
        if (m_extraSlots == 0)
        {
            real::UpdateTouchControlPositions();
            return;
        }
        if (real::GetApp()->GetGameLogic()->m_pTradeMenu != nullptr)
            real::TradeMenuOnInventoryMoved(real::GetApp()->GetGameLogic()->m_pTradeMenu);
        // Realign touch controls to be higher up since the quickbar is just gonna overlap them when
        // expanded.
        Entity* pGameMenu = real::GetApp()->m_pEntityRoot->GetEntityByNameRecursively("GameMenu");
        if (!pGameMenu)
            return;
        Entity* pTouchEnt = pGameMenu->GetEntityByName("TouchControlsBG");
        if (pTouchEnt)
        {
            Entity* pTouchLeft = pTouchEnt->GetEntityByName("TouchLeft");
            Entity* pTouchRight = pTouchEnt->GetEntityByName("TouchRight");
            Entity* pJumpWideButton = pTouchEnt->GetEntityByName("JumpWideButton");
            Entity* pJumpButton = pTouchEnt->GetEntityByName("JumpButton");
            Entity* pPunchButton = pTouchEnt->GetEntityByName("PunchButton");
            Entity* pItemsParent = pGameMenu->GetEntityByName("ItemsParent");

            CL_Vec2f vItemsParentPos = pItemsParent->GetVar("pos2d")->GetVector2();
            CL_Vec2f vJumpButtonSize = pJumpButton->GetVar("size2d")->GetVector2();
            CL_Vec2f vArrowButtonSize = pTouchRight->GetVar("size2d")->GetVector2();

            // Set right/jump button bottom paddings to none to prevent input leakage on hotbar,
            // also get rid of the annoyinging large right padding on arrows. This also makes
            // paddings equal between buttons.
#ifdef ANDROID
            // TODO: Adjust margins for V4.03 + mobile
            if (doTouchButtonsNeedPaddings(pTouchRight))
            {
                float fPaddingRef = vJumpButtonSize.x / 4;
                Rectf rPaddingRect;
                rPaddingRect.left = 100.0f;
                rPaddingRect.right = 100.0f;
                // Top padding is a bit lacking for arrows.
                rPaddingRect.top = fPaddingRef;
                setTouchButtonsPaddingPerElement(pTouchRight, rPaddingRect, false);
                setTouchButtonsPaddingPerElement(pJumpButton, rPaddingRect, false);
                rPaddingRect.bottom = fPaddingRef;
                setTouchButtonsPaddingPerElement(pTouchLeft, rPaddingRect, false);
                setTouchButtonsPaddingPerElement(pPunchButton, rPaddingRect, false);
            }

            float fMarginY = -real::iPadMapY(40.0f);
#else
            if (doTouchButtonsNeedPaddings(pTouchRight))
            {
                float fPaddingRef = vJumpButtonSize.x / 4;
                Rectf rPaddingRect;
                rPaddingRect.left = 30.0f;
                rPaddingRect.right = 30.0f;
                rPaddingRect.top = fPaddingRef;
                setTouchButtonsPaddingPerElement(pTouchRight, rPaddingRect, false);
                setTouchButtonsPaddingPerElement(pJumpButton, rPaddingRect, false);
                rPaddingRect.bottom = fPaddingRef;
                setTouchButtonsPaddingPerElement(pTouchLeft, rPaddingRect, false);
                setTouchButtonsPaddingPerElement(pPunchButton, rPaddingRect, false);
            }

            float fMarginY = real::iPadMapY(5.0f);
#endif
            float fNewY = vItemsParentPos.y - vJumpButtonSize.y - fMarginY;
            Rectf screenRect = GetScreenRect();
            if (fNewY > screenRect.bottom)
                fNewY = screenRect.bottom;

            fixTouchButtonAlignment(pJumpButton, fNewY);
            fixTouchButtonAlignment(pPunchButton, fNewY);

            // Position the arrows to be more centered with jump/punch ones.
            fNewY = vItemsParentPos.y - vArrowButtonSize.y - ((vJumpButtonSize.y - vArrowButtonSize.y) / 2) - fMarginY;
            if (fNewY > screenRect.bottom)
                fNewY = screenRect.bottom;

            fixTouchButtonAlignment(pTouchLeft, fNewY);
            fixTouchButtonAlignment(pTouchRight, fNewY);
            fixTouchButtonAlignment(pJumpWideButton, fNewY);

            std::list<Entity*>* children = pTouchEnt->GetChildren();
            for (auto it = children->begin(); it != children->end(); it++)
            {
                if ((*it)->GetComponentByName("TouchHandler"))
                {
                    (*it)->RemoveComponentByName("TouchHandler");
                    EntityComponent* pComp = (EntityComponent*)operator new(0x120);
                    real::TouchHandlerComponent(pComp);
                    (*it)->AddComponent(pComp);
                }
                else
                {
                    (*it)->RemoveComponentByName("TouchHandlerArcade");
                    EntityComponent* pComp = (EntityComponent*)operator new(0x148);
                    real::TouchHandlerArcadeComponent(pComp);
                    (*it)->AddComponent(pComp);
                }
            }
        }
    }

    static float calculateAddedWidthPerTool(Entity* pToolMenu)
    {
        Entity* pTool0 = pToolMenu->GetEntityByName("Tool0");
        Entity* pTool1 = pToolMenu->GetEntityByName("Tool1");
        CL_Vec2f vToolSize = pTool0->GetVar("size2d")->GetVector2();
        float fWidthMargin =
            (pTool1->GetVar("pos2d")->GetVector2() - pTool0->GetVar("pos2d")->GetVector2()).x - vToolSize.x;
        return fWidthMargin + vToolSize.x;
    }

    static void InventoryMenuCreate(Entity* pEnt)
    {
        real::InventoryMenuCreate(pEnt);

        // Max is currently 4 without game crashing - needs investigation still.
        int iToolsToAdd = m_extraSlots;

        // Calculate added widths and reposition the tools
        Entity* pToolMenu = pEnt->GetEntityByNameRecursively("ToolSelectMenu");
        float fAddedWidth = calculateAddedWidthPerTool(pToolMenu) * iToolsToAdd;

        Variant* pVar = pToolMenu->GetVar("pos2d");
        CL_Vec2f pos2d = pVar->GetVector2();
        pos2d.x -= ceilf(fAddedWidth / 2);
        pVar->Set(pos2d);
        pVar = pToolMenu->GetVar("size2d");
        CL_Vec2f size2d = pVar->GetVector2();
        size2d.x += fAddedWidth;
        pVar->Set(size2d);

        // Create our tools
        iToolsToAdd += 3;
        for (int i = 4; i <= iToolsToAdd; i++)
            real::AddTool(i, pToolMenu);
    }

    static void updateQuickToolsForNewSize(int iToolsToAdd)
    {
        Entity* pToolMenu = real::GetApp()->m_pEntityRoot->GetEntityByNameRecursively("ToolSelectMenu");
        if (!pToolMenu)
            return;
        float fAddedWidth = calculateAddedWidthPerTool(pToolMenu);

        // Resize quick tools to match new slots
        int iCurrentTools = (int)pToolMenu->GetChildren()->size();
        if (iCurrentTools > (4 + iToolsToAdd))
        {
            // We need to remove slots, reduce width and remove tools
            int iToolsToRemove = iCurrentTools - (4 + iToolsToAdd);
            float fRemovedWidth = fAddedWidth * (iToolsToRemove);
            Variant* pVar = pToolMenu->GetVar("pos2d");
            CL_Vec2f pos2d = pVar->GetVector2();
            pos2d.x += ceilf(fRemovedWidth / 2);
            pVar->Set(pos2d);
            pVar = pToolMenu->GetVar("size2d");
            CL_Vec2f size2d = pVar->GetVector2();
            size2d.x -= fRemovedWidth;
            pVar->Set(size2d);

            for (int i = 4 + iToolsToAdd; i < iCurrentTools; i++)
            {
                Entity* pTool = pToolMenu->GetEntityByName("Tool" + toString(i));
                if (pTool)
                    pToolMenu->RemoveEntityByAddress(pTool);
                m_extendedSlots[i - 4] = 0;
            }
        }
        else
        {
            // We need to add slots, add width and tools
            fAddedWidth *= iToolsToAdd + 4 - iCurrentTools;
            Variant* pVar = pToolMenu->GetVar("pos2d");
            CL_Vec2f pos2d = pVar->GetVector2();
            pos2d.x -= ceilf(fAddedWidth / 2);
            pVar->Set(pos2d);
            pVar = pToolMenu->GetVar("size2d");
            CL_Vec2f size2d = pVar->GetVector2();
            size2d.x += fAddedWidth;
            pVar->Set(size2d);
            for (int i = iCurrentTools; i < iToolsToAdd + 4; i++)
                real::AddTool(i, pToolMenu);
            // We will need to re-populate the slots as well
            PlayerItemsFillBlankQuickToolSlotsWithStuff(&real::GetApp()->GetGameLogic()->m_playerItems);
        }
    }

  private:
    static short m_extendedSlots[6];
    static short m_extraSlots;
    static std::vector<std::string> m_optionNames;
};
short HotbarExpanded::m_extendedSlots[6] = {0, 0, 0, 0, 0, 0};
short HotbarExpanded::m_extraSlots = 0;
std::vector<std::string> HotbarExpanded::m_optionNames;
REGISTER_USER_GAME_PATCH(HotbarExpanded, hotbar_expanded);

static int gmsfNoteIDs[34] = {0,    420,  422,  424,  414,   416,   418,   426,   412,   4634, 4636, 4638,
                              4640, 4642, 4192, 5726, 5728,  5730,  5370,  6030,  6032,  6034, 6808, 6810,
                              6812, 7218, 7220, 7222, 10528, 10530, 10532, 10828, 10830, 10832};
static std::vector<std::string> g_overlayAggressivenessNames = {"Least obtrusive", "Overlay paint/spatula icons",
                                                                "Overlay icons & what to break",
                                                                "Overlay icons & mismatched tiles"};
class Buildomatica : public patch::BasePatch
{
    void apply() const override
    {
        // Buildomatica is a building schematic overlay mod for Growtopia heavily inspired by the
        // Minecraft mods Schematica and Litematica.
        // To use - create a "schematics" folder and paste your .gtworld, .dat and/or .GMSF worlds
        // in it. Next time you enter a world, the schematic will be overlayed.

        // FIXME:
        // - Steam Pistons, Revolvers, etc having missing components on holograms

        // Nice-to-have/not a priority:
        // - Look into possibility of using shaders in bgfx to highlight the "hologram" blocks.

        auto& game = game::GameHarness::get();
        // Hooks
        game.hookFunctionPattern<WorldRendererDrawBackgroundTiles_t>(pattern::WorldRendererDrawBackgroundTiles,
                                                                     WorldRendererDrawBackgroundTiles,
                                                                     &real::WorldRendererDrawBackgroundTiles);
        game.hookFunctionPattern<WorldRendererDrawWater_t>(pattern::WorldRendererDrawWater, WorldRendererDrawWater,
                                                           &real::WorldRendererDrawWater);
        game.hookFunctionPattern<WorldTileMapChooseVisual_SmartEdge_t>(pattern::WorldTileMapChooseVisual_SmartEdge,
                                                                       WorldTileMapChooseVisual_SmartEdge,
                                                                       &real::WorldTileMapChooseVisual_SmartEdge);
        game.hookFunctionPattern<WorldTileMapGetTileSafe_t>(pattern::WorldTileMapGetTileSafe, WorldTileMapGetTileSafe,
                                                            &real::WorldTileMapGetTileSafe);
        game.hookFunctionPattern<NetAvatarProcessTileWeAreDirectlyOver_t>(
            pattern::NetAvatarProcessTileWeAreDirectlyOver, NetAvatarProcessTileWeAreDirectlyOver,
            &real::NetAvatarProcessTileWeAreDirectlyOver);
        RESOLVE_SYMBOL(WorldTileMapChooseVisual);
        RESOLVE_SYMBOL(WorldTileMapChooseVisual_Flag);
        RESOLVE_SYMBOL(DrawTile);
        RESOLVE_SYMBOL(TextManagerAddTextByTile);
        real::g_fireBatcher = (void*)dlsym(game.getGameHandle(), pattern::g_fireBatcher.c_str());

        // We will reset our fake tilemap on map load
        auto& events = game::EventsAPI::get();
        events.m_sig_onMapLoaded.connect(&OnMapLoaded);
        events.m_sig_loadFromMem.connect(&loadFromMemCallback);

        // We'll want to overlay ontop of everything if there's anything amiss.
        events.m_sig_worldRendererOnRender.connect(&WorldRendererOnRender);

        auto& optionsMgr = game::OptionsManager::get();
        optionsMgr.addCheckboxOption("qol", "Buildomatica", "osgt_qol_buildomatica_toggle",
                                     "Render schematic if one exists for this world", &OnBuildomaticaToggle);
        optionsMgr.addMultiChoiceOption("qol", "Buildomatica", "osgt_qol_buildomatica_overlay_obtrusiveness",
                                        "Overlay Obtrusiveness", g_overlayAggressivenessNames,
                                        &OnBuildomaticaOverlayObtrusivity, 80.0f);
    }

    static void loadFromMemCallback()
    {
        Variant* pVariant = real::GetApp()->GetVar("osgt_qol_buildomatica_toggle");
        if (pVariant->GetType() == Variant::TYPE_UNUSED)
            pVariant->Set(1U);
        m_bModEnabled = pVariant->GetUINT32();

        pVariant = real::GetApp()->GetVar("osgt_qol_buildomatica_overlay_obtrusiveness");
        if (pVariant->GetType() == Variant::TYPE_UNUSED)
            pVariant->Set(1U); // Good middle-ground, overlay paint/flip ontop.
        m_overlayObtrusiveness = pVariant->GetUINT32();
    }

    // Options
    static void OnBuildomaticaToggle(VariantList* pVariant)
    {
        Entity* pCheckbox = pVariant->Get(1).GetEntity();
        m_bModEnabled = pCheckbox->GetVar("checked")->GetUINT32() != 0;
        real::GetApp()->GetVar("osgt_qol_buildomatica_toggle")->Set(uint32_t(m_bModEnabled));

        if (m_bModEnabled && real::GetApp()->GetGameLogic()->m_pWorld != nullptr)
            OnMapLoaded(real::GetApp()->GetGameLogic()->m_pWorldRenderer, 0, 0, 0);
        else
        {
            if (m_fakeTilemap.m_tiles.size() != 0)
                real::LogToConsole("`![Buildomatica]`` Unloaded schematic.");
            m_fakeTilemap.m_tiles.clear();
            m_cameraTiles.clear();
        }
    }

    static void OnBuildomaticaOverlayObtrusivity(VariantList* pVariant)
    {
        Entity* pClickedEnt = pVariant->Get(1).GetEntity();
        Variant* pOptVar = real::GetApp()->GetVar("osgt_qol_buildomatica_overlay_obtrusiveness");
        uint32_t idx = pOptVar->GetUINT32();
        if (pClickedEnt->GetName() == "back")
        {
            if (idx == 0)
                idx = (uint32_t)g_overlayAggressivenessNames.size() - 1;
            else
                idx--;
        }
        else if (pClickedEnt->GetName() == "next")
        {
            if (idx >= g_overlayAggressivenessNames.size() - 1)
                idx = 0;
            else
                idx++;
        }
        pOptVar->Set(idx);
        m_overlayObtrusiveness = idx;
        // Update the option label
        Entity* pTextLabel = pClickedEnt->GetParent()->GetEntityByName("txt");
        real::SetTextEntity(pTextLabel, g_overlayAggressivenessNames[idx]);
    }

    // Entry point for initializing the mod
    static void OnMapLoaded(void* this_, int64_t, int64_t, int64_t)
    {
        if (!m_bModEnabled)
            return;
        // Reset our fake tilemap details
        m_fakeTilemap.m_sizeX = ((WorldRenderer*)this_)->m_pWorld->m_tiles.m_sizeX;
        m_fakeTilemap.m_sizeY = ((WorldRenderer*)this_)->m_pWorld->m_tiles.m_sizeY;
        m_fakeTilemap.m_pParent = ((WorldRenderer*)this_)->m_pWorld;

        m_fakeTilemap.m_tiles.clear();
        m_cameraTiles.clear();

        // Load our schematic based on world name.. and also check for any nasty surprises before we
        // open anything.
        std::string& m_name = ((WorldRenderer*)this_)->m_pWorld->m_name;
        if (m_name.find("..") == std::string::npos && m_name.find("/") == std::string::npos &&
            m_name.find("%") == std::string::npos)
        {
            // Prioritize GPMAP, if it exists.
            int StatusCode = LoadFromGPMAP("schematics/" + m_name + ".dat");
            if (StatusCode != 0)
            {
                if (StatusCode != 1)
                {
                    real::LogToConsole(std::string("`![Buildomatica]`` Failed to load schematic - error code: 0-" +
                                                   toString(StatusCode))
                                           .c_str());
                    return;
                }
                StatusCode = LoadFromCernPlannerFile("schematics/" + m_name + ".gtworld");
                if (StatusCode > 2)
                    real::LogToConsole(std::string("`![Buildomatica]`` Failed to load schematic - error code: 1-" +
                                                   toString(StatusCode))
                                           .c_str());
            }
            if (StatusCode == 0)
                real::LogToConsole("`![Buildomatica]`` Loaded in schematic overlay.");

            int MusicStatusCode = LoadFromGMSF("schematics/" + m_name + ".GMSF");
            if (MusicStatusCode == 0)
                real::LogToConsole("`![Buildomatica]`` Loaded in schematic overlay for music.");
            else if (MusicStatusCode != 1)
                real::LogToConsole(std::string("`![Buildomatica]`` Failed to load music schematic - error code: " +
                                               toString(MusicStatusCode))
                                       .c_str());

            if (StatusCode != 0 && MusicStatusCode != 0)
                return;
            m_bDrawNotesOnly = StatusCode != 0 && MusicStatusCode == 0;
        }

        RebuildIndexes(&m_fakeTilemap);
        for (auto& t : m_fakeTilemap.m_tiles)
            real::WorldTileMapChooseVisual(&m_fakeTilemap, &t);
        CullTilemap(&m_fakeTilemap);
    }

    // Helpers
    static void RebuildIndexes(WorldTileMap* pTilemap)
    {
        // Replicate what client does with actual tilemap, it sets a rect for culling purposes and
        // indexes tiles by their order.
        for (int x = 0; x < pTilemap->m_sizeX; x++)
        {
            for (int y = 0; y < pTilemap->m_sizeY; y++)
            {
                int m_index = x + (y * pTilemap->m_sizeX);
                Tile& t = pTilemap->m_tiles[m_index];
                t.x = x;
                t.y = y;
                t.m_worldRect.left = t.x * 32.0f;
                t.m_worldRect.right = t.m_worldRect.left + 32.0f;
                t.m_worldRect.top = t.y * 32.0f;
                t.m_worldRect.bottom = t.m_worldRect.top + 32.0f;
                t.m_mapIndex = m_index;
            }
        }
    }

    static void CullTilemap(WorldTileMap* pTilemap)
    {
        m_cameraTiles.clear();
        WorldRenderer* pRender = (WorldRenderer*)real::GetApp()->GetGameLogic()->m_pWorldRenderer;
        Rectf m_viewableRect = {
            pRender->m_camera.m_vCamWorldPosUpperLeft.x - 32.f, pRender->m_camera.m_vCamWorldPosUpperLeft.y - 32.f,
            pRender->m_camera.m_vWorldSizeViewableInScreen.x + pRender->m_camera.m_vCamWorldPosUpperLeft.x + 32.f,
            pRender->m_camera.m_vWorldSizeViewableInScreen.y + pRender->m_camera.m_vCamWorldPosUpperLeft.y + 32.f};

        // This will reserve either the exact amount or slightly higher amount for vec.
        int x = (int)((m_viewableRect.right - m_viewableRect.left) / 32.0f);
        int y = (int)((m_viewableRect.bottom - m_viewableRect.top) / 32.0f);
        m_cameraTiles.reserve(x * y);
        for (auto& t : m_fakeTilemap.m_tiles)
        {
            if (t.m_worldRect.left >= m_viewableRect.left && t.m_worldRect.right <= m_viewableRect.right &&
                t.m_worldRect.top >= m_viewableRect.top && t.m_worldRect.bottom <= m_viewableRect.bottom)
                m_cameraTiles.push_back(&t);
        }
    }

    static int CalculatePaintColor(Tile* t)
    {
        int Flags = t->m_flags & TILE_PROPERTY_PAINT_BLACK;
        int r = 0xff;
        int g = 0xff;
        int b = 0xff;
        if (Flags == TILE_PROPERTY_PAINT_BLACK)
        {
            // Do not mux this one and also use slightly higher opacity for visibility.
            return 0x3C3C3CCC;
        }
        if (Flags == TILE_PROPERTY_PAINT_AQUA)
            r = 0x3C;
        else if (Flags == TILE_PROPERTY_PAINT_PURPLE)
            g = 0x3C;
        else if (Flags == TILE_PROPERTY_PAINT_YELLOW)
            b = 0x3C;
        else if (Flags == TILE_PROPERTY_PAINT_BLUE)
        {
            g = 0x3C;
            r = 0x3C;
        }
        else if (Flags == TILE_PROPERTY_PAINT_GREEN)
        {
            r = 0x3C;
            b = 0x3C;
        }
        else if (Flags == TILE_PROPERTY_PAINT_RED)
        {
            g = 0x3C;
            b = 0x3C;
        }
        return ColorCombine(0xff + (r << 8) + (g << 16) + (b << 24), 0xe8ffb0aa, 0.66f);
    }

    static int GetMuxedColorForTile(WorldRenderer* pRenderer, Tile* pTile, bool bFG)
    {
        ItemInfo* pItemInfo =
            real::GetApp()->GetItemInfoManager()->GetItemByIDSafe(bFG ? pTile->m_itemID : pTile->m_itemBGID);
        bool bPainted = pTile->currentColor != 0xe8ffb0aa;
        if (pItemInfo->m_visualEffect == 25)
        {
            // VISUAL_EFFECT_RAINBOW_SHIFT
            // (Shifty Blocks)
            int r, g, b = 0;
            float Hue = (float)(pTile->x + (pTile->x * 4) + pRenderer->m_rainbowCycle + (pTile->y << 3));
            while (360.f <= Hue)
                Hue -= 360.f;
            HSVToRGB(Hue, 1.0, 1.0, &r, &g, &b);
            return ColorCombine(0xff + (r << 8) + (g << 16) + (b << 24), 0xe8ffb0aa, 0.66f);
        }
        else if (pItemInfo->m_visualEffect == 36 && !bPainted)
        {
            // VISUAL_EFFECT_DISCOLOR
            // (Copper Plumbing)
            if (!(pItemInfo->m_itemID & 1))
                pItemInfo = real::GetApp()->GetItemInfoManager()->GetItemByIDSafe(pItemInfo->m_itemID + 1);
            return ColorCombine(pItemInfo->m_growInfo.m_overlayColor, 0xe8ffb0aa, 0.66f);
        }
        return pTile->currentColor;
    }

    // Hacks to make client prefer our tilemap inside DrawTile.
    static int WorldTileMapChooseVisual_SmartEdge(WorldTileMap* pTilemap, Tile* p2, int p3)
    {
        // Used for steam block borders
        return real::WorldTileMapChooseVisual_SmartEdge(m_bDrawingHologram ? &m_fakeTilemap : pTilemap, p2, p3);
    }

    static Tile* WorldTileMapGetTileSafe(WorldTileMap* pTilemap, int x, int y)
    {
        // Used in various places in DrawTile and WorldRenderer/Tilemap subfunctions such as
        // PickVisualOnTheFly which determines spike locations.
        return real::WorldTileMapGetTileSafe(m_bDrawingHologram ? &m_fakeTilemap : pTilemap, x, y);
    }

    // Rendering
    static void NetAvatarProcessTileWeAreDirectlyOver(void* this_)
    {
        // We'll draw any relevant tile data info as textobjects.
        real::NetAvatarProcessTileWeAreDirectlyOver(this_);

        // Likely no fake tilemap set up yet.
        if (m_cameraTiles.size() == 0)
            return;

        Rectf colRect = ((NetMoving*)this_)->GetCollisionRectWorld();

        // Get centered position
        int x = (int)((colRect.left + (colRect.right - colRect.left) * 0.5f) * 0.03125f);
        int y = (int)((colRect.top + (colRect.bottom - colRect.top) * 0.5f) * 0.03125f);

        Tile* pTile = real::WorldTileMapGetTileSafe(&m_fakeTilemap, x, y);
        if (pTile != nullptr)
        {
            ItemInfo* pInfo = real::GetApp()->GetItemInfoManager()->GetItemByIDSafe(pTile->m_itemID);
            if (pInfo->m_type == 99)
            {
                // TYPE_SUPERMUSIC
                int idx = pTile->x + (pTile->y * m_fakeTilemap.m_sizeX);
                if (m_gmsfRackLabels.find(idx) != m_gmsfRackLabels.end())
                {
                    // TODO: Compare and highlight. Needs TileExtra stuff.
                    TextObject* pObj = real::TextManagerAddTextByTile(
                        &real::GetApp()->GetGameLogic()->m_textManager, pTile,
                        m_gmsfRackLabels[idx].pattern + "\nVolume: " + toString(m_gmsfRackLabels[idx].Volume), true,
                        false, 1, false);
                    // Use same color as providers.
                    pObj->m_color = 0x404040ff;
                    pObj->m_colorBG = 0x969696ff;
                }
            }
        }
    }

    static void WorldRendererDrawBackgroundTiles(WorldRenderer* this_, std::vector<Tile*>* tiles)
    {
        if (m_bModEnabled || m_bDrawNotesOnly)
            CullTilemap(&m_fakeTilemap);
        if (m_bModEnabled)
        {
            // Draw our background "hologram" from fake tilemap after world background has been drawn.
            CL_Vec2f camera;
            WorldTileMap& m_origTilemap = this_->m_pWorld->m_tiles;
            for (auto& t : m_cameraTiles)
            {
                if (t->x >= m_origTilemap.m_sizeX || t->y >= m_origTilemap.m_sizeY)
                    continue;
                if (t->m_itemBGID == 0)
                    continue;
                Tile* m_pRef = &m_origTilemap.m_tiles[t->x + (t->y * m_origTilemap.m_sizeX)];
                if (m_pRef->m_itemID != 0 || m_pRef->m_itemBGID != 0)
                    continue;
                CL_Vec2f tilePos(t->x * 32.f, t->y * 32.f);
                this_->m_camera.WorldToScreen(&camera, tilePos);
                if (t->m_itemBGID != 0)
                    real::DrawTile(this_, t->m_itemBGID, t->m_visualBG, &camera, GetMuxedColorForTile(this_, t, false),
                                   t, 1, 0);
            }
        }
        real::WorldRendererDrawBackgroundTiles(this_, tiles);
        if (!m_bModEnabled || m_bDrawNotesOnly)
            return;
        // Draw our foreground "hologram" from fake tilemap after world bgs has been drawn.
        CL_Vec2f camera;
        WorldTileMap& m_origTilemap = this_->m_pWorld->m_tiles;
        m_bDrawingHologram = true;
        for (auto& t : m_cameraTiles)
        {
            if (t->x >= m_origTilemap.m_sizeX || t->y >= m_origTilemap.m_sizeY)
                continue;
            if (t->m_itemID == 0)
                continue;
            Tile* m_pRef = &m_origTilemap.m_tiles[t->x + (t->y * m_origTilemap.m_sizeX)];
            if ((m_pRef->m_itemBGID != 0 && t->m_itemID == 0) || t->m_itemID == m_pRef->m_itemID)
                continue;
            CL_Vec2f tilePos(t->x * 32.f, t->y * 32.f);
            this_->m_camera.WorldToScreen(&camera, tilePos);
            // Ignore seeds if they are somehow present in our data. Neither planner format supports
            // it, but converter tools may leave residue.
            if (t->m_itemID != 0 && !(t->m_itemID & 1))
            {
                ItemInfo* pInfo = real::GetApp()->GetItemInfoManager()->GetItemByIDSafe(t->m_itemID);
                if (pInfo->m_flags & 0x40)
                {
                    // Hack in workaround for NOSHADOW. Game expects alpha 0xFF, but that kinda
                    // ruins the overlay. One could nop out DrawTile+0x368 to avoid doing this, but
                    // it has unknown consequences.
                    pInfo->m_flags &= ~0x40;
                    real::DrawTile(this_, t->m_itemID, t->m_visual, &camera, GetMuxedColorForTile(this_, t, true), t, 0,
                                   0);
                    pInfo->m_flags |= 0x40;
                }
                else
                {
                    real::DrawTile(this_, t->m_itemID, t->m_visual, &camera, GetMuxedColorForTile(this_, t, true), t, 0,
                                   0);
                }
            }
        }
        m_bDrawingHologram = false;
    }

    static void WorldRendererDrawWater(WorldRenderer* this_, std::vector<Tile*>* tiles)
    {
        real::WorldRendererDrawWater(this_, tiles);
        if (!m_bModEnabled || m_bDrawNotesOnly)
            return;
        // Draw our water "hologram" from fake tilemap after rest of world has been drawn.
        // TODO: Fire skewing/animation
        CL_Vec2f camera;
        CL_Vec2f rotation(0, 0);
        WorldTileMap& m_origTilemap = this_->m_pWorld->m_tiles;
        for (auto& t : m_cameraTiles)
        {
            if (t->x >= m_origTilemap.m_sizeX || t->y >= m_origTilemap.m_sizeY)
                continue;
            Tile* m_pRef = &m_origTilemap.m_tiles[t->x + (t->y * m_origTilemap.m_sizeX)];
            if ((t->m_flags & TILE_PROPERTY_WATER) == (m_pRef->m_flags & TILE_PROPERTY_WATER) &&
                (t->m_flags & TILE_PROPERTY_FIRE) == (m_pRef->m_flags & TILE_PROPERTY_FIRE))
                continue;
            CL_Vec2f tilePos(t->x * 32.f, t->y * 32.f);
            tilePos = *this_->m_camera.WorldToScreen(&camera, tilePos);
            if (t->m_flags & TILE_PROPERTY_WATER)
            {
                int visual = real::WorldTileMapChooseVisual_Flag(&m_fakeTilemap, t->x, t->y, 0x400);
                this_->m_waterImg->BlitScaledAnim(tilePos.x, tilePos.y, visual % 8, visual >> 3,
                                                  &this_->m_camera.m_vScale, 0, 0xE8FFB050, 0, rotation, false, false,
                                                  real::g_globalBatcher);
            }
            else if (m_pRef->m_flags & TILE_PROPERTY_WATER)
            {
                // Tint it purplish red by using green water as base surface and tinting it red.
                int visual = real::WorldTileMapChooseVisual_Flag(&this_->m_pWorld->m_tiles, t->x, t->y, 0x400);
                this_->m_greenWaterImg->BlitScaledAnim(tilePos.x, tilePos.y, visual % 8, visual >> 3,
                                                       &this_->m_camera.m_vScale, 0, 0xFF90, 0, rotation, false, false,
                                                       real::g_globalBatcher);
            }
            else if (t->m_flags & TILE_PROPERTY_FIRE)
            {
                int visual = real::WorldTileMapChooseVisual_Flag(&m_fakeTilemap, t->x, t->y, 0x1000);
                this_->m_fireImg->BlitScaledAnim(tilePos.x, tilePos.y, visual % 8, visual >> 3,
                                                 &this_->m_camera.m_vScale, 0, 0xE8FFB0A0, 0, rotation, false, false,
                                                 real::g_fireBatcher);
            }
            else if (m_pRef->m_flags & TILE_PROPERTY_FIRE)
            {
                int visual = real::WorldTileMapChooseVisual_Flag(&this_->m_pWorld->m_tiles, t->x, t->y, 0x1000);
                this_->m_fireImg->BlitScaledAnim(tilePos.x, tilePos.y, visual % 8, visual >> 3,
                                                 &this_->m_camera.m_vScale, 0, 0xFFA0, 0, rotation, false, false,
                                                 real::g_fireBatcher);
            }
        }
        real::RenderBatcherFlush(real::g_globalBatcher, 0, -1);
        real::RenderBatcherFlush(real::g_fireBatcher, 0, -1);
    }

    static void WorldRendererOnRender(void* this__, CL_Vec2f*)
    {
        // TODO: highlight for backgrounds as well on mismatch.
        WorldRenderer* this_ = (WorldRenderer*)this__;
        if (!m_bModEnabled)
            return;
        if (m_overlayObtrusiveness == 0)
            return;
        // Overlay stuff ontop after everythings drawn.
        CL_Vec2f camera;
        WorldTileMap& m_origTilemap = this_->m_pWorld->m_tiles;
        for (auto& t : m_cameraTiles)
        {
            if (t->x >= m_origTilemap.m_sizeX || t->y >= m_origTilemap.m_sizeY)
                continue;
            Tile* m_pRef = &m_origTilemap.m_tiles[t->x + (t->y * m_origTilemap.m_sizeX)];
            bool bMatchingItem = m_pRef->m_itemID == t->m_itemID;
            int overlayIcon = -1;
            int iconTint = 0xFFFFFFAA;
            // Blit an icon if some of the properties mismatch.
            if (bMatchingItem && !m_bDrawNotesOnly)
            {
                ItemInfo* pItemInfo = real::GetApp()->GetItemInfoManager()->GetItemByIDSafe(t->m_itemID);
                if ((pItemInfo->m_flags & 1) &&
                    (t->m_flags & TILE_PROPERTY_FACING_LEFT) != (m_pRef->m_flags & TILE_PROPERTY_FACING_LEFT))
                    overlayIcon = 2966; // Enchanted Spatula
                else if ((t->m_flags & TILE_PROPERTY_GLUE) != (m_pRef->m_flags & TILE_PROPERTY_GLUE))
                    overlayIcon = 1866; // Block Glue
                else if ((t->m_flags & TILE_PROPERTY_PAINT_BLACK) != (m_pRef->m_flags & TILE_PROPERTY_PAINT_BLACK))
                {
                    // Overlay a paint bucket icon if we're mismatching a paint.
                    int Flags = t->m_flags & TILE_PROPERTY_PAINT_BLACK;
                    if (Flags == 0)
                        overlayIcon = 3492;
                    else if (Flags == TILE_PROPERTY_PAINT_BLACK)
                        overlayIcon = 3490;
                    else if (Flags == TILE_PROPERTY_PAINT_AQUA)
                        overlayIcon = 3484;
                    else if (Flags == TILE_PROPERTY_PAINT_PURPLE)
                        overlayIcon = 3488;
                    else if (Flags == TILE_PROPERTY_PAINT_YELLOW)
                        overlayIcon = 3480;
                    else if (Flags == TILE_PROPERTY_PAINT_BLUE)
                        overlayIcon = 3486;
                    else if (Flags == TILE_PROPERTY_PAINT_GREEN)
                        overlayIcon = 3482;
                    else if (Flags == TILE_PROPERTY_PAINT_RED)
                        overlayIcon = 3478;
                }
            }
            bool bMatchingBG = m_pRef->m_itemBGID == t->m_itemBGID;
            if (m_pRef->m_itemID == 0 && m_pRef->m_itemBGID == 0)
                continue;
            CL_Vec2f tilePos(t->x * 32.f, t->y * 32.f);
            this_->m_camera.WorldToScreen(&camera, tilePos);
            if (m_overlayObtrusiveness >= 2 && !bMatchingBG && (m_pRef->m_itemBGID != 0 || m_pRef->m_itemID != 0))
            {
                // Draw a vaporizer ray if we've placed a matching FG tile, but BG is wrong.
                if (m_overlayObtrusiveness >= 3 && t->m_itemBGID == 0 && bMatchingItem && m_pRef->m_itemID != 0 &&
                    !m_bDrawNotesOnly)
                {
                    overlayIcon = 3156;
                    iconTint = 0x3C3CFFAA;
                }
                else
                {
                    // Draw a ghost of intended background if wrong one is used and we're on the
                    // intended fg. Or highlight existing if you're supposed to get rid of it.
                    if (t->m_itemBGID != 0 && (bMatchingItem || m_pRef->m_itemID == 0))
                    {
                        // We want this to draw at vislevel 2 if there's no foreground tiles placed.
                        if (m_overlayObtrusiveness >= 3 || m_pRef->m_itemID == 0)
                        {
                            int col = 0xFF80;
                            // If we just missed one, make it a more neutral yellow tone, less
                            // confusing.
                            if (m_pRef->m_itemBGID == 0)
                                col = 0xFFFF80;
                            real::DrawTile(this_, t->m_itemBGID, t->m_visualBG, &camera, col, t, 1, 0);
                        }
                    }
                    else if (t->m_itemBGID == 0 && m_pRef->m_itemID == 0 && !m_bDrawNotesOnly)
                        real::DrawTile(this_, m_pRef->m_itemBGID, m_pRef->m_visualBG, &camera, 0xFF80, m_pRef, 1, 0);
                }
            }
            if (m_overlayObtrusiveness >= 2 && !bMatchingItem && m_pRef->m_itemID != 0 && !(m_pRef->m_itemID & 1))
            {
                // Draw a red overlay on tiles we need to break and overlay intended tile ontop if
                // there is one.
                if (!m_bDrawNotesOnly || (m_bDrawNotesOnly && t->m_itemBGID != 0))
                {
                    real::DrawTile(this_, m_pRef->m_itemID, m_pRef->m_visual, &camera, 0xFF40, m_pRef, 0, 0);
                    if (t->m_itemID != 0)
                    {
                        m_bDrawingHologram = true;
                        ItemInfo* pInfo = real::GetApp()->GetItemInfoManager()->GetItemByIDSafe(t->m_itemID);
                        if (pInfo->m_flags & 0x40)
                        {
                            // Hack in workaround for NOSHADOW here as well.
                            pInfo->m_flags &= ~0x40;
                            real::DrawTile(this_, t->m_itemID, t->m_visual, &camera, 0xFF80, t, 0, 0);
                            pInfo->m_flags |= 0x40;
                        }
                        else
                        {
                            real::DrawTile(this_, t->m_itemID, t->m_visual, &camera, 0xFF80, t, 0, 0);
                        }
                        m_bDrawingHologram = false;
                    }
                }
            }
            if (m_overlayObtrusiveness >= 1 && overlayIcon != -1)
            {
                // If we have any icon to draw, we'll need to masquerade tile's tilevisual and item
                // ID, otherwise it'll render garbage.
                int origItem = t->m_itemID;
                int origVis = t->m_visual;

                t->m_itemID = overlayIcon;
                t->m_visual = 0;
                real::DrawTile(this_, t->m_itemID, t->m_visual, &camera, iconTint, t, 0, 0);
                t->m_itemID = origItem;
                t->m_visual = origVis;
            }
        }
    }

    static int u16toi(const std::u16string& s)
    {
        std::string narrow(s.begin(), s.end());
        return std::atoi(narrow.c_str());
    }

    // World planner conversions
    static int LoadFromGPMAP(std::string Path)
    {
        std::ifstream world(real::GetSavePath() + Path);
        if (!world)
            return 1;
        // Load the file in as a wstring, we need to decompress it using lzstring library and it
        // requires a wstring.
        std::u16string m_saveData{std::istreambuf_iterator<char>(world), std::istreambuf_iterator<char>()};
        world.close();

        m_saveData = lzstring::decompressFromBase64(m_saveData);

        // Not a valid GPMAP2 file.
        size_t headerpos = m_saveData.find(u"GPMAP2");
        if (headerpos == std::wstring::npos)
            return 2;

        // Get position of the '|' delimiter after Width/Height/Weather data.
        size_t delim = m_saveData.find(u"|", headerpos + 7);
        if (delim == std::wstring::npos)
            return 3;

        std::u16string header = m_saveData.substr(7, delim);
        std::vector<std::u16string> tokens = StringTokenize(header, u",");

        // We need at least width/height available. Don't really care about weather.
        if (tokens.size() < 2)
            return 4;

        int Width = u16toi(tokens[0]);
        int Height = u16toi(tokens[1]);

        // Reject loading if sizes do not match.
        if (Width != m_fakeTilemap.m_sizeX || Height != m_fakeTilemap.m_sizeY)
            return 5;

        // Fill in the tilemap and set the tile tint in place.
        m_fakeTilemap.m_tiles.clear();
        m_fakeTilemap.m_tiles.reserve(m_fakeTilemap.m_sizeY * m_fakeTilemap.m_sizeX);
        for (int y = 0; y < m_fakeTilemap.m_sizeY; y++)
        {
            for (int x = 0; x < m_fakeTilemap.m_sizeX; x++)
            {
                m_fakeTilemap.m_tiles.push_back(Tile());
                // RGB #b0e8ff
                m_fakeTilemap.m_tiles.back().currentColor = 0xe8ffb0aa;
            }
        }

        // Parse the chunks. GPMAP2 works not too differently from .gtworld, it's just using Item
        // IDs rather than names and sections layers off the same way.
        size_t ChunkSize = Width * Height * 4;

        // Narrow down UTF-16 to UTF-8. codecvt doesn't help here, loses too much info.
        std::u16string data = m_saveData.substr(delim + 1, ChunkSize * 4);
        if (data.size() != ChunkSize * 4)
            return 6;
        uint8_t* pMem = new uint8_t[ChunkSize * 4];
        size_t ptr = 0;
        for (wchar_t c : data)
            pMem[ptr++] = (char)c;

        // "fg" layer
        ptr = 0;
        for (int i = 0; i < (Width * Height); i++)
        {
            int itemID = *((int*)(pMem + ptr));
            ItemInfo* pItem = real::GetApp()->GetItemInfoManager()->GetItemByIDSafe(itemID);
            if (pItem->m_itemID != 0 && !(pItem->m_itemID & 1))
            {
                m_fakeTilemap.m_tiles[i].m_itemID = itemID;
                m_fakeTilemap.m_tiles[i].m_collisionType = pItem->m_collisionType;
                m_fakeTilemap.m_tiles[i].m_collidable = pItem->m_collisionType != 0 && pItem->m_collisionType != 5;
            }
            ptr += 4;
        }
        // "bg" layer
        for (int i = 0; i < (Width * Height); i++)
        {
            int itemID = *((int*)(pMem + ptr));
            ItemInfo* pItem = real::GetApp()->GetItemInfoManager()->GetItemByIDSafe(itemID);
            if (pItem->m_itemID != 0)
                m_fakeTilemap.m_tiles[i].m_itemBGID = itemID;
            ptr += 4;
        }
        // "ex" layer
        for (int i = 0; i < (Width * Height); i++)
        {
            int flags = *((int*)(pMem + ptr));
            if (flags == 10001)
                m_fakeTilemap.m_tiles[i].m_flags |= TILE_PROPERTY_WATER;
            else if (flags == 10002)
                m_fakeTilemap.m_tiles[i].m_flags |= TILE_PROPERTY_FIRE;
            ptr += 4;
        }
        // "meta"
        for (int i = 0; i < (Width * Height); i++)
        {
            int Flag = *((int*)(pMem + ptr));
            ptr += 4;
            if (Flag & GPMAP_PROPERTY_GLUE)
                m_fakeTilemap.m_tiles[i].m_flags |= TILE_PROPERTY_GLUE;
            if (Flag & GPMAP_PROPERTY_FLIP)
                m_fakeTilemap.m_tiles[i].m_flags |= TILE_PROPERTY_FACING_LEFT;
            if (Flag & GPMAP_PROPERTY_TOGGLED)
                m_fakeTilemap.m_tiles[i].m_flags |= TILE_PROPERTY_TOGGLED | TILE_PROPERTY_SILENCED;
            if ((Flag & GPMAP_PROPERTY_PAINT_CHARCOAL) == GPMAP_PROPERTY_PAINT_CHARCOAL)
                m_fakeTilemap.m_tiles[i].m_flags |= TILE_PROPERTY_PAINT_BLACK;
            else if ((Flag & GPMAP_PROPERTY_PAINT_RED) == GPMAP_PROPERTY_PAINT_RED)
                m_fakeTilemap.m_tiles[i].m_flags |= TILE_PROPERTY_PAINT_RED;
            else if ((Flag & GPMAP_PROPERTY_PAINT_GREEN) == GPMAP_PROPERTY_PAINT_GREEN)
                m_fakeTilemap.m_tiles[i].m_flags |= TILE_PROPERTY_PAINT_GREEN;
            else if ((Flag & GPMAP_PROPERTY_PAINT_BLUE) == GPMAP_PROPERTY_PAINT_BLUE)
                m_fakeTilemap.m_tiles[i].m_flags |= TILE_PROPERTY_PAINT_BLUE;
            else if (Flag & GPMAP_PROPERTY_PAINT_YELLOW)
                m_fakeTilemap.m_tiles[i].m_flags |= TILE_PROPERTY_PAINT_YELLOW;
            else if (Flag & GPMAP_PROPERTY_PAINT_PURPLE)
                m_fakeTilemap.m_tiles[i].m_flags |= TILE_PROPERTY_PAINT_PURPLE;
            else if (Flag & GPMAP_PROPERTY_PAINT_AQUA)
                m_fakeTilemap.m_tiles[i].m_flags |= TILE_PROPERTY_PAINT_AQUA;
            if (m_fakeTilemap.m_tiles[i].m_flags & TILE_PROPERTY_PAINT_BLACK)
            {
                if (m_fakeTilemap.m_tiles[i].m_itemID == 0 && m_fakeTilemap.m_tiles[i].m_itemBGID == 0)
                    m_fakeTilemap.m_tiles[i].m_flags &= ~TILE_PROPERTY_PAINT_BLACK;
                else
                    m_fakeTilemap.m_tiles[i].currentColor = CalculatePaintColor(&m_fakeTilemap.m_tiles[i]);
            }
        }
        delete[] pMem;
        return 0;
    }

    static int LoadFromCernPlannerFile(std::string Path)
    {
        // Cernodile's World Planner format. Kinda horrible, bunch of string parsing.
        if (m_fakeTilemap.m_sizeY != 60 && m_fakeTilemap.m_sizeX != 100)
            return 1;
        std::ifstream world(real::GetSavePath() + Path);
        if (!world)
            return 2;
        std::string m_saveData{std::istreambuf_iterator<char>(world), std::istreambuf_iterator<char>()};
        world.close();
        if (m_saveData.substr(0, 18) != "%cernworldplanner;")
            return 3;

        m_fakeTilemap.m_tiles.clear();
        m_fakeTilemap.m_tiles.reserve(m_fakeTilemap.m_sizeY * m_fakeTilemap.m_sizeX);
        for (int y = 0; y < m_fakeTilemap.m_sizeY; y++)
        {
            for (int x = 0; x < m_fakeTilemap.m_sizeX; x++)
            {
                m_fakeTilemap.m_tiles.push_back(Tile());
                // RGB #b0e8ff
                m_fakeTilemap.m_tiles.back().currentColor = 0xe8ffb0aa;
            }
        }

        // Cernodile's World Planner packs layers into 4 comma-separated arrays
        std::vector<std::string> foreground, background;
        std::vector<int> water, glue;
        // Sections are walled off by "section=", so we can use them as guiderails for how much to
        // scan.
        if (m_saveData.find("fg=") != std::string::npos && m_saveData.find("bg=") != std::string::npos)
        {
            std::vector<std::string> m_layers = StringTokenize(
                m_saveData.substr(m_saveData.find("fg="), m_saveData.find("bg=") - m_saveData.find("fg=")).substr(3),
                "\n");
            for (int i = 0; i != m_layers.size(); i++)
            {
                std::vector<std::string> tiles = StringTokenize(m_layers[i], ",");
                for (int j = 0; j < tiles.size(); j++)
                    foreground.emplace_back(tiles[j]);
            }
        }
        if (m_saveData.find("bg=") != std::string::npos && m_saveData.find("water=") != std::string::npos)
        {
            std::vector<std::string> m_layers = StringTokenize(
                m_saveData.substr(m_saveData.find("bg="), m_saveData.find("water=") - m_saveData.find("bg=")).substr(3),
                "\n");
            for (int i = 0; i != m_layers.size(); i++)
            {
                std::vector<std::string> tiles = StringTokenize(m_layers[i], ",");
                for (int j = 0; j < tiles.size(); j++)
                    background.emplace_back(tiles[j]);
            }
        }
        if (m_saveData.find("water=") != std::string::npos && m_saveData.find("glue=") != std::string::npos)
        {
            std::vector<std::string> m_layers = StringTokenize(
                m_saveData.substr(m_saveData.find("water="), m_saveData.find("glue=") - m_saveData.find("water="))
                    .substr(6),
                "\n");
            for (int i = 0; i != m_layers.size(); i++)
            {
                std::vector<std::string> tiles = StringTokenize(m_layers[i], ",");
                for (int j = 0; j < tiles.size(); j++)
                    water.emplace_back(tiles[j] == "Water" ? 1 : tiles[j] == "Fire" ? 2 : 0);
            }
        }
        if (m_saveData.find("water=") != std::string::npos && m_saveData.find("glue=") != std::string::npos)
        {
            std::vector<std::string> m_layers =
                StringTokenize(m_saveData.substr(m_saveData.find("glue=")).substr(5), "\n");
            for (int i = 0; i != m_layers.size(); i++)
            {
                std::vector<std::string> tiles = StringTokenize(m_layers[i], ",");
                for (int j = 0; j < tiles.size(); j++)
                    glue.emplace_back(tiles[j] == "Block Glue" ? 1 : 0);
            }
        }

        // Planner and tilemap sizes don't match, lets not do any OOB writes here, bail out.
        if (foreground.size() > (m_fakeTilemap.m_sizeX * m_fakeTilemap.m_sizeY))
            return 4;
        for (int i = 0; i < foreground.size(); i++)
        {
            if (foreground[i].length() == 0)
                continue;
            std::vector<std::string> props = StringTokenize(foreground[i], "_");
            if (props.size() > 1)
            {
                if (foreground[i][2] == '_')
                {
                    char Paint = foreground[i][1];
                    switch (Paint)
                    {
                    case 'R':
                        m_fakeTilemap.m_tiles[i].m_flags |= TILE_PROPERTY_PAINT_RED;
                        break;
                    case 'G':
                        m_fakeTilemap.m_tiles[i].m_flags |= TILE_PROPERTY_PAINT_GREEN;
                        break;
                    case 'B':
                        m_fakeTilemap.m_tiles[i].m_flags |= TILE_PROPERTY_PAINT_BLUE;
                        break;
                    case 'P':
                        m_fakeTilemap.m_tiles[i].m_flags |= TILE_PROPERTY_PAINT_PURPLE;
                        break;
                    case 'Y':
                        m_fakeTilemap.m_tiles[i].m_flags |= TILE_PROPERTY_PAINT_YELLOW;
                        break;
                    case 'A':
                        m_fakeTilemap.m_tiles[i].m_flags |= TILE_PROPERTY_PAINT_AQUA;
                        break;
                    case 'C':
                        m_fakeTilemap.m_tiles[i].m_flags |= TILE_PROPERTY_PAINT_BLACK;
                        break;
                    }
                    m_fakeTilemap.m_tiles[i].currentColor = CalculatePaintColor(&m_fakeTilemap.m_tiles[i]);
                    foreground[i] = foreground[i].substr(3);
                }
                if (StringFromEndMatches(foreground[i], "_FL"))
                {
                    m_fakeTilemap.m_tiles[i].m_flags |= TILE_PROPERTY_FACING_LEFT;
                    foreground[i] = foreground[i].substr(0, foreground[i].length() - 3);
                }
            }
            ItemInfo* info = real::GetApp()->GetItemInfoManager()->GetItemByName(foreground[i]);
            if (info->m_itemID != 0)
            {
                (&m_fakeTilemap.m_tiles[i])->m_itemID = info->m_itemID;
                (&m_fakeTilemap.m_tiles[i])->m_collisionType = info->m_collisionType;
                (&m_fakeTilemap.m_tiles[i])->m_collidable = info->m_collisionType != 0 && info->m_collisionType != 5;
            }
        }
        for (int i = 0; i < background.size(); i++)
        {
            if (background[i].length() == 0)
                continue;
            ItemInfo* info = real::GetApp()->GetItemInfoManager()->GetItemByName(background[i]);
            if (info->m_itemID != 0)
                (&m_fakeTilemap.m_tiles[i])->m_itemBGID = info->m_itemID;
        }
        for (int i = 0; i < water.size(); i++)
        {
            if (water[i] == 1)
                (&m_fakeTilemap.m_tiles[i])->m_flags |= TILE_PROPERTY_WATER;
            else if (water[i] == 2)
                (&m_fakeTilemap.m_tiles[i])->m_flags |= TILE_PROPERTY_FIRE;
        }
        for (int i = 0; i < glue.size(); i++)
        {
            if (glue[i] != 0)
                (&m_fakeTilemap.m_tiles[i])->m_flags |= TILE_PROPERTY_GLUE;
        }
        return 0;
    }

    static std::string GMSFNoteToString(int ID, int position)
    {
        if (ID == 0)
            return "";
        std::string res;
        // Get the note key based off its position. Char 66 corresponds to capital 'B'. 65 corresponds to 'A'. Once we
        // get to 64, we're back on symbols table, so we can add 7 back to the char count and start decrementing down
        // from 'G' to 'C'.
        char noteKeyBase = 66 - (position % 7);
        if (noteKeyBase <= 64)
            noteKeyBase += 7;
        // We'll shift everything up by 0x20 on the ASCII table to get lowercase letters for second octave.
        if (position >= 7)
            noteKeyBase += 32;
        std::string noteKey;
        noteKey += noteKeyBase;
        if (ID >= 1 && ID <= 3)
            res = "P" + noteKey + (ID == 1 ? "-" : ID == 3 ? "b" : "#");
        else if (ID >= 4 && ID <= 6)
            res = "B" + noteKey + (ID == 4 ? "-" : ID == 6 ? "b" : "#");
        else if (ID == 7)
            res = "D" + noteKey + "-";
        else if (ID >= 9 && ID <= 11)
            res = "S" + noteKey + (ID == 9 ? "-" : ID == 11 ? "b" : "#");
        else if (ID >= 16 && ID <= 18)
            res = "F" + noteKey + (ID == 16 ? "-" : ID == 18 ? "b" : "#");
        else if (ID >= 20 && ID <= 22)
            res = "G" + noteKey + (ID == 20 ? "-" : ID == 22 ? "b" : "#");
        else if (ID >= 23 && ID <= 25)
            res = "V" + noteKey + (ID == 23 ? "-" : ID == 25 ? "b" : "#");
        else if (ID >= 26 && ID <= 28)
            res = "L" + noteKey + (ID == 26 ? "-" : ID == 28 ? "b" : "#");
        else if (ID >= 29 && ID <= 31)
            res = "E" + noteKey + (ID == 29 ? "-" : ID == 31 ? "b" : "#");
        else if (ID >= 32 && ID <= 34)
            res = "T" + noteKey + (ID == 32 ? "-" : ID == 34 ? "b" : "#");
        return res;
    }

    static int LoadFromGMSF(std::string Path)
    {
        std::ifstream world(real::GetSavePath() + Path, std::ios_base::binary);
        if (!world)
            return 1;
        world.seekg(0, std::ios_base::end);
        size_t length = world.tellg();
        world.seekg(0, std::ios_base::beg);

        if (length < 11)
        {
            world.close();
            return 2;
        }

        uint8_t* pMem = new uint8_t[length];
        world.read((char*)pMem, length);
        world.close();

        if (pMem[0] != 'G' || pMem[1] != 'M' || pMem[2] != 'S' || pMem[3] != 'F')
            return 3;

        int8_t m_ver = *((int8_t*)(pMem + 4));
        if (m_ver > 1)
            return 4;
        short m_rows = *((short*)(pMem + 8));

        short m_maxRows = (short)(m_fakeTilemap.m_sizeX * floor((float)m_fakeTilemap.m_sizeY / 14.0f));

        if (m_maxRows < m_rows)
            return 5;

        if (length < 12 + (m_rows * 14))
            return 6;

        // Verify if we even have a valid tilemap right now..
        if (m_fakeTilemap.m_tiles.size() != m_fakeTilemap.m_sizeY * m_fakeTilemap.m_sizeX)
        {
            m_fakeTilemap.m_tiles.clear();
            m_fakeTilemap.m_tiles.reserve(m_fakeTilemap.m_sizeY * m_fakeTilemap.m_sizeX);
            for (int y = 0; y < m_fakeTilemap.m_sizeY; y++)
            {
                for (int x = 0; x < m_fakeTilemap.m_sizeX; x++)
                {
                    m_fakeTilemap.m_tiles.push_back(Tile());
                    // RGB #b0e8ff
                    m_fakeTilemap.m_tiles.back().currentColor = 0xe8ffb0aa;
                }
            }
        }

        int ptr = 12;
        for (int y = 0; y < 14; y++)
        {
            for (int i = 0; i < m_rows; i++)
            {
                if (ptr >= length)
                    return 7;
                int baseY = (int)floor(i / m_fakeTilemap.m_sizeX) * 14;
                int noteID = *((int8_t*)(pMem + ptr++));
                Tile* pTarget =
                    &m_fakeTilemap.m_tiles[(i % m_fakeTilemap.m_sizeX) + ((baseY + y) * m_fakeTilemap.m_sizeX)];
                if (noteID == 15)
                {
                    // Audio Rack
                    std::string pattern;
                    bool bAddedLast = false;
                    for (int j = 0; j < 5; j++)
                    {
                        int type = *((int8_t*)(pMem + ptr++));
                        int position = *((int8_t*)(pMem + ptr++));
                        std::string note = GMSFNoteToString(type, position);
                        if (note.size() > 0)
                        {
                            if (bAddedLast)
                                pattern += " ";
                            pattern += note;
                            bAddedLast = true;
                        }
                        else
                            bAddedLast = false;
                    }
                    pTarget->m_itemID = 4632;
                    int volume = *((int8_t*)(pMem + ptr++));
                    AudioRackInfo audioObj{pattern, volume};
                    m_gmsfRackLabels[(i % m_fakeTilemap.m_sizeX) + ((baseY + y) * m_fakeTilemap.m_sizeX)] = audioObj;
                }
                else
                {
                    if (noteID > 0 && noteID <= 33)
                        pTarget->m_itemBGID = gmsfNoteIDs[noteID];
                }
            }
        }
        return 0;
    }

  private:
    struct AudioRackInfo
    {
        std::string pattern;
        int Volume;
    };
    static WorldTileMap m_fakeTilemap;
    static std::vector<Tile*> m_cameraTiles;
    static std::map<int, AudioRackInfo> m_gmsfRackLabels;
    static bool m_bModEnabled;
    static bool m_bDrawingHologram;
    static bool m_bDrawNotesOnly;
    static int m_overlayObtrusiveness;

    enum GPMAPTileProperties : unsigned int
    {
        GPMAP_PROPERTY_FLIP = 1,
        GPMAP_PROPERTY_GLUE = 2 << 0,
        GPMAP_PROPERTY_TOGGLED = 2 << 1,
        GPMAP_PROPERTY_UNK2 = 2 << 2,
        GPMAP_PROPERTY_PAINT_AQUA = 2 << 3,
        GPMAP_PROPERTY_PAINT_PURPLE = 2 << 4,
        GPMAP_PROPERTY_PAINT_YELLOW = 2 << 5,
        GPMAP_PROPERTY_PAINT_BLUE = GPMAP_PROPERTY_PAINT_AQUA | GPMAP_PROPERTY_PAINT_PURPLE,
        GPMAP_PROPERTY_PAINT_GREEN = GPMAP_PROPERTY_PAINT_AQUA | GPMAP_PROPERTY_PAINT_YELLOW,
        GPMAP_PROPERTY_PAINT_RED = GPMAP_PROPERTY_PAINT_PURPLE | GPMAP_PROPERTY_PAINT_YELLOW,
        GPMAP_PROPERTY_PAINT_CHARCOAL =
            GPMAP_PROPERTY_PAINT_AQUA | GPMAP_PROPERTY_PAINT_PURPLE | GPMAP_PROPERTY_PAINT_YELLOW,
    };
};
WorldTileMap Buildomatica::m_fakeTilemap = WorldTileMap();
std::vector<Tile*> Buildomatica::m_cameraTiles = std::vector<Tile*>();
std::map<int, Buildomatica::AudioRackInfo> Buildomatica::m_gmsfRackLabels;
bool Buildomatica::m_bModEnabled = true;
bool Buildomatica::m_bDrawingHologram = false;
bool Buildomatica::m_bDrawNotesOnly = false;
int Buildomatica::m_overlayObtrusiveness = 1;
REGISTER_USER_GAME_PATCH(Buildomatica, buildomatica);