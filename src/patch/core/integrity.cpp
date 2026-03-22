#include "game/game.hpp"
#include "patch/patch.hpp"

#include "game/struct/net/gameupdatepacket.hpp"

// DownloadFileComponent::Init
REGISTER_GAME_FUNCTION(DownloadFileComponentInit, "_ZN21DownloadFileComponent4InitESsSsSs", void, void*, std::string,
                       void*, void*)

// GameLogicComponent::HandleBattleEvent
REGISTER_GAME_FUNCTION(GameLogicComponentHandleBattleEvent,
                       "_ZN18GameLogicComponent17HandleBattleEventEP16GameUpdatePacket", void, void*,
                       GameUpdatePacket*);

class SecureItemManager : public patch::BasePatch
{
  public:
    void apply() const override
    {
        auto& game = game::GameHarness::get();
        game.hookFunctionPattern<DownloadFileComponentInit_t>(
            pattern::DownloadFileComponentInit, DownloadFileComponentInit, &real::DownloadFileComponentInit);
    }

    static void DownloadFileComponentInit(void* this_, std::string filePath, void* unk3, void* unk4)
    {
        // Prevent path traversal by strictly disallowing usage of ".." in file paths.
        // We do not need to sanitize for / \ : characters as absolute cache path already gets
        // prepended to the file name, so attacking via absolute path isn't going to work.
        // This attack vector is commonly used by malicious servers in order to drop malware to
        // end-user's computer.
        if (filePath.find("..") != std::string::npos)
        {
            std::string msg = "The server you were connecting to contained a malicious item definitions file. "
                              "Shutting down game to prevent further damages. Offending filename: " +
                              filePath;
            LOG_ERROR("!!!! FATAL ABORT - SECURITY BREACH ATTEMPTED !!!!\n%s", msg.c_str());
            exit(1);
            return;
        }
        real::DownloadFileComponentInit(this_, filePath, unk3, unk4);
    }
};
REGISTER_CORE_GAME_PATCH(SecureItemManager, secure_item_manager);

static const std::string g_battleEventErrorMessage =
    "Server sent a malformed BattleEvent packet, this could be an attempt "
    "at Remote Code Execution. OSGT-QOL has shut down the game to prevent "
    "any further exploitation. If you are a server developer and believe "
    "this is sent in error, please open an issue on GitHub and provide the "
    "raw packet sent to the client. Error code: ";
class SecureBattleEvent : public patch::BasePatch
{
  public:
    void apply() const override
    {
        auto& game = game::GameHarness::get();
        game.hookFunctionPattern<GameLogicComponentHandleBattleEvent_t>(pattern::GameLogicComponentHandleBattleEvent,
                                                                        GameLogicComponentHandleBattleEvent,
                                                                        &real::GameLogicComponentHandleBattleEvent);
    }

    static void GameLogicComponentHandleBattleEvent(void* this_, GameUpdatePacket* packet)
    {
        // Battle Events are made rather poorly by the game and leaves space for exploitation.
        // For one, event subtype 0 does no proper bounds checking and allows to read past packet
        // bounds. There may not be really that much attack surface here, but it's good to have it
        // patched.
        // For second, more damning, event subtype 3 allows you to point to any arbitrary
        // point in memory with the team ID due to no safety rails in its packet handling code,
        // allowing a threat actor to arbitrarily write memory wherever they please relative to
        // m_battleStatus in GameLogicComponent.
        if (packet->x == 0)
        {
            if (packet->extendedDataSize != 0)
            {
                uint8_t* packetExtraData = ((uint8_t*)(packet)) + 56;
                short nameLen = *(short*)(packetExtraData);
                packetExtraData += nameLen + 6; // Skip name and flags
                uint8_t m_teams = *packetExtraData;
                // The game only has 4 teams it registers memory for, rest could be an attempt at
                // reading past bounds.
                if (m_teams > 4)
                {
                    LOG_ERROR("!!!! FATAL ABORT - SECURITY BREACH ATTEMPTED !!!!\n%s",
                              (g_battleEventErrorMessage + "0").c_str());
                    exit(1);
                    return;
                }
                // Calculate packet length
                int packetLen = nameLen + 6 + 1 + (m_teams * 4);
                if (packetLen != packet->extendedDataSize)
                {
                    LOG_ERROR("!!!! FATAL ABORT - SECURITY BREACH ATTEMPTED !!!!\n%s",
                              (g_battleEventErrorMessage + "1").c_str());
                    exit(1);
                    return;
                }
            }
        }
        if (packet->x == 3)
        {
            // Prevent out-of-bounds writes, the game only has space for 5 teams in BattleStatus
            // struct.
            if (packet->m_parm1 < 0 || packet->m_parm1 > 3)
            {
                auto& game = game::GameHarness::get();
                LOG_ERROR("!!!! FATAL ABORT - SECURITY BREACH ATTEMPTED !!!!\n%s",
                          (g_battleEventErrorMessage + "2").c_str());
                exit(1);
                return;
            }
        }
        real::GameLogicComponentHandleBattleEvent(this_, packet);
    }
};
REGISTER_CORE_GAME_PATCH(SecureBattleEvent, secure_battle_event);