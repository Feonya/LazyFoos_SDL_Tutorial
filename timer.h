#ifndef TIMER_H_
#define TIMER_H_

#include "SDL2/SDL.h"

// The application time based timer.
class Timer
{
public:
    Timer();

    // The various clock actions.
    void Start();
    void Stop();
    void Pause();
    void Unpause();

    // Get the timer's time.
    unsigned GetTicks();

    // Checks the status of the timer.
    bool IsStarted();
    bool IsPaused();

private:
    // The clock time when the timer started.
    unsigned start_ticks_;

    // The ticks stored when the timer was paused.
    unsigned paused_ticks_;

    // The timer status.
    bool started_;
    bool paused_;
};

#endif  // TIMER_H_
