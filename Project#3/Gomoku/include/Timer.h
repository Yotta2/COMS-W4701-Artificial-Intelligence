#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>

class Timer
{
public:
    Timer();
    void setStartTime();
    void setEndTime();
    long getTimeElapsed();
private:
    struct timeval start, end;
    long mseconds, seconds, useconds;
};

#endif // TIMER_H
