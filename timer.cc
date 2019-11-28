#include "timer.h"

Timer::Timer() : start_ticks_(0), paused_ticks_(0), started_(true), paused_(false) {}

void Timer::Start()
{
    // Start the timer.
    started_ = true;
    // Unpause the timer.
    paused_ = false;
    // Get the current clock time.
    start_ticks_ = SDL_GetTicks();
    paused_ticks_ = 0;
}

void Timer::Stop()
{
    // Stop the timer.
    started_ = false;
    // Unpaused the timer.
    paused_ = false;
    // Clear tick variables.
    start_ticks_ = 0;
    paused_ticks_ = 0;
}

void Timer::Pause()
{
    // If the timer is running and isn't already paused.
    if (started_ && !paused_)
    {
        // Pause the timer.
        paused_ = true;
        // Calculate the paused ticks.
        paused_ticks_ = SDL_GetTicks() - start_ticks_;
        start_ticks_ = 0;
    }
}

void Timer::Unpause()
{
    // If the timer is running and paused.
    if (started_ && paused_)
    {
        // Unpaused the timer.
        paused_ = false;
        // Reset the starting ticks.
        start_ticks_ = SDL_GetTicks() - paused_ticks_;
        // Reset the paused ticks.
        paused_ticks_ = 0;
    }
}

unsigned Timer::GetTicks()
{
    unsigned time = 0;
    // If the timer is started.
    if (started_)
    {
        // If the timer is running.
        if (!paused_) time = SDL_GetTicks() - start_ticks_;
        // Else the timer is paused.
        else          time = paused_ticks_;
    }

    return time;
}

bool Timer::IsStarted() { return started_; }

bool Timer::IsPaused() { return paused_; }
