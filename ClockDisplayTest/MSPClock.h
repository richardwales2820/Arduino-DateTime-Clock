#pragma once
class MSPClock
{
public:
	int sec, min, hour, day, month, year;
	int daysPerMonth[12];

	MSPClock();
	MSPClock(int inSec, int inMin, int inHour, int inDay, int inMonth, int inYear);
	~MSPClock();
	void tickTock();
	void checkLeap();
};

