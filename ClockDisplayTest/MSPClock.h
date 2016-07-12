#pragma once //only include this header once on compilation (just for compiler safety)
class MSPClock
{
// public object variables and functions able to be called from anyone
public:
  // time variables to store datetime 
	unsigned int sec, min, hour, day, month, year;
  unsigned int dayOfWeek;
  unsigned int dstDone;
	double sunriseHour, sunriseMinute, sunsetHour, sunsetMinute;
  // daysPerMonth holds days in each month, changes depending on leap year  
	unsigned int daysPerMonth[12];

  // MSPClock.cpp will include these functions with the signatures defined below
	MSPClock();
	MSPClock(unsigned int inSec, unsigned int inMin, unsigned int inHour, unsigned int inDay, unsigned int inMonth, unsigned int inYear);
	~MSPClock();
	void tickTock();
	void checkLeap();
	void riseAndShine();
  unsigned int getSecondsPerDegree();
};

