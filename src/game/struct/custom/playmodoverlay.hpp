#include "game/struct/entity.hpp"
#include "game/struct/miscutils.hpp"
#include "game/struct/variant.hpp"
#include <cstdint>


#pragma pack(push, 1)
enum PlaymodPacketAction : uint8_t
{
    PLAYMOD_PROTOCOL_NO_OP,
    PLAYMOD_PROTOCOL_ADD_OR_REMOVE,
    PLAYMOD_PROTOCOL_SEND_BULK,
    PLAYMOD_PROTOCOL_UPDATE_ICON_DATA
};
struct PlaymodPacketDataPlaymodInfo
{
    short PlaymodID = -1;
    int Duration = 0;
};
struct PlaymodPacketDataIconInfo
{
    short PlaymodID = -1;
    short ItemID = 0;
};
#pragma pack(pop)

class PlaymodTimerEntity : public Entity
{
  public:
    PlaymodTimerEntity()
    {
        SetName("PlaymodTimerEntity");
        GetFunction("OnUpdate")->sig_function.connect(1, boost::bind(&PlaymodTimerEntity::OnUpdate, this, _1));
    }
    std::string m_trackableStr;
    time_t m_killstamp = 0;
    time_t m_lastUpdate = 0;
    int m_trackableID = -1;
    bool m_bInited = false;
    bool m_bKillable = false;

    void SetTimer(time_t stamp)
    {
        m_killstamp = time(0) + stamp;
        m_bInited = true;
    }

    void SetTrackableID(int ID) { m_trackableID = ID; }
    void SetTrackableString(std::string PlaymodName) { m_trackableStr = PlaymodName; }

    void OnUpdate(VariantList*)
    {
        if (!m_bInited || m_bKillable || m_lastUpdate >= time(0))
            return;
        if (m_killstamp <= time(0))
            m_bKillable = true;
        Variant* pTextVar = GetEntityByName("txt")->GetComponentByName("TextRender")->GetVar("text");
        int remainder = (int)(m_killstamp - time(0));
        if (remainder < 0)
            remainder = 0;
        std::string txt = IntToTimeSeconds(remainder, false, false);
        if (remainder < 60)
            txt = "`4" + txt;
        pTextVar->Set(txt);
        m_lastUpdate = time(0);
    }
};

struct ActiveMod
{
    time_t m_killstamp = 0;
    int m_iconID = 0;
    int m_playmodID = -1;
    std::string m_strModName = "";
    bool m_bDelayLoad = false;

    bool operator<(ActiveMod const& other) { return m_killstamp > other.m_killstamp; }
};