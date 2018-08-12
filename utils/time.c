#include <stdlib.h>
#include <sys/time.h>

int timeold_ms;
int timenew_ms;
int count = -1;

int getfps()
{
    count++;
    struct timeval start;
    gettimeofday(&start, NULL);
    int ms = start.tv_sec * 1000 + start.tv_usec/1000;;
    if (count % 3 == 0)
    {
        timeold_ms = ms;
        return -1;
    }
    else if (count % 3 == 1)
    {
        timenew_ms = ms;
        return 1000/(timenew_ms - timeold_ms);
    }
    return -1;
}