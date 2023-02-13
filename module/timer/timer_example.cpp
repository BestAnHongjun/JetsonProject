#include "timer.h"
#include <unistd.h>


int main()
{
    TIMER(Timer timer("timer_20"));
    TIMER(Timer timer_40("timer_40"));
    
    for (int i = 0; i < 100; i++)
    {
        TIMER(timer.start());
        usleep(3000);
        TIMER(timer.end());

        TIMER(timer_40.start());
        usleep(1000);
        TIMER(timer_40.end());
    }

    return 0;
}