#pragma once

#include "game/struct/vec.hpp"

class WorldRenderer;

// Namings matched.
class WorldCamera
{
  public:
    WorldCamera();
    virtual ~WorldCamera(){};

    WorldRenderer* m_pWorldRenderer = nullptr;
    CL_Vec2f m_vCamWorldPosUpperLeft{0, 0}; // Current camera position, what the player actually sees.
    CL_Vec2f m_vTarget{0, 0};               // NetAvatar's position
    CL_Vec2f m_lockPos{0, 0};
    CL_Vec2f m_vScale{0, 0};
    CL_Vec2f m_vScreenPos{0, 0};
    CL_Vec2f m_vScreenSize{0, 0};
    CL_Vec2f m_vWorldSizeViewableInScreen{0, 0}; // Screen size with zoom level multiplied ontop
    bool m_additiveEnabled = false;

    CL_Vec2f* WorldToScreen(CL_Vec2f* result, CL_Vec2f vWorldPos)
    {
        CL_Vec2f relativePos = vWorldPos - this->m_vCamWorldPosUpperLeft;

        CL_Vec2f screenPos = relativePos;
        screenPos += this->m_vScreenPos;

        screenPos.x *= this->m_vScale.x;
        screenPos.y *= this->m_vScale.y;

        screenPos.x = static_cast<float>(static_cast<int>(screenPos.x));
        screenPos.y = static_cast<float>(static_cast<int>(screenPos.y));

        *result = screenPos;
        return result;
    }
};