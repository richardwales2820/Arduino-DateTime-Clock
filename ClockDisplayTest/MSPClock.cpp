#include "MSPClock.h"



MSPClock::MSPClock()
{
}

MSPClock::MSPClock(int inSec, int inMin, int inHour, int inDay, int inMonth, int inYear)
{
	sec = inSec;
	min = inMin;
	hour = inHour;
	day = inDay;
	month = inDay;
	year = inYear;

	int i;

	for (i = 0; i < 12; i++)
	{
		if (i == 0 || i == 2 || i == 4 || i == 6 || i == 7 || i == 9 || i == 11)
			daysPerMonth[i] = 31;
		else
			daysPerMonth[i] = 30;
	}

	checkLeap();
}

MSPClock::~MSPClock()
{
}

void MSPClock::checkLeap()
{
	if (year % 4 == 0)
		daysPerMonth[1] = 29;
	else
		daysPerMonth[1] = 28;
}

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

			if (hour < 23)
				hour++;

			else
			{
				hour = 0;

				if (day < daysPerMonth[month])
					day++;

				else
				{
					day = 0;

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
