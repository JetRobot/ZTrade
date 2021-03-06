#include ".\usdate.h"
#using <mscorlib.dll>

bool UsDate::isBusinessDay() {
	Weekday w = weekday();
	Day d = dayOfMonth();
	Month m = month();
	if ((w == Saturday || w == Sunday)
		// New Year's Day (possibly moved to Monday if on Sunday)
		|| ((d == 1 || (d == 2 && w == Monday)) && m == January)
		// (or to Friday if on Saturday)
		|| (d == 31 && w == Friday && m == December)
		// Martin Luther King's birthday (third Monday in January)
		|| ((d >= 15 && d <= 21) && w == Monday && m == January)
		// Washington's birthday (third Monday in February)
		|| ((d >= 15 && d <= 21) && w == Monday && m == February)
		// Memorial Day (last Monday in May)
		|| (d >= 25 && w == Monday && m == May)
		// Independence Day (Monday if Sunday or Friday if Saturday)
		|| ((d == 4 || (d == 5 && w == Monday) ||
		(d == 3 && w == Friday)) && m == July)
		// Labor Day (first Monday in September)
		|| (d <= 7 && w == Monday && m == September)
		// Columbus Day (second Monday in October)
		|| ((d >= 8 && d <= 14) && w == Monday && m == October)
		// Veteran's Day (Monday if Sunday or Friday if Saturday)
		|| ((d == 11 || (d == 12 && w == Monday) ||
		(d == 10 && w == Friday)) && m == November)
		// Thanksgiving Day (fourth Thursday in November)
		|| ((d >= 22 && d <= 28) && w == Thursday && m == November)
		// Christmas (Monday if Sunday or Friday if Saturday)
		|| ((d == 25 || (d == 26 && w == Monday) ||
		(d == 24 && w == Friday)) && m == December))
		return false;
	return true;
}
