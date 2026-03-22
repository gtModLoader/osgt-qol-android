#include "game.hpp"
#include "game/struct/entity.hpp"
#include "game/struct/entityutils.hpp"
#include "signatures.hpp"
#include "struct/renderutils.hpp"
#include "struct/rtrect.hpp"
#include "struct/variant.hpp"

REGISTER_GAME_FUNCTION(GetScreenSizeXf, "_Z15GetScreenSizeXfv", float);
REGISTER_GAME_FUNCTION(GetScreenSizeYf, "_Z15GetScreenSizeYfv", float);

REGISTER_GAME_FUNCTION(CreateOverlayEntity, "_Z19CreateOverlayEntityP6EntitySsSsff", Entity*, Entity* pParentEnt,
                       std::string const& name, std::string const& fileName, float x, float y);

REGISTER_GAME_FUNCTION(AddBMPRectAroundEntity, "_Z22AddBMPRectAroundEntityP6Entityjjfbf5eFontb", void, Entity* pEnt,
                       uint32_t col1, uint32_t col2, float padding, bool bUnk, float fontScale, uint32_t fontID,
                       bool bUnk4);
REGISTER_GAME_FUNCTION(EntitySetScaleBySize, "_Z20EntitySetScaleBySizeP6Entity7CL_Vec2IfEbb", void, Entity*, CL_Vec2f&,
                       bool, bool);
// OptionsMenuAddContent
REGISTER_GAME_FUNCTION(OptionsMenuAddContent,
                       "_Z27OptionsMenuAddScrollContentP6EntitybRSt6vectorIS0_SaIS0_EERSt13unordered_"
                       "mapI14EInputControlsS0_18EInputControlsHashSt8equal_toIS6_ESaISt4pairIKS6_S0_EEE",
                       void, void* pScrollChild, void*, void*, void*);

// CreateSlider
// NOTE: Need to investigate the final strings. They're not there originally in Proton SDK.
/* CreateSlider(Entity*, float, float, float, std::string, std::string, std::string, std::string,
   std::string, bool, std::string, std::string) */
REGISTER_GAME_FUNCTION(CreateSlider, "_Z12CreateSliderP6EntityfffSsSsSsSsSsbSsSs", EntityComponent*, Entity* pBG,
                       float x, float y, float sizeX, std::string const& buttonFileName, std::string const& left,
                       std::string const& middle, std::string const& right, std::string const&, bool,
                       std::string const&, std::string const&);

// CreateCheckbox
// Again with trailing strings that aren't there in Proton.
REGISTER_GAME_FUNCTION(CreateCheckBox, "_Z14CreateCheckboxP6EntitySsSsffb5eFontfbSsbSs", Entity*, Entity* pBG,
                       std::string const& name, std::string const& text, float x, float y, bool bChecked,
                       uint32_t fontID, float fontScale, bool unclickable, std::string const& unk10,
                       std::string const& unk11, std::string const& unk12)

// CreateTextButtonEntity
REGISTER_GAME_FUNCTION(CreateTextButtonEntity, "_Z22CreateTextButtonEntityP6EntitySsffSsb14EInputControlsSsbSsbb",
                       Entity*, Entity* pParentEnt, std::string const& name, float x, float y, std::string const& text,
                       bool bUnderline, int unk7 /*=0*/, std::string const& controlGroup /*=""*/, bool unk9 /*=false*/,
                       std::string const& unk10 /*=""*/, bool unk11 /*=true*/, bool unk12 /*=false*/)

REGISTER_GAME_FUNCTION(iPadMapX, "_Z8iPadMapXf", float, float);
REGISTER_GAME_FUNCTION(iPadMapY, "_Z8iPadMapYf", float, float);
REGISTER_GAME_FUNCTION(iPhoneMapX, "_Z10iPhoneMapXf", float, float);
REGISTER_GAME_FUNCTION(iPhoneMapY, "_Z10iPhoneMapYf", float, float);

REGISTER_GAME_FUNCTION(AddFocusIfNeeded, "_Z16AddFocusIfNeededP6Entitybii", void, Entity*, bool, int);

REGISTER_GAME_FUNCTION(TouchHandlerComponent, "_ZN21TouchHandlerComponentC1Ev", EntityComponent*, void*);
REGISTER_GAME_FUNCTION(TouchHandlerArcadeComponent, "_ZN27TouchHandlerArcadeComponentC1Ev", EntityComponent*, void*);
REGISTER_GAME_FUNCTION(FilterInputComponent, "_ZN20FilterInputComponentC1Ev", EntityComponent*, void*);

REGISTER_GAME_FUNCTION(MessageManagerCallEntityFunction,
                       "_ZN14MessageManager18CallEntityFunctionEP6EntityiRKSsPK11VariantList13eTimingSystem", void,
                       void*, Entity*, int, std::string const&, VariantList*, int);

REGISTER_GAME_FUNCTION(GetMessageManager, "_Z17GetMessageManagerv", void*);
REGISTER_GAME_FUNCTION(DisableAllButtonsEntity, "_Z23DisableAllButtonsEntityP6Entityb", void, Entity*, bool);
REGISTER_GAME_FUNCTION(OptionsMenuCreate, "_Z17OptionsMenuCreateP6Entityb", void, Entity*, bool);
REGISTER_GAME_FUNCTION(AddHotkeyToButton, "_Z17AddHotKeyToButtonP6Entityj", void, Entity*, int);
REGISTER_GAME_FUNCTION(SetButtonStyleEntity, "_Z20SetButtonStyleEntityP6EntityN17Button2DComponent12eButtonStyleE",
                       void, Entity*, int);
struct ScrollSettings
{
    CL_Vec2f position = {0, 0};
    CL_Vec2f size = {0, 0};
    bool initFromParent = false;
    bool invertScrollBarProgress = false;
    Entity* const pParent = nullptr;
    bool enableFingerTracking = true;
    bool useScrollWidth = false;
};
REGISTER_GAME_FUNCTION(ScrollScroll, "_ZN6ScrollC1ERK14ScrollSettings", Entity*, void* pMem, ScrollSettings*);
REGISTER_GAME_FUNCTION(SetScrollProgressEntity, "_Z23SetScrollProgressEntityP6EntityRK7CL_Vec2IfE", void, Entity*,
                       CL_Vec2f*);
REGISTER_GAME_FUNCTION(OnMenuButtonPressed, "_Z19OnMenuButtonPressedP11VariantList", void, VariantList*);
REGISTER_GAME_FUNCTION(OnGemButtonPressed, "_Z18OnGemButtonPressedP11VariantList", void, VariantList*);
namespace game
{

OptionsManager& OptionsManager::get()
{
    static OptionsManager instance;
    return instance;
}

void game::OptionsManager::initialize()
{
    auto& game = game::GameHarness::get();

    RESOLVE_SYMBOL(GetScreenSizeXf);
    RESOLVE_SYMBOL(GetScreenSizeYf);
    RESOLVE_SYMBOL(CreateOverlayEntity);
    RESOLVE_SYMBOL(AddBMPRectAroundEntity);
    RESOLVE_SYMBOL(EntitySetScaleBySize);
    RESOLVE_SYMBOL(CreateSlider);
    RESOLVE_SYMBOL(CreateCheckBox);
    RESOLVE_SYMBOL(CreateTextButtonEntity);
    RESOLVE_SYMBOL(iPadMapX);
    RESOLVE_SYMBOL(iPadMapY);
    RESOLVE_SYMBOL(iPhoneMapX);
    RESOLVE_SYMBOL(iPhoneMapY);
    RESOLVE_SYMBOL(AddFocusIfNeeded);
    RESOLVE_SYMBOL(TouchHandlerComponent);
    RESOLVE_SYMBOL(TouchHandlerArcadeComponent);
    RESOLVE_SYMBOL(FilterInputComponent);
    RESOLVE_SYMBOL(ScrollScroll);
    RESOLVE_SYMBOL(SetScrollProgressEntity);
    RESOLVE_SYMBOL(DisableAllButtonsEntity);
    RESOLVE_SYMBOL(OptionsMenuCreate);
    RESOLVE_SYMBOL(AddHotkeyToButton);
    RESOLVE_SYMBOL(GetMessageManager);
    RESOLVE_SYMBOL(MessageManagerCallEntityFunction);
    RESOLVE_SYMBOL(SetButtonStyleEntity);

    // Hook
    game.hookFunction<OptionsMenuAddContent_t>(game.resolveSymbol(pattern::OptionsMenuAddContent),
                                               OptionsMenuAddContent, &real::OptionsMenuAddContent);
    game.hookFunction<OnMenuButtonPressed_t>(game.resolveSymbol(pattern::OnMenuButtonPressed), OnMenuButtonPressed,
                                             &real::OnMenuButtonPressed);
    game.hookFunction<OnGemButtonPressed_t>(game.resolveSymbol(pattern::OnGemButtonPressed), OnGemButtonPressed,
                                            &real::OnGemButtonPressed);
}

void OptionsManager::renderSlider(OptionsManager::GameOption& optionDef, void* pEntityPtr, float vPosX, float& vPosY)
{
    Entity* pEnt = reinterpret_cast<Entity*>(pEntityPtr);

    // The client uses this real::iPhoneMapY(20.0) * 2 in main page, but since it's not really
    // scaled in height, we'll opt to use a static padding.
    vPosY += 100.0f;

    // We really don't want it to be smaller than vanilla 1024x768 iPhoneMapX(360.0f), but we want
    // it to be leaner in high res.
    float vSixeX = real::iPhoneMapX(260.0f);
    if (vSixeX < 768.80f)
        vSixeX = 768.80f;

#ifdef ANDROID
    // Workaround for now since you can't apply defaults in apply on android. Might have to add something for that stage
    // specifically.
    Variant* pSaveVar = real::GetApp()->GetVar(optionDef.varName);
    if (pSaveVar->GetType() == Variant::TYPE_UNUSED)
        pSaveVar->Set(1.0f);
#endif

    // The final 4 string args aren't there in Proton, but they are in client. Currently they don't
    // seem to have much use.
    EntityComponent* pSliderComp =
        real::CreateSlider(pEnt, vPosX, vPosY, vSixeX, "interface/slider_button.rttex", "Min", optionDef.displayName,
                           "Max", "", false, "", "CustomSlider");

    CL_Vec2f vSliderSize = pSliderComp->GetParent()->GetVar("size2d")->GetVector2();
    // If we have a hint string assigned, we'll display it below the slider, helps with centering
    // and width issues.
    if (optionDef.extraInfo.size() > 0)
    {
        Entity* pTxtEnt = real::CreateTextLabelEntity(pEnt, "txt", vPosX + (vSixeX / 2), vPosY + vSliderSize.y + 25.0f,
                                                      optionDef.extraInfo);
        real::SetupTextEntity(pTxtEnt, 0, 0);
        pTxtEnt->GetVar("alignment")->Set(5U);

        vPosY += pTxtEnt->GetVar("size2d")->GetVector2().y + 25.0f;
    }

    // Move the slider button according to variable in SharedDB.
    pSliderComp->GetVar("progress")->Set(real::GetApp()->GetVar(optionDef.varName)->GetFloat());
    if (optionDef.signal != nullptr)
        pSliderComp->GetVar("progress")
            ->GetSigOnChanged()
            ->connect(reinterpret_cast<VariantCallback>(optionDef.signal));

    // Adjust margin for next option.
    vPosY += vSliderSize.y;
}

void OptionsManager::renderCheckbox(OptionsManager::GameOption& optionDef, void* pEntityPtr, float vPosX, float& vPosY)
{
    Entity* pEnt = reinterpret_cast<Entity*>(pEntityPtr);

    // Pad before next checkbox, client originally uses real::iPhoneMapY(20.0) in main page, but on
    // large screens it's kinda wack.
    vPosY += 30.0f;

    uint32_t fontID = 0;
    float fontScale = 1.0f;

    // Retrieve our variant as we need to set checkbox value as its created
    Variant* pVariant = real::GetApp()->GetVar(optionDef.varName);

    // The final 4 string args aren't there in Proton, but they are in client. Currently they don't
    // seem to have much use.
    Entity* pCheckbox = real::CreateCheckBox(pEnt, optionDef.varName, optionDef.displayName, vPosX, vPosY,
                                             pVariant->GetUINT32() == 1, fontID, fontScale, false, "", "", "");

    if (optionDef.signal != nullptr)
        pCheckbox->GetFunction("OnButtonSelected")
            ->sig_function.connect(reinterpret_cast<VariantListCallback>(optionDef.signal));

    // Adjust margin for next option.
    vPosY += pCheckbox->GetVar("size2d")->GetVector2().y;
}

void OptionsManager::renderMultiChoice(OptionsManager::GameOption& optionDef, void* pEntityPtr, float vPosX,
                                       float& vPosY)
{
    if (optionDef.displayOptions->size() == 0)
        return;

    float vSizeX = real::iPhoneMapX(180.0f) + real::iPhoneMapX(optionDef.vModSizeX);

    Entity* pEnt = reinterpret_cast<Entity*>(pEntityPtr);

    // Namespace it for multichoice callbacks to find label easier
    Entity* pMCEnt = new Entity("opt_mc_container");
    pEnt->AddEntity(pMCEnt);

    // Pad it similarly to rest of options
    vPosY += 30.0f;

    // Retrieve fontscale
    uint32_t fontID;
    float fontScale;
    real::GetFontAndScaleToFitThisLinesPerScreenY(fontID, fontScale, 28);

    // Create the option label
    Entity* pOptionsLabel =
        real::CreateTextLabelEntity(pMCEnt, "optLabel", vPosX + (vSizeX / 2), vPosY, optionDef.displayName);
    real::SetupTextEntity(pOptionsLabel, 0, 0);
    pOptionsLabel->GetVar("alignment")->Set(ALIGNMENT_UPPER_CENTER);

    vPosY += pOptionsLabel->GetVar("size2d")->GetVector2().y + real::iPhoneMapY(5.0);

    // Retrieve our variant as we need to set the shown option label with it.
    uint32_t idx = real::GetApp()->GetVar(optionDef.varName)->GetUINT32();
    if (idx > optionDef.displayOptions->size())
    {
        real::GetApp()->GetVar(optionDef.varName)->Set(0U);
        idx = 0;
    }

    // Re-scale for the multichoice modal itself.
    Rectf screenRect = GetScreenRect();
    if (screenRect.bottom <= 1080)
        real::GetFontAndScaleToFitThisLinesPerScreenY(fontID, fontScale, 24);
    else
        real::GetFontAndScaleToFitThisLinesPerScreenY(fontID, fontScale, 30);

    // They added some wack trailing args we don't care about to end of TextButtonEntity.
    Entity* pBackButton =
        real::CreateTextButtonEntity(pMCEnt, "back", vPosX, vPosY, " <<  ", false, 0, "", 0, "", 1, 0);
    real::SetupTextEntity(pBackButton, fontID, fontScale);
    real::AddBMPRectAroundEntity(pBackButton, 0xccb887ff, 0xccb887ff, real::iPadMapY(5.0), true, fontScale, fontID,
                                 false);
    SetTextShadowColor(pBackButton, 150);

    if (optionDef.type == OPTION_MULTICHOICE_DUALBUTTONS)
    {
        Entity* pBackButtonLower = real::CreateTextButtonEntity(
            pMCEnt, "back_lower", vPosX + pBackButton->GetVar("size2d")->GetVector2().x + real::iPadMapY(10.0), vPosY,
            " <  ", false, 0, "", 0, "", 1, 0);
        real::SetupTextEntity(pBackButtonLower, fontID, fontScale);
        real::AddBMPRectAroundEntity(pBackButtonLower, 0xccb887ff, 0xccb887ff, real::iPadMapY(5.0), true, fontScale,
                                     fontID, false);
        SetTextShadowColor(pBackButtonLower, 150);
        if (optionDef.signal != nullptr)
            pBackButtonLower->GetFunction("OnButtonSelected")
                ->sig_function.connect(reinterpret_cast<VariantListCallback>(optionDef.signal));
    }

    Entity* pTextLabel = real::CreateTextLabelEntity(pMCEnt, "txt", vPosX + (vSizeX / 2),
                                                     vPosY + (pBackButton->GetVar("size2d")->GetVector2().y / 2),
                                                     (*optionDef.displayOptions)[idx]);
    real::SetupTextEntity(pTextLabel, 0, 0);
    pTextLabel->GetVar("alignment")->Set(ALIGNMENT_CENTER);

    Entity* pNextButton =
        real::CreateTextButtonEntity(pMCEnt, "next", vPosX + vSizeX, vPosY, " >> ", false, 0, "", 0, "", 1, 0);
    pNextButton->GetVar("alignment")->Set(ALIGNMENT_UPPER_RIGHT);
    real::SetupTextEntity(pNextButton, fontID, fontScale);
    real::AddBMPRectAroundEntity(pNextButton, 0xccb887ff, 0xccb887ff, real::iPadMapY(5.0), true, fontScale, fontID,
                                 false);
    SetTextShadowColor(pNextButton, 150);

    if (optionDef.type == OPTION_MULTICHOICE_DUALBUTTONS)
    {
        Entity* pNextButtonLower = real::CreateTextButtonEntity(
            pMCEnt, "next_lower", vPosX + vSizeX - pNextButton->GetVar("size2d")->GetVector2().x - real::iPadMapY(10.0),
            vPosY, " >  ", false, 0, "", 0, "", 1, 0);
        pNextButtonLower->GetVar("alignment")->Set(ALIGNMENT_UPPER_RIGHT);
        real::SetupTextEntity(pNextButtonLower, fontID, fontScale);
        real::AddBMPRectAroundEntity(pNextButtonLower, 0xccb887ff, 0xccb887ff, real::iPadMapY(5.0), true, fontScale,
                                     fontID, false);
        SetTextShadowColor(pNextButtonLower, 150);
        if (optionDef.signal != nullptr)
            pNextButtonLower->GetFunction("OnButtonSelected")
                ->sig_function.connect(reinterpret_cast<VariantListCallback>(optionDef.signal));
    }

    if (optionDef.signal != nullptr)
    {
        pBackButton->GetFunction("OnButtonSelected")
            ->sig_function.connect(reinterpret_cast<VariantListCallback>(optionDef.signal));
        pNextButton->GetFunction("OnButtonSelected")
            ->sig_function.connect(reinterpret_cast<VariantListCallback>(optionDef.signal));
    }

    CL_Vec2f vButtonSize = pNextButton->GetVar("size2d")->GetVector2();
    // If we have a hint string assigned, we'll display it below the option, helps with centering
    // and width issues.
    if (optionDef.extraInfo.size() > 0)
    {
        Entity* pTxtEnt = real::CreateTextLabelEntity(pEnt, "txt", vPosX + (vSizeX / 2), vPosY + vButtonSize.y + 25.0f,
                                                      optionDef.extraInfo);
        real::SetupTextEntity(pTxtEnt, 0, 0);
        pTxtEnt->GetVar("alignment")->Set(ALIGNMENT_UPPER_CENTER);

        vPosY += pTxtEnt->GetVar("size2d")->GetVector2().y + 25.0f;
    }

    // Adjust margin for next option.
    vPosY += vButtonSize.y;
}

void OptionPageOnSelect(VariantList* pVL)
{
    if (pVL->Get(1).GetEntity()->GetName() == "Back")
    {
        // Kill the hotkey from repeat-firing, otherwise spamming esc can cause some weirdness.
        // pVL->Get(1).GetEntity()->RemoveComponentByName("SelectButtonWithCustomInput");

        // Kill rest of the menu and recreate original options.
        Entity* pParentEnt = pVL->Get(1).GetEntity()->GetParent();
        real::SlideScreen(pParentEnt, 0, 500, 0);
        real::MessageManagerCallEntityFunction(real::GetMessageManager(), pParentEnt, 500, "OnDelete", 0, 1);
        real::OptionsMenuCreate(real::GetApp()->m_pEntityRoot->GetEntityByName("GUI"),
                                pParentEnt->GetVar("FromMainMenu")->GetUINT32() == 1);
        return;
    }
}

void OptionsManager::HandleOptionPageButton(VariantList* pVL)
{
    // Kill the main options menu so we don't click-through it.
    Entity* pParentEnt = pVL->Get(1).GetEntity()->GetParent()->GetParent()->GetParent();
    uint32_t bFromMainMenu = pParentEnt->GetVar("FromMainMenu")->GetUINT32();

    // Kill the hotkey from repeat-firing, otherwise spamming esc can cause some weirdness.
    Entity* pParentBackBtn = pParentEnt->GetEntityByName("Back");
    // pParentBackBtn->RemoveComponentByName("SelectButtonWithCustomInput");

    real::DisableAllButtonsEntity(pParentEnt, true);
    real::SlideScreen(pParentEnt, 0, 500, 0);
    real::MessageManagerCallEntityFunction(real::GetMessageManager(), pParentEnt, 500, "OnDelete", 0, 1);

    // Construct our own OptionsPage Entity
    Entity* pClickedEnt = pVL->Get(1).GetEntity();
    auto& optionsMgr = game::OptionsManager::get();
    if (optionsMgr.optionPages.find(pClickedEnt->GetName()) == optionsMgr.optionPages.end())
        return;
    Entity* pOverEnt = real::CreateOverlayEntity(real::GetApp()->m_pEntityRoot->GetEntityByName("GUI"), "OptionsPage",
                                                 "interface/large/generic_menu.rttex", 0, 0);
    real::AddFocusIfNeeded(pOverEnt, true, 500);

    // This is necessary for Options mainly to know when we re-create it later.
    pOverEnt->GetVar("FromMainMenu")->Set(bFromMainMenu);

    // We will size it to match our screen.
    Rectf screenRect = GetScreenRect();
    CL_Vec2f vScreenSize(screenRect.right, screenRect.bottom);
    real::EntitySetScaleBySize(pOverEnt, vScreenSize, 0, 0);

    // Setup the dimensions of where the scroll area will go
    CL_Vec2f vTextAreaPos = CL_Vec2f((float)(int)real::iPhoneMapX(2), real::iPhoneMapY(10));
    float offsetFromBottom = real::iPhoneMapY(48);
    float offsetFromRight = real::iPhoneMapY(0);
    CL_Vec2f vTextAreaBounds = (vScreenSize - CL_Vec2f(offsetFromRight, offsetFromBottom)) - vTextAreaPos;

    // Create scroll itself - using vanilla Proton components won't work as Growtopia modified them
    // heavily. They will function, but you can't use sliders properly with vanilla ones.
    ScrollSettings scrollSet;
    scrollSet.position = vTextAreaPos;
    scrollSet.size = vTextAreaBounds;
    Entity* pScroll = (Entity*)operator new(0x1b8);
    real::ScrollScroll(pScroll, &scrollSet);
    pOverEnt->AddEntity(pScroll);
    Entity* pScrollChild = pScroll->GetEntityByName("scroll_child");

    OptionsManager::OptionsPage& page = optionsMgr.optionPages[pClickedEnt->GetName()];
    float vPosX = real::iPhoneMapX(5.0);
    float vPosY = 0;

    uint32_t fontID;
    float fontScale;
    real::GetFontAndScaleToFitThisLinesPerScreenY(fontID, fontScale, 11);

    // Blit the page title
    Entity* pPageLabel = real::CreateTextLabelEntity(
        pScrollChild, "title", vPosX, vPosY, page.fancyName.size() == 0 ? pClickedEnt->GetName() : page.fancyName);
    real::SetupTextEntity(pPageLabel, fontID, 0);
    vPosY += pPageLabel->GetVar("size2d")->GetVector2().y;

    for (auto& section : optionsMgr.optionPages[pClickedEnt->GetName()].sections)
    {
        Entity* pSectionLabel = real::CreateTextLabelEntity(pScrollChild, "section", vPosX, vPosY, section.first);
        real::SetupTextEntity(pSectionLabel, fontID, 0);
        vPosY += pSectionLabel->GetVar("size2d")->GetVector2().y;
        for (auto& option : section.second)
        {
            switch (option.type)
            {
            case game::OptionsManager::OPTION_SLIDER:
            {
                renderSlider(option, (void*)pScrollChild, vPosX, vPosY);
                break;
            }
            case game::OptionsManager::OPTION_CHECKBOX:
            {
                renderCheckbox(option, (void*)pScrollChild, vPosX, vPosY);
                break;
            }
            case game::OptionsManager::OPTION_MULTICHOICE:
            case game::OptionsManager::OPTION_MULTICHOICE_DUALBUTTONS:
            {
                renderMultiChoice(option, (void*)pScrollChild, vPosX, vPosY);
                break;
            }
            default:
                break;
            }
        }
        vPosY += real::iPhoneMapY(20.0);
    }

    // Resize scroll bounds before sending.
    VariantList vl(pScrollChild->GetParent()->GetParent());
    real::ResizeScrollBounds(&vl);

    // and blit a Back button.
    real::GetFontAndScaleToFitThisLinesPerScreenY(fontID, fontScale, 13);
    Entity* pBackButton =
        real::CreateTextButtonEntity(pOverEnt, "Back", real::iPhoneMapX(25.0),
                                     screenRect.bottom - real::iPhoneMapY(40.0), "Back", false, 0, "", 0, "", 1, 0);
    pBackButton->GetFunction("OnButtonSelected")->sig_function.connect(&OptionPageOnSelect);
    real::SetupTextEntity(pBackButton, fontID, fontScale);
    real::AddBMPRectAroundEntity(pBackButton, 0xccb887ff, 0xccb887ff, real::iPadMapY(20.0), true, fontScale, fontID,
                                 false);
    SetTextShadowColor(pBackButton, 150);
    real::AddHotkeyToButton(pBackButton, 500000);
    real::SetButtonStyleEntity(pBackButton, 1);
    real::SlideScreen(pOverEnt, true, 500, 1);
}

void OptionsManager::HandleOptionPageScrollButton(VariantList* pVL)
{
    CL_Vec2f progress = {0.0f, 1.0f};
    real::SetScrollProgressEntity(pVL->Get(1).GetEntity()->GetParent()->GetParent(), &progress);
}

void OptionsManager::OptionsMenuAddContent(void* pEnt, void* unk2, void* unk3, void* unk4)
{
    // Let the game construct options menu for us.
    real::OptionsMenuAddContent(pEnt, unk2, unk3, unk4);
    Entity* pScrollChild = reinterpret_cast<Entity*>(pEnt);

    // Patch the uncentered buttons located after vanilla checkboxes.
    /*int i = 0;
    for (const auto& ent : *pScrollChild->GetChildren())
    {
        // Bit lazy, but I don't want to do 5 string comparisons every loop cycle
        // or iterate over the list 5 separate times.
        if (i > 0 || ent->GetName() == "support")
        {
            // Fix the alignment manually. It's not exactly "perfect", but it beats vanilla
            // alignment.
            CL_Vec2f pos2d = ent->GetVar("pos2d")->GetVector2();
            pos2d.x += real::iPadMapY(10.0f);
            ent->GetVar("pos2d")->Set(pos2d);
            if (++i >= 5)
                break;
        }
    }
    */

    // Use final or penultimate element of the menu for anchoring our elements to.
    Entity* pLastEntity = pScrollChild->GetChildren()->back();
    if (pLastEntity->GetName() == "SkinSelected")
        pLastEntity = *std::prev(pScrollChild->GetChildren()->end(), 2);
    // I know, iPhoneMapX used on a Y pos, but that's apparently what the client does.
    float vPosY = real::iPhoneMapX(5.0);
    vPosY += pLastEntity->GetVar("pos2d")->GetVector2().y;
    vPosY += pLastEntity->GetVar("size2d")->GetVector2().y;
    float vPosX = real::iPhoneMapX(5.0);

    // Create scaling for our label
    uint32_t fontID;
    float fontScale;
    real::GetFontAndScaleToFitThisLinesPerScreenY(fontID, fontScale, 16);

    // Create a quick-jump to bottom button next to parental settings
    Entity* pParentalSettings = pScrollChild->GetEntityByName("Parental");
    CL_Vec2f vParentalSize = pParentalSettings->GetVar("size2d")->GetVector2();
    CL_Vec2f vParentalPos = pParentalSettings->GetVar("pos2d")->GetVector2();
    Entity* pButtonEnt = real::CreateTextButtonEntity(
        pScrollChild, "JumpButton", vPosX + vParentalSize.x + real::iPadMapX(40.0f),
        vParentalPos.y + real::iPadMapY(10.0f), "Jump to Bottom", false, 0, "", 0, "", 1, 0);
    real::SetupTextEntity(pButtonEnt, fontID, fontScale);
    real::AddBMPRectAroundEntity(pButtonEnt, 0xfbe3a5ff, 0xfbe3a5ff, real::iPadMapX(5.0), true, fontScale, fontID,
                                 false);
    SetTextShadowColor(pButtonEnt, 150);
    LOG_DEBUG("pButtonEnt OnButtSelect %p", pButtonEnt->GetFunction("OnButtonSelected"));
    pButtonEnt->GetFunction("OnButtonSelected")->sig_function.connect(&HandleOptionPageScrollButton);

    // Create our very own label for modded options.
    // 11 lines on desktop, 9 on mobile platforms.
    real::GetFontAndScaleToFitThisLinesPerScreenY(fontID, fontScale, 9);
    Entity* pOptionsLabel =
        real::CreateTextLabelEntity(pScrollChild, "osgt_qol_options", vPosX, vPosY, "Modded Options:");
    // Set scaling for label.
    real::SetupTextEntity(pOptionsLabel, fontID, 0);

    // Move cursor further down by label size.
    vPosY += pOptionsLabel->GetVar("size2d")->GetVector2().y + real::iPhoneMapY(10.0);

    // Lets start building our options.
    auto& optionsMgr = game::OptionsManager::get();

    real::GetFontAndScaleToFitThisLinesPerScreenY(fontID, fontScale, 20);
    for (auto& page : optionsMgr.optionPages)
    {
        Entity* pButtonEnt = real::CreateTextButtonEntity(
            pScrollChild, page.first, vPosX, vPosY,
            page.second.fancyName.size() > 0 ? page.second.fancyName : page.first, false, 0, "", 0, "", 1, 0);
        real::SetupTextEntity(pButtonEnt, fontID, fontScale);
        real::AddBMPRectAroundEntity(pButtonEnt, 0xccb887ff, 0xccb887ff, real::iPadMapY(20.0), true, fontScale, fontID,
                                     false);
        SetTextShadowColor(pButtonEnt, 150);
        CL_Vec2f pos2d = pButtonEnt->GetVar("pos2d")->GetVector2();
        pos2d.x += real::iPadMapY(10.0f);
        pButtonEnt->GetVar("pos2d")->Set(pos2d);
        pButtonEnt->GetFunction("OnButtonSelected")->sig_function.connect(&HandleOptionPageButton);
        vPosY += real::iPadMapY(20.0f) + real::iPhoneMapY(20.0f) + real::iPhoneMapX(5.0f);
    }
    for (auto& option : optionsMgr.rootOptions)
    {
        switch (option.type)
        {
        case game::OptionsManager::OPTION_SLIDER:
        {
            renderSlider(option, (void*)pScrollChild, vPosX, vPosY);
            break;
        }
        case game::OptionsManager::OPTION_CHECKBOX:
        {
            renderCheckbox(option, (void*)pScrollChild, vPosX, vPosY);
            break;
        }
        case game::OptionsManager::OPTION_MULTICHOICE:
        case game::OptionsManager::OPTION_MULTICHOICE_DUALBUTTONS:
        {
            renderMultiChoice(option, (void*)pScrollChild, vPosX, vPosY);
            break;
        }
        default:
            break;
        }
    }

    // Finally resize.
    VariantList vl(pScrollChild->GetParent()->GetParent());
    real::ResizeScrollBounds(&vl);
    return;
}

void OptionsManager::OnMenuButtonPressed(VariantList* pVL)
{
    // Do we care if anyone else needs OnMenuButtonPressed...? I guess they can create a signal for
    // it if they need it.
    if (real::GetApp()->m_pEntityRoot->GetEntityByNameRecursively("OptionsMenu") ||
        real::GetApp()->m_pEntityRoot->GetEntityByNameRecursively("OptionsPage"))
        return;
    real::OnMenuButtonPressed(pVL);
}

void OptionsManager::OnGemButtonPressed(VariantList* pVL)
{
    // Same comment as for MenuButtonPressed.
    if (real::GetApp()->m_pEntityRoot->GetEntityByNameRecursively("OptionsMenu") ||
        real::GetApp()->m_pEntityRoot->GetEntityByNameRecursively("OptionsPage"))
        return;
    real::OnGemButtonPressed(pVL);
}

}; // namespace game