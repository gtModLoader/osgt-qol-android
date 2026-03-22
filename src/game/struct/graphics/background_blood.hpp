#pragma once
#include "background.hpp"
#include "background_clouds.hpp"
#include "surface.hpp"

// Clone of Background_Default tinted with red and a moon sprite for Proof-of-Concept on custom
// weathers.
class Background_Blood : public Background
{
  public:
    Background_Blood();
    virtual ~Background_Blood();

    virtual void Render(CL_Vec2f& camPos, float graphicDetailLevel);
    virtual void Init(bool useMap);
    virtual void Update();

    void DrawHill(int n, CL_Vec2f camPos);

    Background_Clouds* m_pClouds = nullptr;
    SurfaceAnim m_moon;
    Surface* m_pHills;
};
