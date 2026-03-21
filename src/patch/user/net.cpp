#include "game/game.hpp"
#include "game/signatures.hpp"
#include "game/struct/variant.hpp"
#include "patch/patch.hpp"
#include "game/struct/entity.hpp"

REGISTER_GAME_FUNCTION(OnlineMenuCreate, "_Z16OnlineMenuCreateP6Entity", Entity*, Entity*);
REGISTER_GAME_FUNCTION(OnlineMenuOnConnect, "_Z19OnlineMenuOnConnectP6Entity", Entity*, Entity*);
REGISTER_GAME_FUNCTION(Curl_getaddrinfo_ex, "Curl_getaddrinfo_ex", int, const char* host, const char* service,
                       void* hints, void** res);
REGISTER_GAME_FUNCTION(UbiservicesHTTPSOperationDoStart, "_ZN25UbiservicesHTTPSOperation7DoStartEv", void, void*);
REGISTER_GAME_FUNCTION(UbiservicesHTTPSOperationSetServer, "_ZN25UbiservicesHTTPSOperation9SetServerERKSs", void, void*,
                       void*);
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
        UbiString* uri = (UbiString*)(((int64_t)this_) + 0x80);
        if (uri->getString() == "growtopia/server_data.php")
        {
            real::UbiservicesHTTPSOperationSetServer(
                this_, UbiString(real::GetApp()->GetVar("osgt_qol_server_hostname")->GetString()));
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
        Entity* pServerLabel = real::CreateTextLabelEntity(pOnlineMenu, UbiString("osgt_qol_server_label"), marginX,
                                                           marginY - 2.0f, UbiString("Server Data Location"));
        // We need to call SetupTextEntity so it scales and lines up with the rest of UI.
        real::SetupTextEntity(pServerLabel, fontID, fontScale);

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
            LOG_DEBUG("found pServerInput");
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