#ifndef TIMERCPP
#define TIMERCPP //header guard

#include "TRON.h"

LTimer::LTimer() //initialise un timer
{
  mStartTicks = 0;
  mPausedTicks = 0;
  mPaused = false;
  mStarted = false;
}

void LTimer::start() //démarre le timer
{
  mStarted = true;
  mPaused = false;
  mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void LTimer::stop() //arrête le timer
{
  mStarted = false;
  mPaused = false;
	mStartTicks = 0;
	mPausedTicks = 0;
}

void LTimer::pause() //met le tumer en pause
{
  if(mStarted && !mPaused)
  {
    mPaused = true;
    mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
  }
}

void LTimer::unpause() //redémarre le timer (depuis la valeur où il avait été mis en pause)
{
  if(mStarted && mPaused)
  {
    mPaused = false;
    mStartTicks = SDL_GetTicks() - mPausedTicks;
    mPausedTicks = 0;
  }
}

Uint32 LTimer::getTicks() //renvoie le temps écoulé depuis le démarrage du timer
{
	Uint32 time = 0;
  if(mStarted)
  {
    if(mPaused)
    {
      time = mPausedTicks;
    }
    else
    {
      time = SDL_GetTicks() - mStartTicks;
    }
  }
  return time;
}

bool LTimer::isStarted() //renvoie si le timer est lancé ou non
{
    return mStarted;
}

bool LTimer::isPaused() //renvoie si le timer est pausé ou non
{
  return mPaused && mStarted;
}

#endif
