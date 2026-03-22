#include "background_clouds.hpp"
#include "background.hpp"
#include "game/signatures.hpp"
#include "game/struct/renderutils.hpp"
#include "game/struct/vec.hpp"

#include <string>

// This should ideally move to a different place, but since it's the only consumer right now, it's
// here.
float RandomRangeFloat(float rangeMin, float rangeMax)
{
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = rangeMax - rangeMin;
    float r = random * diff;
    return rangeMin + r;
}

void Cloud::Init(Background* parent, eCloudMoveType moveType, eCloudTintType tintType)
{
    this->distance = RandomRangeFloat(0.1f, 1.0f);
    this->flip = RandomRangeFloat(0.0f, 1.0f) < 0.5f;

    this->x = RandomRangeFloat(0.0f, (parent->m_worldRect.right - parent->m_worldRect.left) / this->distance);
    this->x += parent->m_worldRect.left / this->distance;
    this->y = RandomRangeFloat(0.0f, ((parent->m_worldRect.bottom - parent->m_worldRect.top) * 0.5f) / this->distance);
    this->y += parent->m_worldRect.top / this->distance;

    if (moveType == CLOUD_MOVE_DRIFT)
        this->dx = RandomRangeFloat(-0.05f, -1.0f);
    else if (moveType == CLOUD_MOVE_STAY)
        this->dx = 0; // I don't know either. Maybe something scrapped.

    int r = 0;
    int g = 0;
    int b = 0;
    if (tintType == CLOUD_TINT_BLUE)
    {
        b = (int)(this->distance * 13.0f) + 242;
        g = (int)(this->distance * 40.0f) + 215;
        r = (int)(this->distance * 159.0f) + 96;
    }
    else if (tintType == CLOUD_TINT_RED)
    {
        r = (int)(this->distance * 55.0f) + 200;
        g = (int)(this->distance * 255.0f);
        b = g;
    }
    this->tint = MAKE_RGBA(r, g, b, 255);
}

Background_Clouds::Background_Clouds(Background* parent, std::string cloudImg, eCloudMoveType moveType,
                                     eCloudTintType tintType)
{
    m_moveType = moveType;
    m_tintType = tintType;
    m_cloudCount = 50;
    m_pParent = parent;
    m_pClouds = 0;

    m_cloud.SetBlendingMode(Surface::BLENDING_PREMULTIPLIED_ALPHA);
    m_cloud.LoadFile(cloudImg); // This should use GetSmartFile.
    m_cloud.SetupAnim(1, 1);
}

Background_Clouds::~Background_Clouds()
{
    if (m_pClouds != 0)
    {
        delete[] m_pClouds;
    }
}

void Background_Clouds::InitClouds(int clouds)
{
    m_cloudCount = clouds;
    if (m_pClouds != 0)
    {
        delete[] m_pClouds;
        m_pClouds = 0;
    }
    m_pClouds = new Cloud[clouds];
    if (m_cloudCount > 0)
    {
        int i = 0;
        while (i < m_cloudCount)
        {
            m_pClouds[i].Init(m_pParent, m_moveType, m_tintType);
            i++;
        }
    }
}

void Background_Clouds::Update()
{
    if (!m_pClouds || m_cloudCount == 0)
        return;
    int i = 0;
    while (i < m_cloudCount)
    {
        Cloud* pCloud = &m_pClouds[i];
        pCloud->x = (real::GetApp()->GetDelta() * pCloud->dx) + pCloud->x;
        // Define our boundaries
        // For left side, we have to factor in the cloud surface size and the distance its at.
        float boundLeftX =
            (m_pParent->m_worldRect.left - (m_pParent->m_scale.x * 197.0f) * pCloud->distance) / pCloud->distance;
        // For right side, even though we never move cloud there, it may get stuck on too far right
        // end or cloud may initialize too far right.
        float boundRightX =
            ((m_pParent->m_worldRect.right - m_pParent->m_worldRect.left) * 1.4f + m_pParent->m_worldRect.left) /
            pCloud->distance;
        if (pCloud->x < boundLeftX || pCloud->x > boundRightX)
        {
            // Reset the cloud and it's X coordinate to right side.
            pCloud->Init(m_pParent, m_moveType, m_tintType);
            // UNMATCHING behaviour - this shouldn't be needed!
            pCloud->x =
                ((m_pParent->m_worldRect.right - m_pParent->m_worldRect.left) * 1.2f + m_pParent->m_worldRect.left) /
                pCloud->distance;
        }
        i++;
    }
}

void Background_Clouds::Render(CL_Vec2f& camPos, float graphicDetailLevel, float minDistance, float maxDistance)
{
    // We don't really render clouds if we have none or are using low detail.
    if (!m_pClouds || m_cloudCount == 0 || graphicDetailLevel < 0.4)
        return;
    int i = -1;
    while (++i < m_cloudCount)
    {
        Cloud* pCloud = &m_pClouds[i];
        if (minDistance <= pCloud->distance)
        {
            // Game normally does a return; here because it sorts clouds prior.
            if (maxDistance <= pCloud->distance)
                continue;
            // Scale the cloud relative to their distance.
            CL_Vec2f vScale;
            vScale.x = pCloud->distance * m_pParent->m_scale.x;
            vScale.y = pCloud->distance * m_pParent->m_scale.y;
            // When the game is on high graphic detail, it'll tint the base colour white and apply
            // appropriate opacity.
            if (0.8 < graphicDetailLevel)
                pCloud->tint = -0x100 - (int)(pCloud->distance * -255.0);
            m_cloud.BlitScaledAnim((pCloud->distance * pCloud->x) - camPos.x, (pCloud->distance * pCloud->y) - camPos.y,
                                   0, 0, &vScale, 0, pCloud->tint, 0, 0, pCloud->flip);
        }
    }
}