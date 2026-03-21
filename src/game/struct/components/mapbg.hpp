#pragma once
#include "game/struct/component.hpp"
#include "game/struct/graphics/background.hpp"

// Expected size: 264 bytes
// Namings matched.
class MapBGComponent : public EntityComponent
{
  public:
    virtual ~MapBGComponent();
    virtual void OnAdd(Entity* pEnt);
    virtual void OnRemove();

    CL_Vec2f* m_pPos2d;
    unsigned int* m_pColor;
    unsigned int* m_pColorMod;
    float* m_pAlpha;
    Background* m_pBackground;
    float m_mapPos;
};
static_assert(sizeof(MapBGComponent) == 216, "MapBGComponent class size mismatch.");