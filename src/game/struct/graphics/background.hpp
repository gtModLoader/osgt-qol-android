#pragma once
#include "game/struct/variant.hpp"
#include "game/struct/vec.hpp"

// Namings matched.
class Background
{
  public:
    Background();
    virtual ~Background();

    virtual void Render(CL_Vec2f& camPos, float graphicDetailLevel);
    virtual void Init(bool useMap);
    virtual void Update();
    virtual void RenderForeground(CL_Vec2f& camPos, float graphicDetailLevel);
    virtual void SetScale(CL_Vec2f& scrnScale);
    virtual void UpdateSFXVolume(float sfxVol);

    void OnEnterForeground(VariantList* pVList);
    void OnEnterBackground(VariantList* pVList);

    void OnAudioDisabled();
    void OnAudioEnabled();

    void* trackable;
    void* smth_else;

    bool m_scaleOnTheFly = false;

    // Used for the weather change animation/fade
    float m_flashAlpha = 1.0f;

    // These two are primarily used in parallax scrolling
    CL_Vec2f m_areaSize;
    CL_Vec2f m_scale = CL_Vec2f(1.0f, 1.0f);

    Rectf m_worldRect;
    /*LoopingSound m_loopingSound;
    float m_sfxBaseVolume = 0.25f;*/
};
//static_assert(sizeof(Background) == 264, "Background class size mismatch.");