#include "../include/Timer.h"
#include <cstddef>

Timer::Timer()
{
    //ctor
}

void Timer::setStartTime() {
    gettimeofday(&start, NULL);
}

void Timer::setEndTime() {
    gettimeofday(&end, NULL);
}

long Timer::getTimeElapsed() {
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    mseconds = ((seconds) * 1000 + useconds/1000.0) + 0.5;
    return mseconds;
}
