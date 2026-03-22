#include "game/game.hpp"
#include "game/signatures.hpp"
#include "game/struct/component.hpp"
#include "game/struct/components/gamelogic.hpp"
#include "game/struct/components/mapbg.hpp"
#include "game/struct/entity.hpp"
#include "game/struct/graphics/background_blood.hpp"
#include "game/struct/graphics/background_default.hpp"
#include "game/struct/renderutils.hpp"
#include "game/struct/rtrect.hpp"
#include "game/struct/world/world.hpp"
#include "game/struct/world/worldcamera.hpp"

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