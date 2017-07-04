#pragma once
#ifndef date_h
#define date_h

#include ".\types.h"
#include <stdio.h>

/**
@author Simon
*/

typedef ShortNatural Day;
typedef ShortNatural Year;

enum Weekday { Sunday    = 1,
               Monday    = 2,
               Tuesday   = 3,
               Wednesday = 4,
               Thursday  = 5,
               Friday    = 6,
               Saturday  = 7
};

enum Month { January   = 1,
             February  = 2,
             March     = 3,
             April     = 4,
             May       = 5,
			 June      = 6,
             July      = 7,
             August    = 8,
             September = 9,
             October   = 10,
             November  = 11,
             December  = 12
};

enum TimeUnit { Days,
                Weeks,
                Months,
                Years
};

enum Frequency { NoFrequency = -1,     //null frequency
                 Once = 0,             //zero-coupon
                 Annual = 1,           //once a year
                 Semiannual = 2,       //twice a year
                 EveryFourthMonth = 3, //every fourth month
                 Quarterly = 4,        //every quarter
                 Bimonthly = 6,        //every two months
                 Monthly = 12          //once a month
};

enum BusinessDayConvention {
        Unadjusted,         /* Do not adjust. */
        Preceding,          /* first business day before
                               the given holiday. */
        ModifiedPreceding,  /* first business day before
                               the given holiday unless it belongs to a
                               different month, in which case choose the
                               first business day after the holiday. */
        Following,          /* first business day after
                               the given holiday. */
        ModifiedFollowing,  /* first business day after
                               the given holiday unless it belongs to a
                               different month, in which case choose the
                               first business day before the holiday. */
        MonthEndReference   /* first business day after
                               the given holiday, if the original date falls
                               on last business day of month result reverts
                               to first business day before month-end */
};

enum DayCountConvention {
		ACT_365,		//exact number of days but 365 in a year
		ACT_360,		//exact number of days but 360 in a year
		Day30_365,			//30 days per month and 365 in a year
		Day30_360			//30 days per month and 360 in a year
};

class Date
{
public:
	/** Default constructor */
	Date(void);
	/**Constructor taking a serial number */
	Date(LongInteger serialNumber);
	/**Constructor taking day, month and year */
	Date(Day d, Month m, Year y);
	/**Constructor taking day, month as an integer and year */
	Date(Day d, ShortNatural m, Year y);
	~Date(void);

	Weekday weekday() const;
	Day dayOfMonth() const;
	Day dayOfYear() const;
	Month month() const;
	Year year() const;
	LongInteger serialNumber() const;
	bool isEndOfMonth() const;
	Day lastDayOfMonth() const;

	/** Set Date to system today's date */
	void setDateToToday();

	/**  increments date by the given number of days */
	Date& operator+=(LongInteger days);
	/**  decrement date by the given number of days */
	Date& operator-=(LongInteger days);
	/**  1-day pre-increment */
	Date& operator++();
	/**  1-day post-increment */
	Date operator++(int );
	/**  1-day pre-decrement */
	Date& operator--();
	/**  1-day post-decrement */
	Date operator--(int );
	/**  returns a new date incremented by the given number of days */
	Date operator+(LongInteger days) const;
	/**  returns a new date decremented by the given number of days */
	Date operator-(LongInteger days) const;

	inline bool operator==(const Date& d2){return (_serialNumber == d2.serialNumber());};
	inline bool operator!=(const Date&d2){return (_serialNumber != d2.serialNumber());};
    inline bool operator<(const Date& d2){return (_serialNumber < d2.serialNumber());};
    inline bool operator<=(const Date& d2){return (_serialNumber <= d2.serialNumber());};
    inline bool operator>(const Date& d2){return (_serialNumber > d2.serialNumber());};
    inline bool operator>=(const Date& d2){return (_serialNumber >= d2.serialNumber());};

	Date plusDays(Integer n) const;
	/* \deprecated use date + n*Weeks instead */
	Date plusWeeks(Integer n) const;
	/* \deprecated use date + n*Months instead */
	Date plusMonths(Integer n) const;
	/* \deprecated use date + n*Years instead */
	Date plusYears(Integer n) const;
	/* \deprecated use date + n*units instead */
	Date plus(Integer n, TimeUnit units) const;

	/** earliest allowed date */
	static Date minDate();
	/** latest allowed date */
	static Date maxDate();
	/** whether the given year is a leap one */
	static bool isLeap(Year y);
	/** last day of the month to which the given date belongs */
	static Date endOfMonth(const Date& d);
	/** whether a date is the last day of its month */
	static bool isEOM(const Date& d);
	/** next given weekday following or equal to the given date */
	static Date nextWeekday(const Date& d, Weekday);
	/** n-th given weekday in the given month and year */
	static Date nthWeekday(ShortInteger n, Weekday, Month m, Year y);

	/**Apply Conventions (use for UsDate for example) */
	bool isBusinessDay();
	void applyConvention(BusinessDayConvention convention=Following);
	Date returnDateConvention(const Date& date, BusinessDayConvention convention=Following);
	
	/**DayCount Between Dates */
	Real dayCount(const Date& d,DayCountConvention dayconvention=ACT_365) const;

	/** Return char* version of the date */
	char* toString() const;

private:
	LongInteger _serialNumber;
	static Date advance(const Date& d, Integer units, TimeUnit);
	static Integer monthLength(Month m, bool leapYear);
	static Integer monthOffset(Month m, bool leapYear);
	static LongInteger yearOffset(Year y);
	static LongInteger minimumSerialNumber();
	static LongInteger maximumSerialNumber();
};

#endif
