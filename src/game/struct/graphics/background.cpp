#include "background.hpp"
#include "game/signatures.hpp"

#include "boost/bind/bind.hpp"
#include "game/struct/renderutils.hpp"
#include "game/struct/variant.hpp"
#include "game/struct/vec.hpp"

Background::Background()
{
    real::GetApp()->m_sig_enterforeground.connect(1, boost::bind(&Background::OnEnterForeground, this, _1));
    real::GetApp()->m_sig_enterbackground.connect(1, boost::bind(&Background::OnEnterBackground, this, _1));
}

Background::~Background() {}

void Background::Render(CL_Vec2f& camPos, float graphicDetailLevel)
{
    // Need to find out what this vec is for. Looks like it's related to some bgfx rotation matrix?
    CL_Vec2f unk4(0.0, 0.0);
    // Get our screen size
    Rectf screenRect = GetScreenRect();
    // Draw the default backdrop (same as Background_Default)
    real::DrawFilledRect(screenRect, MAKE_RGBA(96, 215, 242, 255), 0.0f, &unk4);
    // Draw the weather change fade effect.
    if (graphicDetailLevel > 0.1 && m_flashAlpha > 0.0f)
    {
        real::DrawFilledRect(screenRect, -0x100 - (int)(m_flashAlpha * -255.0), 0.0f, &unk4);
        m_flashAlpha = m_flashAlpha - (real::GetApp()->GetDeltaTick() / 1000.0f);
    }
}

void Background::Init(bool useMap) { return; }

void Background::Update() { return; }

void Background::RenderForeground(CL_Vec2f& camPos, float graphicDetailLevel) { return; }

void Background::SetScale(CL_Vec2f& vScale)
{
    if (m_scaleOnTheFly)
    {
        m_scale.x = vScale.x;
        m_scale.y = vScale.y;
    }
}

void Background::UpdateSFXVolume(float sfxVol)
{
    // m_loopingSound.SetVolume(sfxVol * m_sfxBaseVolume *
    // real::GetApp()->GetVar("music_vol")->GetFloat());
}

void Background::OnEnterForeground(VariantList* pVList)
{
    // m_loopingSound.SetDisabled(0);
}

void Background::OnEnterBackground(VariantList* pVList)
{
    // m_loopingSound.SetDisabled(1);
}

void Background::OnAudioEnabled()
{
    // m_loopingSound.SetDisabled(0);
}

void Background::OnAudioDisabled()
{
    // m_loopingSound.SetDisabled(1);
}