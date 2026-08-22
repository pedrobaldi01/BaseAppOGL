#pragma once 
 
////////////////////////////////////////////////////////////////////// 
// INCLUDES 
////////////////////////////////////////////////////////////////////// 
 
#include <chrono>

 
////////////////////////////////////////////////////////////////////// 
// CLASSES 
////////////////////////////////////////////////////////////////////// 
 
class CTimer
{ 
private: 
    std::chrono::steady_clock::time_point m_StartTime;
    std::chrono::steady_clock::time_point m_LastFrameTime;
 
    //FPS variables 
    float m_fSampleElapsed;
    float m_fDiffTime; 
    float m_fFPS; 
    int m_iFramesElapsed; 
    float	m_fDeltaTime;
 
public: 
 
    //---------------------------------------------------------- 
    // Name:    CTimer2::CTimer2 
    // Desc:    Default constructor 
    // Args:    None 
    // Rets:    None 
    //---------------------------------------------------------- 
    CTimer( void );
 
    //---------------------------------------------------------- 
    // Name:    CTimer2::CTimer2 
    // Desc:    Default destructor 
    // Args:    None 
    // Rets:    None 
    //---------------------------------------------------------- 
    virtual ~CTimer( void );
 
    //---------------------------------------------------------- 
    // Name:    CTimer2::Init - public 
    // Desc:    Initiate the timer for the program 
    // Args:    None 
    // Rets:    bool:   -true: using performance timer 
    //                  -false: using multimedia timer 
    //---------------------------------------------------------- 
    bool Init( void );
 
    //---------------------------------------------------------- 
    // Name:    CTimer2::Update - public 
    // Desc:    Update the timer (perform FPS counter calculations) 
    // Args:    None 
    // Rets:    None 
    //---------------------------------------------------------- 
    void Update( void );
 
    //---------------------------------------------------------- 
    // Name:    CTimer2::GetTime - public 
    // Desc:    Get the current time since the program started 
    // Args:    None 
    // Rets:    float: The time elapsed since the program started. 
    //---------------------------------------------------------- 
    float GetTime( void );
 
    //---------------------------------------------------------- 
    // Name:    CTimer2::GetElapsedSeconds - public 
    // Desc:    Get the elapsed seconds since the last frame was drawn. 
    // Args:    elapsedFrames: 
    // Rets:    float: The time elapsed since the program started. 
    //---------------------------------------------------------- 
    float GetElapsedSeconds(unsigned long elapsedFrames = 1);
 
    //---------------------------------------------------------- 
    // Name:    CTimer2::GetFPS - public 
    // Desc:    Get the current number of frames per second 
    // Args:    None 
    // Rets:    float: the number of frames per second 
    //---------------------------------------------------------- 
    float GetFPS( void );

    //---------------------------------------------------------- 
    // Name:    CTimer2::GetFPS - public 
    // Desc:    Get the Frametime
    // Args:    None 
    // Rets:    float: deltatime
    //---------------------------------------------------------- 
    float GetDeltaTime(void);

    //---------------------------------------------------------- 
   // Name:    CTimer2::GetFPS - public 
   // Desc:    Get the Frametime
   // Args:    None 
   // Rets:    float: deltatime
   //---------------------------------------------------------- 
    float GetDeltaTimeForAnimation(void);
}; 
 
