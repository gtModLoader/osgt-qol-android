#pragma once
#include "game/struct/vec.hpp"
#include <cstdint>
#include <string>
#include <vector>

// Namings matched.
struct GoodieInfo
{
    uint8_t m_minCount;
    uint8_t m_maxCount;
    unsigned short m_itemID;
    float m_chance;
    uint8_t m_flags;
};
// Namings matched.
struct GrowInfo
{
    uint8_t m_seedBaseImageFrame;
    uint8_t m_seedOverlayImageFrame;
    uint8_t m_treeBaseImageFrame;
    uint8_t m_treeOverlayImageFrame;
    unsigned int m_baseColor;
    unsigned int m_overlayColor;
    unsigned short m_seed[2];
    unsigned int m_secondsRequiredToBloom;
    int m_maxFruit;
    uint8_t m_growMultiplier;
    unsigned short m_BountifulSeed;
    float m_BountifulChance;
};
struct PVE_Projectile
{
    int m_projectileType;
    int m_projectileLifetime;
    bool m_blockCollision;
    float m_speed;
    CL_Vec2f m_pos;
    CL_Vec2f m_dir;
    uint32_t m_lastTick;
    uint32_t m_deathTime;
    float m_knockback;
    float m_damage;
    float m_blockDamage;
    int animBaseFrameX;
    void* m_pManager;
};
struct PVE_NPC_Settings
{
    int m_health;
    float m_speed;
    float m_damage;
    float m_blockDamage;
    int m_movementType;
    bool m_patrolVertically;
    bool m_gravity;
    int m_attackType;
    float m_acceleration;
    PVE_Projectile m_rojectile;
    float m_attackAngle;
    bool m_shootOnIdle;
    int m_attackRate;
    int m_knockBack;
    int m_detectionRange;
    int m_knockBackResistance;
    bool m_blockCollision;
    int m_detectionDelay;
    float m_rotation;
    float m_rotationSpeed;
    bool m_immunity;
    int m_damageOnDeath;
    std::vector<CL_Vec2i> m_waypoints;
    int m_targetWaypoing;
    bool m_isLemming;
    float m_damageMod[3];
    float m_resistanceMod[3];
};
struct Equipment
{
    int m_type;
    int m_tier;
    unsigned short m_maxLevel;
    std::vector<int> m_weaponLevels;
};
// Namings matched.
struct ItemInfo
{
    unsigned int m_itemID;
    short m_type;
    int m_material;
    int m_element;
    std::string m_displayName;
    unsigned int m_checksumOfFile;
    std::string m_fileName;
    int m_visualEffect;
    int m_cook;
    uint8_t m_frameX;
    uint8_t m_frameY;
    unsigned int m_imageStorage;
    char m_layer;
    int m_collisionType;
    char m_hp;
    short m_flags;
    int m_flags2;
    int m_clientData[15];
    unsigned short m_serverFlags;
    int m_secondsToHeal;
    unsigned int m_bodyPart;
    GrowInfo m_growInfo;
    short m_rarity;
    uint8_t m_maxCanHold;
    std::string m_extraString;
    int m_extraStringChecksum;
    int m_effectTimeMS;
    std::string m_description;
    unsigned int m_playmodID;
    unsigned int m_minimum_protocol;
    int m_recycleValue;
    std::string m_petName[4];
    GoodieInfo m_goodieInfo[2];
    unsigned int m_antiHackChecksum;
    std::string m_itemEffects[8];
    int m_numEffects;
    int m_renderFx;
    std::string m_animFrames;
    std::string m_animFrames2;
    std::string m_secondaryTexture;
    CL_Vec2f m_layer2Coords;

    bool AutoBreakerActionEnabled;
    int m_lightsourceRange;
    bool m_pveTechParticle;
    int m_pveParticleID;
    int m_pveParticleColorR;
    int m_pveParticleColorG;
    int m_pveParticleColorB;
    int m_pveParticleSpeedX;
    int m_pveParticleSpeedY;
    bool m_pveParticleSoundOn;

    int m_pveParticlePosOffsetX;
    int m_pveParticlePosOffsetY;
    //bool m_pveParticleIsProjectile;

    int m_TileRange;
    int m_PileSize;
    int m_extraSlots[9];
    std::string m_CustomizedPunchInfoStr;

    PVE_NPC_Settings m_pveNpcSettings[3];
    Equipment m_pveInfo;
};
static_assert(sizeof(ItemInfo) == 1136, "ItemInfo struct size mismatch, expected 1136 bytes.");

class ItemInfoManager
{
  public:
    virtual ~ItemInfoManager();
    std::vector<ItemInfo> m_itemInfo;
    uint8_t m_filler[16];

    ItemInfo* GetItemByIDSafe(int ID)
    {
        if (ID < 0 || ID >= m_itemInfo.size())
            return &m_itemInfo[0];
        return &m_itemInfo[ID];
    }

    ItemInfo* GetItemByName(std::string Name)
    {
        for (auto& item : m_itemInfo)
        {
            if (item.m_displayName == Name)
                return &item;
        }
        return &m_itemInfo[0];
    }
};
static_assert(sizeof(ItemInfoManager) == 48, "ItemInfoManager class size mismatch.");