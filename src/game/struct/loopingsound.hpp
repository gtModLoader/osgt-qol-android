#pragma once

#include <string>

// Move this to AudioManager:
typedef uintptr_t AudioHandle;
#define AUDIO_HANDLE_BLANK 0

// From Proton SDK
class LoopingSound
{
  public:
    enum eState
    {
        STATE_IDLE,
        STATE_MOVE_START,
        STATE_MOVING,
        STATE_MOVE_END

    };

    LoopingSound()
    {
        m_loopingSoundHandle = AUDIO_HANDLE_BLANK;
        m_transitionSoundHandle = AUDIO_HANDLE_BLANK;
        m_moveStartTimeMS = 1400;
        m_moveEndTimeMS = 4300;
        m_bMoving = false;
        m_state = STATE_IDLE;
        m_bDisabled = false;
        m_volume = 1.0f;
    }
    virtual ~LoopingSound(){}; // STUB

    void Init(std::string loopingMove, std::string moveStart = "", std::string moveEnd = "",
              std::string loopingIdle = "", bool bAddBasePath = true);
    void SetMoving(bool bNew);
    void SetTransitionTimings(int transitionStartMS, int transitionStopMS);
    void Update(); // needs to be called each frame
    void SetDisabled(bool bDisabled);
    void SetVolume(float vol); // 0 to 1

  private:
    void PlayMoveSound();
    void PlayIdleSound();
    void KillAudio();

    std::string m_loopingMove;
    std::string m_moveStart;
    std::string m_moveEnd;
    std::string m_loopingIdle;

    int m_moveStartTimeMS;
    int m_moveEndTimeMS;

    AudioHandle m_loopingSoundHandle;
    AudioHandle m_transitionSoundHandle;
    bool m_bMoving;

    uint32_t m_waitTimer;
    eState m_state;
    bool m_bDisabled;
    bool m_bAddBasePath;
    float m_volume;
};
static_assert(sizeof(LoopingSound) == 88, "LoopingSound class size mismatch.");