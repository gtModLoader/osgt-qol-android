#pragma once

#include "game/struct/graphics/background.hpp"
#include "worldcamera.hpp"

// Incomplete
class World;
class SurfaceAnim;
class WorldRenderer
{
  public:
    WorldRenderer();
    virtual ~WorldRenderer();

    World* m_pWorld;
    SurfaceAnim *m_shatterCrackImg;
    SurfaceAnim* m_crackImg;
    SurfaceAnim* m_lockImg;
    SurfaceAnim* m_treeImg;
    SurfaceAnim* m_bigRainbowImg;
    SurfaceAnim* m_waterImg;
    SurfaceAnim* m_fireImg;
    SurfaceAnim* m_blueWaterImg;
    SurfaceAnim* m_greenWaterImg;
    SurfaceAnim* m_particles6Img;
    SurfaceAnim* m_CheckOutCounterImg;
    SurfaceAnim* m_PlayingCardImg;
    uint8_t m_steamManager[40];
    WorldCamera m_camera;
    Background* m_pBackground;
    int m_curWeather;
    bool m_bWeatherLock;
    uint8_t m_tempRenderData[10747];
    int m_ownershipStatus;
    int m_screenColor;
    float m_rainbowCycle;
    int m_rainbowColor;
};