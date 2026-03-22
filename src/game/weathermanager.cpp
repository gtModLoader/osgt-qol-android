#include "game.hpp"
#include "game/struct/graphics/background.hpp"
#include "signatures.hpp"
#include "struct/world/worldrenderer.hpp"

REGISTER_GAME_FUNCTION(WorldRendererForceBackground, "_ZN13WorldRenderer15ForceBackgroundE12eWeatherType", void,
                       uint8_t*, int, void*, void*);

namespace game
{

WeatherManager& WeatherManager::get()
{
    static WeatherManager instance;
    return instance;
}

void game::WeatherManager::initialize()
{
    auto& game = game::GameHarness::get();

    // Hook.
    game.hookFunction<WorldRendererForceBackground_t>(game.resolveSymbol(pattern::WorldRendererForceBackground),
                                                      WorldRendererForceBackground,
                                                      &real::WorldRendererForceBackground);

    auto& events = game::EventsAPI::get();
    events.m_sig_loadFromMem.connect(&game::WeatherManager::refreshItemDB);
}

void game::WeatherManager::refreshItemDB()
{
    auto& weatherMgr = game::WeatherManager::get();
    for (auto item : real::GetApp()->GetItemInfoManager()->m_itemInfo)
    {
        // We only care about weathers
        if (item.m_type != 41)
            continue;
        // Registering weather server-side works by setting alt-path to
        // loader/weather/pretty_name.
        if (item.m_extraString.size() > 0)
        {
            if (item.m_extraString.rfind("loader/weather/", 0) == 0)
            {
                std::string prettyName = item.m_extraString.substr(15);
                auto pair = weatherMgr.weathers.find(prettyName);
                if (pair != weatherMgr.weathers.end())
                {
                    // a match, map it
                    pair->second.mappedID = item.m_effectTimeMS;
                }
            }
        }
    }
}

void game::WeatherManager::registerWeather(std::string prettyName, WeatherCallback pCallback, int weatherID)
{
    CustomWeather weather;
    weather.mappedID = weatherID;
    weather.callback = pCallback;

    auto& weatherMgr = game::WeatherManager::get();
    auto ret = weatherMgr.weathers.insert(make_pair(prettyName, weather));
    CustomWeatherEvent* evt = new CustomWeatherEvent;
    evt->m_prettyName = prettyName;
    evt->m_pCustWeather = &ret.first->second;
    (weatherMgr.m_sig_eventSubscribe)(evt);
    delete evt;
}

void game::WeatherManager::WorldRendererForceBackground(uint8_t* this_, int WeatherID, void* unk3, void* unk4)
{
    auto& weatherMgr = game::WeatherManager::get();

    for (auto pair : weatherMgr.weathers)
    {
        if (WeatherID == pair.second.mappedID)
        {
            WorldRenderer* pRender = reinterpret_cast<WorldRenderer*>(this_);
            if (pRender->m_pBackground != 0)
                delete pRender->m_pBackground;
            Background* pNewBG = pair.second.callback();
            pNewBG->Init(true);
            pRender->m_curWeather = WeatherID;
            pRender->m_pBackground = pNewBG;
            return;
        }
    }
    real::WorldRendererForceBackground(this_, WeatherID, unk3, unk4);
    return;
}

}; // namespace game