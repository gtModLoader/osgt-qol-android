#pragma once
#include "game/struct/component.hpp"
#include "game/struct/playeritems.hpp"
#include "game/struct/world/world.hpp"
#include "game/struct/world/worldrenderer.hpp"

class GameLogicComponent : public EntityComponent
{
    public:
        WorldTileMap* GetTileMap() { return &m_pWorld->m_tiles; }

        int GetTileWidth() { return GetTileMap()->m_sizeX; }
        int GetTileHeight() { return GetTileMap()->m_sizeY; }
        uint8_t pad[70];
        World* m_pWorld;
        WorldRenderer* m_pWorldRenderer;
        uint8_t pad2[304];
        PlayerItems m_playerItems;
        uint8_t pad3[632];
};
static_assert(sizeof(GameLogicComponent) == 1280, "GameLogicComponent class size mismatch.");