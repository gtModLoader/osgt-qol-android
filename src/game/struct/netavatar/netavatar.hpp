#pragma once
#include "game/struct/component.hpp"
#include "game/struct/vec.hpp"
#include "game/struct/lockfloat.hpp"

// Namings matched.
class NetBase
{
  public:
    virtual ~NetBase(){};
    CL_Vec2f m_vPos;
    CL_Vec2f m_vSize;
    CL_Vec2f m_movementVerify;
    CL_Vec2f m_sizeVerify;
    std::string m_name;
    int m_netID;
    bool m_deleteFlag;
    EntityComponent m_component;
};
static_assert(sizeof(NetBase) == 224, "NetBase class size mismatch.");

// Namings matched.
class NetMoving : public NetBase
{
  public:
    virtual ~NetMoving(){};
    CL_Vec2f m_origin;
    bool m_collidable;
    bool m_bFacingLeft;
    float _moveX;
    float _moveY;

    Rectf GetCollisionRectWorld()
    {
        Rectf res;
        res.left = m_vPos.x;
        res.top = m_vPos.y;
        res.right = m_vPos.x + m_vSize.x;
        res.bottom = m_vPos.y + m_vSize.y;
        return res;
    }
};
static_assert(sizeof(NetMoving) == 248, "NetMoving class size mismatch.");

enum GuildRole : unsigned int
{
    GUILD_ROLE_MEMBER,
    GUILD_ROLE_ELDER,
    GUILD_ROLE_CO_LEADER,
    GUILD_ROLE_LEADER
};
struct GuildClientModel
{
    int guildID;
    int worldID;
    int guildEmblem;
    GuildRole role;
};

class SurfaceAnim;
struct Tile;

// Incomplete.
class NetAvatar : public NetMoving
{
  public:
    virtual ~NetAvatar(){};
    uint8_t pad[68];
    unsigned int m_flags;
    uint8_t pad2[108];
    bool m_bDoubleJumpAvailable;
};