#pragma once //only include this header once on compilation (just for compiler safety)
class MSPClock
{
// public object variables and functions able to be called from anyone
public:
  // time variables to store datetime 
	int sec, min, hour, day, month, year;
  int dayOfWeek;
  int dstDone;
	double sunriseHour, sunriseMinute, sunsetHour, sunsetMinute;
  // daysPerMonth holds days in each month, changes depending on leap year  
	int daysPerMonth[12];

  // MSPClock.cpp will include these functions with the signatures defined below
	MSPClock();
	MSPClock(int inSec, int inMin, int inHour, int inDay, int inMonth, int inYear);
	~MSPClock();
	void tickTock();
	void checkLeap();
	void riseAndShine();
  int getSecondsPerDegree();
};

