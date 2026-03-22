#include "game/game.hpp"
#include "utils/utils.hpp"

REGISTER_GAME_FUNCTION(NetControllerLocalOnArcadeInput, "_ZN18NetControllerLocal13OnArcadeInputEib", void, void* this_,
                       int keyCode, bool bKeyFired);
REGISTER_GAME_FUNCTION(OnArcadeInput, "_Z13OnArcadeInputP11VariantList", void, VariantList*);
REGISTER_GAME_FUNCTION(AddWASDKeys, "_Z11AddWASDKeysv", void);
REGISTER_GAME_FUNCTION(GetArcadeComponent, "_Z18GetArcadeComponentv", EntityComponent*);
REGISTER_GAME_FUNCTION(AddKeyBinding, "_Z13AddKeyBindingP15EntityComponentSsjjbj", void, EntityComponent* pComp,
                       std::string name, uint32_t inputcode, uint32_t outputcode, bool bAlsoSendAsNormalRawKey,
                       uint32_t modifiersRequired);
REGISTER_GAME_FUNCTION(ItemInfoManagerLoadFromMem, "_ZN15ItemInfoManager11LoadFromMemEPhb", void, void*, char*, bool);
REGISTER_GAME_FUNCTION(OnMapLoaded, "_ZN13WorldRenderer11OnMapLoadedEv", void, void*, int64_t, int64_t, int64_t);
REGISTER_GAME_FUNCTION(WorldRendererOnRender, "_ZN13WorldRenderer8OnRenderE7CL_Vec2IfE", void, void*, CL_Vec2f*);

namespace game
{

EventsAPI& EventsAPI::get()
{
    static EventsAPI instance;
    return instance;
}

void game::EventsAPI::initialize()
{
    auto& game = game::GameHarness::get();
    RESOLVE_SYMBOL(GetArcadeComponent);
    RESOLVE_SYMBOL(AddKeyBinding);

    game.hookFunction<NetControllerLocalOnArcadeInput_t>(game.resolveSymbol(pattern::NetControllerLocalOnArcadeInput),
                                                         NetControllerLocalOnArcadeInput,
                                                         &real::NetControllerLocalOnArcadeInput);
    game.hookFunction<OnArcadeInput_t>(game.resolveSymbol(pattern::OnArcadeInput), OnArcadeInput, &real::OnArcadeInput);
    game.hookFunction<AddWASDKeys_t>(game.resolveSymbol(pattern::AddWASDKeys), AddWASDKeys, &real::AddWASDKeys);
    game.hookFunction<ItemInfoManagerLoadFromMem_t>(game.resolveSymbol(pattern::ItemInfoManagerLoadFromMem),
                                                    ItemInfoManagerLoadFromMem, &real::ItemInfoManagerLoadFromMem);
    game.hookFunction<OnMapLoaded_t>(game.resolveSymbol(pattern::OnMapLoaded), OnMapLoaded, &real::OnMapLoaded);
    game.hookFunction<WorldRendererOnRender_t>(game.resolveSymbol(pattern::WorldRendererOnRender),
                                               WorldRendererOnRender, &real::WorldRendererOnRender);

    m_lastKeycode = 600000;
}

int game::EventsAPI::acquireKeycode() { return m_lastKeycode++; }

void game::EventsAPI::ItemInfoManagerLoadFromMem(void* this_, char* pBytes, bool arg3)
{
    real::ItemInfoManagerLoadFromMem(this_, pBytes, arg3);
    auto& EventsAPI = game::EventsAPI::get();
    (EventsAPI.m_sig_loadFromMem)();
}
void game::EventsAPI::OnArcadeInput(VariantList* pVL)
{
    real::OnArcadeInput(pVL);
    auto& EventsAPI = game::EventsAPI::get();
    (EventsAPI.m_sig_onArcadeInput)(pVL);
}
void game::EventsAPI::NetControllerLocalOnArcadeInput(void* this_, int keyCode, bool bKeyFired)
{
    real::NetControllerLocalOnArcadeInput(this_, keyCode, bKeyFired);
    auto& EventsAPI = game::EventsAPI::get();
    (EventsAPI.m_sig_netControllerInput)(this_, keyCode, bKeyFired);
}
void game::EventsAPI::AddWASDKeys()
{
    real::AddWASDKeys();
    auto& EventsAPI = game::EventsAPI::get();
    (EventsAPI.m_sig_addWasdKeys)();
}
void game::EventsAPI::OnMapLoaded(void* this_, int64_t p1, int64_t p2, int64_t p3)
{
    real::OnMapLoaded(this_, p1, p2, p3);
    auto& EventsAPI = game::EventsAPI::get();
    (EventsAPI.m_sig_onMapLoaded)(this_, p1, p2, p3);
}
void game::EventsAPI::WorldRendererOnRender(void* this_, CL_Vec2f* p2)
{
    real::WorldRendererOnRender(this_, p2);
    auto& EventsAPI = game::EventsAPI::get();
    (EventsAPI.m_sig_worldRendererOnRender)(this_, p2);
}

}; // namespace game