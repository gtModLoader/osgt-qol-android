#pragma once
#include "surface.hpp"

class Background;

enum eCloudMoveType : int
{
    CLOUD_MOVE_DRIFT = 0x0,
    CLOUD_MOVE_STAY = 0x1,
};
enum eCloudTintType : int
{
    CLOUD_TINT_BLUE = 0x0,
    CLOUD_TINT_RED = 0x1,
};

struct Cloud
{
    float x;
    float y;
    float dx;
    float dy; // Unused.
    float distance;
    bool flip;
    unsigned int tint;

    void Init(Background* pParentBG, eCloudMoveType moveType, eCloudTintType tintType);
};

// NOTE: This is not a derivative of Background, but a standalone component.
// Namings matched.
class Background_Clouds
{
  public:
    Background_Clouds(Background* parent, std::string cloudImg, eCloudMoveType moveType, eCloudTintType tintType);
    virtual ~Background_Clouds();
    virtual void Render(CL_Vec2f& camPos, float graphicDetailLevel, float minDistance, float maxDistance);
    virtual void Update();

    void InitClouds(int clouds);

    eCloudMoveType m_moveType;
    eCloudTintType m_tintType;
    SurfaceAnim m_cloud;
    int m_cloudCount = 50;
    Cloud* m_pClouds = nullptr;
    Background* m_pParent;
};
