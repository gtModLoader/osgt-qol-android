#pragma once
#include "game/struct/enetclient.hpp"
#include "game/struct/baseapp.hpp"
#include "game/struct/iteminfomanager.hpp"
#include "game/struct/variantdb.hpp"


class GameLogicComponent;
class App : public BaseApp
{
  public:
    virtual ~App();

    GameLogicComponent* GetGameLogic() { return m_pGameLogicComponent; }
    ItemInfoManager* GetItemInfoManager() { return &m_itemInfoManager; }
    void* GetConsole() { return &m_console; }
    void* GetClient() { return m_pClient; }

    Variant* GetVar(std::string varName) { return m_sharedDB.GetVar(varName); }

    GameLogicComponent* m_pGameLogicComponent;
    void* m_pTrackHandler; // TrackHandlerComponent*
    ItemInfoManager m_itemInfoManager;
    uint8_t pad[736];
    ENetClient* m_pClient;
    uint8_t pad2[176];
    VariantDB m_sharedDB; // 3552
};