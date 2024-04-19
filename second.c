#include <sys/time.h>

double second()
{
    clock_t curClock = clock();
    double t;
    t = (double)curClock / CLOCKS_PER_SEC;
    return t;
}