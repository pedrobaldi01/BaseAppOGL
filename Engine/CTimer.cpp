#include "CTimer.h"


//---------------------------------------------------------- 
    // Name:    CTimer::CTimer 
    // Desc:    Default constructor 
    // Args:    None 
    // Rets:    None 
    //---------------------------------------------------------- 
    CTimer::CTimer( void ) 
        : m_StartTime(std::chrono::steady_clock::now()),
          m_LastFrameTime(m_StartTime), m_fSampleElapsed(0.0f), m_fDiffTime(0.0f),
          m_fFPS(0.0f), m_iFramesElapsed(0), m_fDeltaTime(0.0f)
    {   } 
 
    //---------------------------------------------------------- 
    // Name:    CTimer::CTimer 
    // Desc:    Default destructor 
    // Args:    None 
    // Rets:    None 
    //---------------------------------------------------------- 
    CTimer::~CTimer( void ) 
    {   } 
 
    //---------------------------------------------------------- 
    // Name:    CTimer::Init - public 
    // Desc:    Initiate the timer for the program 
    // Args:    None 
    // Rets:    bool:   -true: using performance timer 
    //                  -false: using multimedia timer 
    //---------------------------------------------------------- 
    bool CTimer::Init( void ) 
    { 
        m_StartTime = std::chrono::steady_clock::now();
        m_LastFrameTime = m_StartTime;
        m_fSampleElapsed = 0.0f;
        m_fDiffTime = 0.0f;
        m_fFPS = 0.0f;
        m_iFramesElapsed = 0;
        m_fDeltaTime = 0.0f;

        return true;
    } 
 
    //---------------------------------------------------------- 
    // Name:    CTimer::Update - public 
    // Desc:    Update the timer (perform FPS counter calculations) 
    // Args:    None 
    // Rets:    None 
    //---------------------------------------------------------- 
    void CTimer::Update( void ) 
    {
        const auto now = std::chrono::steady_clock::now();
        const float frameSeconds = std::chrono::duration<float>(now - m_LastFrameTime).count();
        m_LastFrameTime = now;

        if (frameSeconds <= 0.0f)
            return;

        m_fDiffTime = frameSeconds;
        m_fDeltaTime = frameSeconds * 1000.0f;
        m_fSampleElapsed += frameSeconds;
        ++m_iFramesElapsed;

        // Uma janela de meio segundo evita valores de FPS instaveis sem
        // esconder quedas reais de desempenho.
        if (m_fSampleElapsed >= 0.5f)
        {
            m_fFPS = static_cast<float>(m_iFramesElapsed) / m_fSampleElapsed;
            m_fSampleElapsed = 0.0f;
            m_iFramesElapsed = 0;
        }
    } 
 
    //---------------------------------------------------------- 
    // Name:    CTimer::GetTime - public 
    // Desc:    Get the current time since the program started 
    // Args:    None 
    // Rets:    float: The time elapsed since the program started. 
    //---------------------------------------------------------- 
    float CTimer::GetTime( void ) 
    { 
        const auto elapsed = std::chrono::steady_clock::now() - m_StartTime;
        return std::chrono::duration<float, std::milli>(elapsed).count();
    } 
 
    //---------------------------------------------------------- 
    // Name:    CTimer::GetElapsedSeconds - public 
    // Desc:    Get the elapsed seconds since the last frame was drawn. 
    // Args:    elapsedFrames: 
    // Rets:    float: The time elapsed since the program started. 
    //---------------------------------------------------------- 
    float CTimer::GetElapsedSeconds(unsigned long elapsedFrames) 
    {   return m_fDiffTime;     } 
 
    //---------------------------------------------------------- 
    // Name:    CTimer::GetFPS - public 
    // Desc:    Get the current number of frames per second 
    // Args:    None 
    // Rets:    float: the number of frames per second 
    //---------------------------------------------------------- 
    float CTimer::GetFPS( void ) 
    {   return m_fFPS;  } 

    //---------------------------------------------------------- 
    // Name:    CTimer::GetFPS - public 
    // Desc:    Get the Frametime
    // Args:    None 
    // Rets:    float: deltatime
    //---------------------------------------------------------- 
    float CTimer::GetDeltaTime(void)
    {
        return m_fDeltaTime;
    }

    //---------------------------------------------------------- 
    // Name:    CTimer::GetFPS - public 
    // Desc:    Get the Frametime
    // Args:    None 
    // Rets:    float: deltatime
    //---------------------------------------------------------- 
    float CTimer::GetDeltaTimeForAnimation(void)
    {
        return m_fDeltaTime * 0.001f;
    }
