#pragma once

#include "game/struct/vec.hpp"
#include <stdint.h>

// Namings matched.
struct GameUpdatePacket
{
    uint8_t packetType;
    uint8_t m_objectFlags;
    uint8_t byte_remove;
    uint8_t byte_add;
    int m_destinationNetID;
    int m_enemyPlayerID;
    unsigned int m_flags;
    float m_rotation;
    int m_parm1;
    CL_Vec2f pos;
    CL_Vec2f speed;
    float rotation;
    int x;
    int y;
    int extendedDataSize;
};
