#include "background_blood.hpp"
#include "game/signatures.hpp"
#include "game/struct/components/gamelogic.hpp"
#include "game/struct/graphics/background_clouds.hpp"
#include "game/struct/graphics/surface.hpp"
#include "game/struct/renderutils.hpp"
#include "game/struct/vec.hpp"

#include <string>

Background_Blood::Background_Blood()
{
    // Load surfs and setup them.
    m_moon.SetBlendingMode(Surface::BLENDING_PREMULTIPLIED_ALPHA);
    m_moon.LoadFile("game/moon.rttex");
    m_moon.SetupAnim(1, 1);

    m_pHills = new Surface[3]();

    m_pHills[0].SetBlendingMode(Surface::BLENDING_PREMULTIPLIED_ALPHA);
    m_pHills[1].SetBlendingMode(Surface::BLENDING_PREMULTIPLIED_ALPHA);
    m_pHills[2].SetBlendingMode(Surface::BLENDING_PREMULTIPLIED_ALPHA);

    m_pHills[0].LoadFile("game/hills3.rttex");
    m_pHills[1].LoadFile("game/hills2.rttex");
    m_pHills[2].LoadFile("game/hills1.rttex");

    m_pClouds = new Background_Clouds((Background*)this, "game/cloud.rttex", CLOUD_MOVE_DRIFT, CLOUD_TINT_BLUE);
}

Background_Blood::~Background_Blood()
{
    delete m_pClouds;
    delete[] m_pHills;
}

void Background_Blood::Render(CL_Vec2f& camPos, float graphicDetailLevel)
{
    // Need to find out what this vec is for. Looks like it's related to some bgfx rotation matrix?
    CL_Vec2f unk4(0.0, 0.0);
    unsigned int skyColour = MAKE_RGBA(90, 10, 10, 255);
    // Get our screen size
    Rectf screenRect = GetScreenRect();
    // Draw the backdrop
    real::DrawFilledRect(screenRect, skyColour, 0.0f, &unk4);
    if (graphicDetailLevel > 0.1)
    {
        // Draw all the "high detail" assets, e.g. sun/hills/clouds.
        CL_Vec2f moonScale = CL_Vec2f((m_areaSize.x / 161.0f) / 6.0f);
        m_moon.BlitScaledAnim(m_areaSize.x * 0.7f, m_areaSize.y * 0.1f, 0, 0, &moonScale, 0);
        // Draw clouds piecemeal e.g. 0-20%, 20-40% to be between hill layers.
        m_pClouds->Render(camPos, graphicDetailLevel, 0.0f, 0.2f);
        DrawHill(0, camPos);
        m_pClouds->Render(camPos, graphicDetailLevel, 0.2f, 0.4f);
        DrawHill(1, camPos);
        m_pClouds->Render(camPos, graphicDetailLevel, 0.4f, 0.6f);
        DrawHill(2, camPos);
        m_pClouds->Render(camPos, graphicDetailLevel, 0.6f, 1.0f);
        // Draw the weather change fade effect.
        if (m_flashAlpha > 0.0f)
        {
            real::DrawFilledRect(screenRect, -0x100 - (int)(m_flashAlpha * -255.0), 0.0f, &unk4);
            m_flashAlpha = m_flashAlpha - (real::GetApp()->GetDeltaTick() / 1000.0f);
        }
    }
}

void Background_Blood::Init(bool useMap)
{
    if (!useMap)
    {
        m_worldRect = GetScreenRect();
        m_pClouds->InitClouds(20);
    }
    else
    {
        m_worldRect.bottom = real::GetApp()->GetGameLogic()->GetTileHeight() * 32.0f;
        m_worldRect.right = real::GetApp()->GetGameLogic()->GetTileWidth() * 32.0f;
        m_pClouds->InitClouds(50);
    }
}

void Background_Blood::Update()
{
    m_pClouds->Update();
    return;
}

void Background_Blood::DrawHill(int n, CL_Vec2f camPos)
{
    // Scale proportional to the user's screen width
    float hillScale = m_areaSize.x / 512.0f;

    // Use the passed hill level to determine parallax scrolling speed and progress.
    float parallaxStep = (float)(n + 1) * -0.2f;
    float parallaxScroll = floorf((float)(parallaxStep * camPos.x) * m_scale.x);

    // Determine X location after scrolling
    float x = 0.0f;
    do
    {
        x = parallaxScroll;
        parallaxScroll = x + m_areaSize.x;
    } while (parallaxScroll < 0.0);

    // Fix the hill heights. The game fixes these 2.0f above.
    float hillHeight = m_areaSize.y + 2.0f;
    float y = hillHeight;
    if (n == 0)
        y = hillHeight - hillScale * 46.0f;
    else if (n == 1)
        y = hillScale * 60.0f + hillHeight;

    unsigned int RGBA = MAKE_RGBA(255, 40, 40, 255);
    for (; x <= m_areaSize.x; x = x + hillScale * 512.0f)
    {
        CL_Vec2f vScale(hillScale);
        m_pHills[n].BlitScaled(x, y, vScale, 4, RGBA, 0.0, NULL, false, false);
    }
}