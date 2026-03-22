#pragma once

#include "game/struct/vec.hpp"

class WorldRenderer;

// Namings matched.
class WorldCamera
{
  public:
    WorldCamera();
    virtual ~WorldCamera(){};

    WorldRenderer* m_pWorldRenderer;
    CL_Vec2f m_vCamWorldPosUpperLeft; // Current camera position, what the player actually sees.
    CL_Vec2f m_vTarget;               // NetAvatar's position
    CL_Vec2f m_lockPos;
    CL_Vec2f m_vScale;
    CL_Vec2f m_vScreenPos;
    CL_Vec2f m_vScreenSize;
    CL_Vec2f m_vWorldSizeViewableInScreen; // Screen size with zoom level multiplied ontop
    bool m_additiveEnabled;
};