#include <stdio.h>
#include "MSPClock.h"

int main(void)
{
    MSPClock c(0, 0, 7, 3, 4, 2016);
    c.riseAndShine();

    printf("Sunrise: %02.lf:%02.lf\n", c.sunriseHour, c.sunriseMinute);
    printf("Sunset: %02.lf:%02.lf\n", c.sunsetHour, c.sunsetMinute);

    return 0;
}
