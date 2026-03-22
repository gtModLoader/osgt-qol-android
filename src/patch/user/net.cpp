#include "game/game.hpp"
#include "game/signatures.hpp"
#include "game/struct/entity.hpp"
#include "game/struct/variant.hpp"
#include "patch/patch.hpp"

REGISTER_GAME_FUNCTION(OnlineMenuCreate, "_Z16OnlineMenuCreateP6Entity", Entity*, Entity*);
REGISTER_GAME_FUNCTION(OnlineMenuOnConnect, "_Z19OnlineMenuOnConnectP6Entity", Entity*, Entity*);
REGISTER_GAME_FUNCTION(UbiservicesHTTPSOperationDoStart, "_ZN25UbiservicesHTTPSOperation7DoStartEv", void, void*);
REGISTER_GAME_FUNCTION(UbiservicesHTTPSOperationSetServer, "_ZN25UbiservicesHTTPSOperation9SetServerERKSs", void, void*,
                       std::string const&);
REGISTER_GAME_FUNCTION(GameLogicComponentOnLogonAccepted,
                       "_ZN18GameLogicComponent22OnInitialLogonAcceptedEP11VariantList", void, EntityComponent*,
                       VariantList*);
REGISTER_GAME_FUNCTION(GameLogicComponentOnAdd, "_ZN18GameLogicComponent5OnAddEP6Entity", void, EntityComponent*);
REGISTER_GAME_FUNCTION(GetRegionString, "_Z15GetRegionStringv", std::string);
class ServerSwitcher : public patch::BasePatch
{
  public:
    void apply() const override
    {
        auto& game = game::GameHarness::get();

        // We cannot really change it in HTTPComponent, there's some weird allocator ties there that I wasn't able to
        // resolve, but utilizing UbiservicesHTTPSOperation own functions does work, so :shrug:
        // NOTE: Unlike the v3.02 PC counterpart, you WILL need an SSL certificate with matching SANs on sever side. If
        // the server refuses to provide it, you are still forced to use a VPN redirection towards www.growtopia1.com
        RESOLVE_SYMBOL(UbiservicesHTTPSOperationSetServer);
        game.hookFunction<UbiservicesHTTPSOperationDoStart_t>(
            game.resolveSymbol(pattern::UbiservicesHTTPSOperationDoStart), UbiservicesHTTPSOperationDoStart,
            &real::UbiservicesHTTPSOperationDoStart);

        game.hookFunction<OnlineMenuCreate_t>(game.resolveSymbol(pattern::OnlineMenuCreate), OnlineMenuCreate,
                                              &real::OnlineMenuCreate);
        game.hookFunction<OnlineMenuOnConnect_t>(game.resolveSymbol(pattern::OnlineMenuOnConnect), OnlineMenuOnConnect,
                                                 &real::OnlineMenuOnConnect);
    }

    static void UbiservicesHTTPSOperationDoStart(void* this_)
    {
        std::string& uri = *(std::string*)(((int64_t)this_) + 0x80);
        if (uri == "growtopia/server_data.php")
        {
            real::UbiservicesHTTPSOperationSetServer(this_,
                                                     real::GetApp()->GetVar("osgt_qol_server_hostname")->GetString());
            real::LogToConsole(std::string("Using `w" +
                                           real::GetApp()->GetVar("osgt_qol_server_hostname")->GetString() +
                                           "`` as the server data provider...")
                                   .c_str());
        }
        real::UbiservicesHTTPSOperationDoStart(this_);
    }

    static Entity* OnlineMenuCreate(Entity* pGUI)
    {
        // Let game construct online menu itself ready for us
        Entity* pOnlineMenu = real::OnlineMenuCreate(pGUI);

        // OnlineMenu uses this scale for its UI elements
        uint32_t fontID;
        float fontScale;
        real::GetFontAndScaleToFitThisLinesPerScreenY(fontID, fontScale, 18.0f);

        // Take a few elements as anchors to get margins down.
        // We are namely interested in "GrowID: " label for Y coordinate and
        // "text" label for X coordinate.
        float marginY = pOnlineMenu->GetEntityByName("tankid_name_label")->GetVar("pos2d")->GetVector2().y;
        float marginX = pOnlineMenu->GetEntityByName("text")->GetVar("pos2d")->GetVector2().x;

        // Create our very own label
        Entity* pServerLabel = real::CreateTextLabelEntity(pOnlineMenu, "osgt_qol_server_label", marginX,
                                                           marginY - 2.0f, "Server Data Location");
        // We need to call SetupTextEntity so it scales and lines up with the rest of UI.
        real::SetupTextEntity(pServerLabel, fontID, fontScale);

        Variant* pVar = real::GetApp()->GetVar("osgt_qol_server_hostname");
        if (pVar->GetType() != Variant::TYPE_STRING)
            pVar->Set("osgt1.cernodile.com");

        // ..and an accompanying textbox
        // We will append the height of our label to be directly below it for marginY.
        // We will also make the textbox stretch to same length as non-ID name field.
        // The larger InputTextEntity is, the more characters it can fit within its bounds.
        float vSizeX = pOnlineMenu->GetEntityByName("name")->GetVar("size2d")->GetVector2().x;
        vSizeX += pOnlineMenu->GetEntityByName("name_input_box_online")->GetVar("size2d")->GetVector2().x;
        Entity* pServerInput = real::CreateInputTextEntity(
            pOnlineMenu, "osgt_qol_server_input", marginX, marginY + pServerLabel->GetVar("size2d")->GetVector2().y,
            real::GetApp()->GetVar("osgt_qol_server_hostname")->GetString(), vSizeX, 0.0, "", "", "", 0);

        // Make it a bit neater by setting a max length and disallowing going through bounds.
        EntityComponent* pTextRenderComp = pServerInput->GetComponentByName("InputTextRender");
        pTextRenderComp->GetVar("inputLengthMax")->Set(32U);
        pTextRenderComp->GetVar("truncateTextIfNeeded")->Set(1U);
        // InputTextEntity is still an TextEntity, we have to scale it to rest of UI with the helper
        // function.
        real::SetupTextEntity(pServerInput, fontID, fontScale);
        return pOnlineMenu;
    }

    static Entity* OnlineMenuOnConnect(Entity* pEnt)
    {
        // pEnt is normally "Connect" button, but if you enter the game via Enter hotkey, then it
        // will be null. (Is this even a concern on Android?)
        Entity* pServerInput = nullptr;
        if (pEnt)
            pServerInput = pEnt->GetParent()->GetEntityByName("osgt_qol_server_input");
        else
        {
            // We have to call "ByNameRecursively" here because when you launch the game, OnlineMenu
            // goes under "GUI", but when you change resolutions *before* entering the game, it
            // suddenly changes whose the parent and causes game to crash.. kinda weird.
            pServerInput = real::GetApp()
                               ->m_pEntityRoot->GetEntityByNameRecursively("OnlineMenu")
                               ->GetEntityByName("osgt_qol_server_input");
        }

        if (pServerInput != nullptr)
        {
            // If the patch takes too long to load, the element may not even exist.
            std::string userInput = pServerInput->GetComponentByName("InputTextRender")->GetVar("text")->GetString();
            if (userInput.size() != 0)
            {
                real::GetApp()->GetVar("osgt_qol_server_hostname")->Set(userInput);
            }
        }
        return real::OnlineMenuOnConnect(pEnt);
    }
};
REGISTER_USER_GAME_PATCH(ServerSwitcher, server_data_switcher);

class AcceptOlderLogonIdentifier : public patch::BasePatch
{
  public:
    void apply() const override
    {
        auto& game = game::GameHarness::get();
        RESOLVE_SYMBOL(GameLogicComponentOnLogonAccepted);
        game.hookFunction<GameLogicComponentOnAdd_t>(game.resolveSymbol(pattern::GameLogicComponentOnAdd),
                                                     GameLogicComponentOnAdd, &real::GameLogicComponentOnAdd);
    }

    static void GameLogicComponentOnAdd(EntityComponent* this_)
    {
        real::GameLogicComponentOnAdd(this_);
        std::vector<std::string> logonFunctions = {"OnSuperMainStartAcceptLogonFB211131ddf",
                                                   "OnSuperMainStartAcceptLogonFB211131dd",
                                                   "OnSuperMainStartAcceptLogonFB211131d",
                                                   "OnSuperMainStartAcceptLogonFB211131",
                                                   "OnSuperMainStartAcceptLogonFB21113",
                                                   "OnSuperMainStartAcceptLogonFB2111",
                                                   "OnSuperMainStartAcceptLogonFB211",
                                                   "OnSuperMainStartAcceptLogonFB21",
                                                   "OnSuperMainStartAcceptLogonFB2",
                                                   "OnSuperMainStartAcceptLogonFB",
                                                   "OnSuperMainStartAcceptLogonF",
                                                   "OnSuperMainStartAcceptLogon",
                                                   "OnAcceptLogon",
                                                   "OnFirstLogonAccepted",
                                                   "OnLogonAccepted",
                                                   "OnInitialLogonAccepted"};

        for (auto logonFunction : logonFunctions)
        {
            this_->GetShared()
                ->GetFunction(logonFunction)
                ->sig_function.connect(1, boost::bind(real::GameLogicComponentOnLogonAccepted, this_, _1));
        }
    }
};
REGISTER_USER_GAME_PATCH(AcceptOlderLogonIdentifier, accept_old_onsuperlogon);

// Valid 2-letter non-developer flags from PC version (V3.02), sorted alphabetically.
static std::vector<std::string> validRegions = {
    "ad", "ae", "af", "ag", "ai", "al", "am", "an", "ao", "ar", "as", "at", "au", "aw", "ax", "az", "ba", "bb", "bd",
    "be", "bf", "bg", "bh", "bi", "bj", "bm", "bn", "bo", "br", "bs", "bt", "bv", "bw", "by", "bz", "ca", "cc", "cd",
    "cf", "cg", "ch", "ci", "ck", "cl", "cm", "cn", "co", "cr", "cs", "cu", "cv", "cx", "cy", "cz", "de", "dj", "dk",
    "dm", "do", "dz", "ec", "ee", "eg", "eh", "er", "es", "et", "fi", "fj", "fk", "fm", "fo", "fr", "ga", "gb", "gd",
    "ge", "gf", "gh", "gi", "gl", "gm", "gn", "gp", "gq", "gr", "gs", "gt", "gu", "gw", "gy", "hk", "hm", "hn", "hr",
    "ht", "hu", "id", "ie", "il", "in", "io", "iq", "ir", "is", "it", "jm", "jo", "jp", "ke", "kg", "kh", "ki", "km",
    "kn", "kp", "kr", "kw", "ky", "kz", "la", "lb", "lc", "li", "lk", "lr", "ls", "lt", "lu", "lv", "ly", "ma", "mc",
    "md", "me", "mg", "mh", "mk", "ml", "mm", "mn", "mo", "mp", "mq", "mr", "ms", "mt", "mu", "mv", "mw", "mx", "my",
    "mz", "na", "nc", "ne", "nf", "ng", "ni", "nl", "no", "np", "nr", "nu", "nz", "om", "pa", "pe", "pf", "pg", "ph",
    "pk", "pl", "pm", "pn", "pr", "ps", "pt", "pw", "py", "qa", "re", "ro", "rs", "ru", "rw", "sa", "sb", "sc", "sd",
    "se", "sg", "sh", "si", "sj", "sk", "sl", "sm", "sn", "so", "sr", "st", "sv", "sy", "sz", "tc", "td", "tf", "tg",
    "th", "tj", "tk", "tl", "tm", "tn", "to", "tr", "tt", "tv", "tw", "tz", "ua", "ug", "um", "us", "uy", "uz", "va",
    "vc", "ve", "vg", "vi", "vn", "vu", "wf", "ws", "ye", "yt", "za", "zm", "zw"};

class LocaleSwitcher : public patch::BasePatch
{
  public:
    void apply() const override
    {
        auto& game = game::GameHarness::get();

        game.hookFunction<GetRegionString_t>(game.resolveSymbol(pattern::GetRegionString), GetRegionString,
                                             &real::GetRegionString);

        auto& optionsMgr = game::OptionsManager::get();
        optionsMgr.addMultiChoiceOptionDoubleButtons("qol", "System", "osgt_qol_localeswitch_pref", "Locale for flag",
                                                     validRegions, &LocaleSwitcherOnSelect, 80.0f,
                                                     "`5(needs re-login to apply)``");
    }

    static void LocaleSwitcherOnSelect(VariantList* pVariant)
    {
        // Update the multichoice index
        Entity* pClickedEnt = pVariant->Get(1).GetEntity();
        Variant* pOptVar = real::GetApp()->GetVar("osgt_qol_localeswitch_pref");
        uint32_t idx = pOptVar->GetUINT32();
        std::string entName = pClickedEnt->GetName();
        // We are using double-buttons optiondef, we'll use "lower" values to decrement/increment by
        // one and the normal values to skip by first character of country code.
        if (entName == "back_lower")
        {
            if (idx == 0)
                idx = (uint32_t)validRegions.size() - 1;
            else
                idx--;
        }
        else if (entName == "next_lower")
        {
            if (idx >= validRegions.size() - 1)
                idx = 0;
            else
                idx++;
        }
        else if (entName == "back")
        {
            char group = validRegions[idx][0];
            while (true)
            {
                if (idx == 0)
                    idx = (uint32_t)validRegions.size() - 1;
                else
                    idx--;
                if (validRegions[idx][0] != group)
                {
                    break;
                }
            }
        }
        else if (entName == "next")
        {
            char group = validRegions[idx][0];
            while (true)
            {
                if (idx >= validRegions.size() - 1)
                    idx = 0;
                else
                    idx++;
                if (validRegions[idx][0] != group)
                {
                    break;
                }
            }
        }
        pOptVar->Set(idx);
        // Update the option label
        Entity* pTextLabel = pClickedEnt->GetParent()->GetEntityByName("txt");
        real::SetTextEntity(pTextLabel, validRegions[idx]);
    }

    static std::string GetRegionString()
    {
        Variant* pVariant = real::GetApp()->GetVar("osgt_qol_localeswitch_pref");
        if (pVariant->GetType() != Variant::TYPE_UINT32)
        {
            std::string currentRegion = real::GetRegionString();
            if (currentRegion.length() == 5)
                currentRegion = ToLowerCaseString(currentRegion.substr(3, 2));
            auto it = std::find(validRegions.begin(), validRegions.end(), currentRegion);
            if (it != validRegions.end())
                pVariant->Set(uint32_t(std::distance(validRegions.begin(), it)));
            else
                pVariant->Set(225U);
        }
        if (pVariant->GetUINT32() < validRegions.size())
        {
            return "en_" + validRegions[pVariant->GetUINT32()];
        }
        return real::GetRegionString();
    }
};
REGISTER_USER_GAME_PATCH(LocaleSwitcher, locale_switcher);