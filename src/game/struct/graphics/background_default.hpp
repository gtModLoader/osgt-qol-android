#pragma once
#include "background.hpp"
#include "background_clouds.hpp"
#include "surface.hpp"


struct AnzuChannelHolder
{

};
struct AnzuBackgroundModule
{
     std::vector<AnzuChannelHolder> m_billboards0;
     std::vector<AnzuChannelHolder> m_billboards1;
     std::vector<AnzuChannelHolder> m_billboards2;
     std::vector<std::vector<AnzuChannelHolder>> m_billboards;
     std::vector<AnzuChannelHolder> m_blimps;
     std::vector<AnzuChannelHolder> m_planeBanners;
     std::vector<int> m_planeTimers;
     std::vector<int> m_planeIntervals;
     std::vector<AnzuChannelHolder> m_visibleRects;
     bool m_drawBillboards;
     bool m_drawBlimps;
     std::vector<float> m_blimpOffsets;
     std::vector<float> m_blimpSpeeds;
     bool m_drawPlanes;
     std::vector<float> m_planeOffsets;
     std::vector<float> m_planeSpeeds;
};
// Namings matched.
class Background_Default : public Background
{
  public:
    Background_Default();
    virtual ~Background_Default();

    virtual void Render(CL_Vec2f& camPos, float graphicDetailLevel);
    virtual void Init(bool useMap);
    virtual void Update();

    void DrawHill(int n, CL_Vec2f camPos);

    bool m_springInstead;
    Background_Clouds* m_pClouds;
    SurfaceAnim m_sun;
    Surface* m_pHills;

    AnzuBackgroundModule m_anzuModule;
};
static_assert(sizeof(Background_Default) == 600, "BG_default mismatch");