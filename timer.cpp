#include "timer.h"

LTimer::LTimer()
{
    mStartTicks = mPausedTicks = 0;
    mStarted = mPaused = false;
}

void LTimer::start()
{
    mStarted = true;
    mPaused = false;
    mStartTicks = SDL_GetTicks();
    mPausedTicks = 0;
}

void LTimer::stop()
{
    mStarted = mPaused = false;
    mStarted = mPausedTicks = 0;
}

void LTimer::pause()
{
    if (mStarted && !mPaused)
    {
        mPaused = true;
        mPausedTicks = SDL_GetTicks() - mStartTicks;
        mStartTicks = 0;
    }
}

void LTimer::unpause()
{
    if (mStarted && mPaused)
    {
        mPaused = false;
        mStartTicks = SDL_GetTicks() - mPausedTicks;
        mPausedTicks = 0;
    }
}

Uint32 LTimer::getTicks()
{
    Uint32 time = 0;
    if (mStarted)
    {
        if (mPaused)
            time = mPausedTicks;
        else
            time = SDL_GetTicks() - mStartTicks;
    }
    return time;
}

bool LTimer::isStarted()
{
    return mStarted;
}

bool LTimer::isPaused()
{
    return (mStarted && mPaused);
}