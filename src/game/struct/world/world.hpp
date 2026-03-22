#pragma once
#include "boost/signals2.hpp"
#include "game/struct/vec.hpp"
#include <cstdint>
#include <list>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>


enum TileProperties : uint16_t
{
    TILE_PROPERTY_TILE_DATA = 1,
    TILE_PROPERTY_LOCKED = 2 << 0,
    TILE_PROPERTY_SPLICED = 2 << 1,
    TILE_PROPERTY_SEED_DROP = 2 << 2,
    TILE_PROPERTY_IS_TREE = 2 << 3,
    TILE_PROPERTY_FACING_LEFT = 2 << 4,
    TILE_PROPERTY_TOGGLED = 2 << 5,
    TILE_PROPERTY_PUBLIC = 2 << 6,
    TILE_PROPERTY_BGTOGGLE = 2 << 7,
    TILE_PROPERTY_SILENCED = 2 << 8,
    TILE_PROPERTY_WATER = 2 << 9,
    TILE_PROPERTY_GLUE = 2 << 10,
    TILE_PROPERTY_FIRE = 2 << 11,
    TILE_PROPERTY_PAINT_RED = 2 << 12,
    TILE_PROPERTY_PAINT_GREEN = 2 << 13,
    TILE_PROPERTY_PAINT_BLUE = 2 << 14,
    TILE_PROPERTY_PAINT_YELLOW = TILE_PROPERTY_PAINT_RED | TILE_PROPERTY_PAINT_GREEN,
    TILE_PROPERTY_PAINT_PURPLE = TILE_PROPERTY_PAINT_BLUE | TILE_PROPERTY_PAINT_RED,
    TILE_PROPERTY_PAINT_AQUA = TILE_PROPERTY_PAINT_BLUE | TILE_PROPERTY_PAINT_GREEN,
    TILE_PROPERTY_PAINT_BLACK = TILE_PROPERTY_PAINT_BLUE | TILE_PROPERTY_PAINT_GREEN | TILE_PROPERTY_PAINT_RED
};

// Namings matched.
struct Tile
{
    int currentColor;
    short m_itemID;
    short m_itemBGID;
    short m_flags;
    uint8_t x;
    uint8_t y;
    short m_mapIndex;
    short m_visual;
    short m_visualBG;
    bool m_collidable;
    int m_collisionType;
    Rectf m_worldRect;
    void* m_pTileExtra;
    uint8_t m_damage;
    unsigned int m_lastDamageTimer;
    unsigned int m_effectTimer;
    unsigned int pad5;
    float m_lightLevel; // off 68
    uint8_t pad6[84];
};
static_assert(sizeof(Tile) == 152, "Tile size mismatch.");

// Namings matched.
class WorldObject
{
  public:
    virtual ~WorldObject(){};
    CL_Vec2f m_vPos;
    short m_itemID;
    uint8_t m_count;
    uint8_t m_flags;
    unsigned int m_objectID;
    unsigned int m_timeCreated;
    unsigned int m_timeTouched;
};
static_assert(sizeof(WorldObject) == 32, "WorldObject class size mismatch.");

class World;

// Namings matched.
class WorldTileMap
{
  public:
    virtual ~WorldTileMap(){};
    int m_sizeX;
    int m_sizeY;
    short m_firesLit;
    std::vector<Tile> m_tiles;
    World* m_pParent;
};
// Namings matched.
class WorldObjectMap
{
  public:
    virtual ~WorldObjectMap(){};
    unsigned int m_uniqueNumberGen;
    std::list<WorldObject> m_objects;
};

struct DoorInfo
{
    Tile* m_pTile;
};
struct HeartInfo
{
    Tile* m_pTile;
    std::string m_name;
};
struct AutoDeleteInfo
{
    Tile* m_pTile;
    unsigned int m_time;
};
struct BlockSpawnerInfo
{
    Tile* m_pTile;
};

class World
{
  public:
    virtual ~World();
    bool XboxLiveExclusive;
    short m_version;
    boost::signals2::signal<void(Tile&)> TileRemoved;
    boost::signals2::signal<void(Tile&)> TileAdded;
    std::unordered_map<int, std::unordered_map<unsigned int, Tile*>> tilesByItemId;
    WorldTileMap m_tiles;
    WorldObjectMap m_objects;
    std::string m_name;
    unsigned int m_flags;
    short m_baseWeatherType;
    short m_terraformType;
    short m_currentWeatherType;
    short m_unused2;
    unsigned int m_unused3;
    int m_blockInvite;
    unsigned int m_timeLastSolidAdded;
    std::string m_worldBalancerBaseName;
    std::string m_description;
    std::string m_creationDate;
    std::list<DoorInfo> m_doors;
    std::list<HeartInfo> m_heartMonitors;
    std::vector<AutoDeleteInfo> m_autoDeleteBlocks;
    std::vector<BlockSpawnerInfo> m_blockSpawners;
    int m_givingTreeCount;
    int m_steamStompers;
    int m_steamItems;
    int m_silkworms;
    int m_cameras;
    int m_blasters;
    int m_robots;
    int m_StorageBoxExtreme;
    Tile* m_pJammerTile;
    Tile* m_pWorldLockTile;
    int m_lockCount;
    Tile* m_pZombieJammerTile;
    Tile* m_pDropJammerTile;
    Tile* m_pGravityJammerTile;
    Tile *m_pMusicAmplifierTile;
    Tile* m_pConsumableJammerTile;
    Tile* m_pPunchJammerTile;
    Tile* m_pXenoniteTile;
    Tile* m_pFirehouseTile;
    Tile* m_pGhostCharmTile;
    Tile* m_pBalloonJammerTile;
    Tile* m_pArchitectMachineTile;
    Tile* m_pDwarvenDoorTile;
    Tile* m_pDoomsdayWeatherTile;
    Tile *m_pInfinityWeatherMachineTile;
    std::list<Tile*> m_containmentNodes;
    std::vector<std::vector<Tile*>> m_activeNodeGroups;
    std::list<Tile*> m_ArchitectMachineTiles;
    std::list<Tile*> m_DimensionBlockTiles;
    std::list<Tile*> m_DeathTrapWalls;
    std::list<Tile*> lightningClouds;
    std::list<Tile*> checkoutCounters;
    std::list<Tile*> m_PlayingCards;
    std::list<Tile*> m_otherWorldlyWarningLights;
    std::list<Tile*> m_starshipComponents;
    std::list<Tile*> m_starshipHull;
    std::list<Tile*> m_autoActionBlocks;
    std::list<Tile *> m_PressingPads;
    std::list<Tile*> m_DunckingBuckets;
    std::set<Tile*> m_Tricksters;
    Tile* m_pDataTile;
    Tile* m_pMainDoor;
    bool m_IsTilesDirty;
    unsigned int m_TilesDirtyVersion;
    bool m_magplantCountDirty;
    int m_magplantCount;
    Tile *m_pPVEBossSwitch;
    std::list<Tile *> m_PVENPCTiles;
    std::list<Tile *> m_AnzuTiles;
    Tile *m_pAirshipBaseTile;
    bool m_mapLoaded;
    Tile *starDataTile;
};
static_assert(sizeof(World) == 888, "World class size mismatch.");