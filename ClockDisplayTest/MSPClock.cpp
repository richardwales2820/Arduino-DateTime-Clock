#include "MSPClock.h"
#include <math.h>

// Default Constructor
MSPClock::MSPClock()
{
}

// Overloaded constructor to include the ability to set the data variables to some values at initialization
MSPClock::MSPClock(int inSec, int inMin, int inHour, int inDay, int inMonth, int inYear)
{
  // Sets local object's values to the passed in arguments
	sec = inSec;
	min = inMin;
	hour = inHour;
	day = inDay;
	month = inDay;
	year = inYear;
  dstDone = 0;
  
	int i;

  // Initializes the dasPerMonth array to the correct number of days according to the month
	for (i = 0; i < 12; i++)
	{
		if (i == 0 || i == 2 || i == 4 || i == 6 || i == 7 || i == 9 || i == 11)
			daysPerMonth[i] = 31;
		else
			daysPerMonth[i] = 30;
	}

  // checkLeap() will set feb to 28 or 29 days depending on the year
	checkLeap();
}

// Default deconstructor called when object is deleted
// No memory freeing has to be done here as no pointers are used for dynamic memory allocation
MSPClock::~MSPClock()
{
}

// if year mod 4, its a leap year. Set feb days to 29, else set to 28
void MSPClock::checkLeap()
{
	if (year % 4 == 0)
		daysPerMonth[1] = 29;
	else
		daysPerMonth[1] = 28;
}

// Increments the seconds by 1 when it is called
// Will increment other time variables if the previous variable reaches its highest number to overflow over
void MSPClock::tickTock()
{
	if (sec < 59)
		sec++;

	else
	{
		sec = 0;

		if (min < 59)
			min++;

		else
		{
			min = 0;

      if (hour == 1 && day >= 7 && dayOfWeek == 7 && month == 3 && dstDone == 0)
      {
        hour++;
        dstDone = 1;
      }
      
      else if (hour == 1 && dayOfWeek == 7 && month == 11 && dstDone == 0)
      {
        hour--;
        dstDone = 1;
      }
      
			if (hour < 23)
				hour++;

			else
			{
				hour = 0;

        dayOfWeek++;
        if (dayOfWeek == 8)
          dayOfWeek = 1;
        
				if (day < daysPerMonth[month])
					day++;

				else
				{
					day = 0;

          dstDone = 0;
					if (month < 12)
						month++;
          
					else
					{
						month = 0;

						year++;
						checkLeap();
					}
				}
			}
		}
  }
}

void MSPClock::riseAndShine()
{
	double pi = M_PI;
	double theta = 2 * pi * (day - 80)/365.25;
	
	double radiusEarth = 6378 * pow(10, 3);
	double latitude = 0.698;
	double earthsDistance = 149598000 * pow (10, 3);
	double epsilon = 0.409;
	
	double z_s = earthsDistance * sin(theta) * sin(epsilon);
	double r_p = sqrt(pow(earthsDistance, 2) - pow(z_s, 2));
	double t_0 = (1440 * acos((radiusEarth-z_s * sin(latitude))/(r_p * cos(latitude)))/
				 (2 * pi)) / 60.0;
	double t_hat = (t_0 + 5.0/60.0);
	
	double n = (720-10 * sin(4 * pi * (day - 80)/365.25) + 8 * sin(2 * pi * 
				day / 365.25)) / 60.0;
	double sunrise = (n - t_hat);
	sunriseHour = floor(sunrise);
	double sunriseFraction = sunrise - sunriseHour;
	sunriseMinute = sunriseFraction * 60.0;
	
	double sunset = (n + t_hat);
	sunsetHour = floor(sunset);
	double sunsetFraction = sunset - sunsetHour;
	sunsetMinute = sunsetFraction * 60.0;
}

int MSPClock::getSecondsPerDegree()
{
  double daylightSeconds = ((sunsetHour - sunriseHour) * 60 + (sunsetMinute - sunriseMinute)) * 60;
  int secondsPerDegree = floor(daylightSeconds / 180);

  return secondsPerDegree;
}

