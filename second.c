#include <sys/time.h>

/*
double second()
{
    clock_t curClock = clock();
    double t;
    t = (double)curClock / CLOCKS_PER_SEC;
    return t;
}
*/
double second()
{
    struct timeval tm;
    double t;
    gettimeofday(&tm, NULL);
    t = (double)(tm.tv_sec) + ((double)(tm.tv_usec)) / 1.0e6;
    return t;
}