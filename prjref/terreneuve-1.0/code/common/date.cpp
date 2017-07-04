#include ".\date.h"
#include <time.h>

Date::Date(void)
:_serialNumber(LongInteger(0)){
}

Date::Date(LongInteger serialNumber)
: _serialNumber(serialNumber) {
}

Date::Date(Day d, Month m, Year y){
	bool leap = isLeap(y);
	Day len = monthLength(m,leap), offset = monthOffset(m,leap);
	_serialNumber = d + offset + yearOffset(y);
}

Date::Date(Day d, ShortNatural m, Year y) {
	switch (m) {
			case 1:
				Date(d,January,y);
			case 2:
				Date(d,February,y);
			case 3:
				Date(d,March,y);
			case 4:
				Date(d,April,y);
			case 5:
				Date(d,May,y);
			case 6:
				Date(d,June,y);
			case 7:
				Date(d,July,y);
			case 8:
				Date(d,August,y);
			case 9:
				Date(d,September,y);
			case 10:
				Date(d,October,y);
			case 11:
				Date(d,November,y);
			case 12:
				Date(d,December,y);
	}

}

Weekday Date::weekday() const {
	Integer w = _serialNumber % 7;
	return Weekday(w == 0 ? 7 : w);
}

Day Date::dayOfMonth() const {
	return dayOfYear() - monthOffset(month(),isLeap(year()));
}

Day Date::dayOfYear() const {
	return (Day)(_serialNumber - yearOffset(year()));
}

Month Date::month() const {
	Day d = dayOfYear();
	Integer m = d/30 + 1;
	bool leap = isLeap(year());
	while (d <= monthOffset(Month(m),leap))
		m--;
	while (d > monthOffset(Month(m+1),leap))
		m++;
	return Month(m);
}

Year Date::year() const {
	Year y = (Year)(1900+(_serialNumber / 365));
	if (_serialNumber <= yearOffset(y))
		y--;
	return y;
}

LongInteger Date::serialNumber() const {
	return _serialNumber;
}

bool Date::isEndOfMonth() const {
	return isEOM(*this);
}

Day Date::lastDayOfMonth() const {
	return endOfMonth(*this).dayOfMonth();
}

void Date::setDateToToday() {
	time_t tval = time(0);
	struct tm *tmp= localtime(&tval);
	bool leap = isLeap(tmp->tm_year + 1900);
	Day len = monthLength(Month(tmp->tm_mon+1),leap), offset = monthOffset(Month(tmp->tm_mon+1),leap);
	_serialNumber = tmp->tm_mday + offset + yearOffset(tmp->tm_year + 1900);
}

Date& Date::operator+=(LongInteger days) {
	LongInteger serial = _serialNumber + days;
	_serialNumber = serial;
	return *this;
}

Date& Date::operator-=(LongInteger days) {
	LongInteger serial = _serialNumber - days;
	_serialNumber = serial;
	return *this;
}

Date& Date::operator++() {
	LongInteger serial = _serialNumber + 1;
	_serialNumber = serial;
	return *this;
}

Date Date::operator++(int ) {
	Date temp = *this;
	LongInteger serial = _serialNumber + 1;
	_serialNumber = serial;
	return temp;
}

Date& Date::operator--() {
	LongInteger serial = _serialNumber - 1;
	_serialNumber = serial;
	return *this;
}

Date Date::operator--(int ) {
	Date temp = *this;
	LongInteger serial = _serialNumber - 1;
	_serialNumber = serial;
	return temp;
}

Date Date::operator+(LongInteger days) const {
	return Date(_serialNumber+days);
}

Date Date::operator-(LongInteger days) const {
	return Date(_serialNumber-days);
}

Date Date::plusDays(Integer days) const {
	return advance(*this,days,Days);
}

Date Date::plusWeeks(Integer weeks) const {
	return advance(*this,weeks,Weeks);
}

Date Date::plusMonths(Integer months) const {
	return advance(*this,months,Months);
}

Date Date::plusYears(Integer years) const {
	return advance(*this,years,Years);
}

Date Date::plus(Integer n, TimeUnit units) const {
	return advance(*this,n,units);
}

Date Date::minDate() {
	static const Date minimumDate(minimumSerialNumber());
	return minimumDate;
}

Date Date::maxDate() {
	static const Date maximumDate(maximumSerialNumber());
	return maximumDate;
}

Date Date::advance(const Date& date, Integer n, TimeUnit units) {
	switch (units) {
		  case Days:
			  return date + n;
		  case Weeks:
			  return date + 7*n;
		  case Months: {
			  Day d = date.dayOfMonth();
			  Integer m = Integer(date.month())+n;
			  Year y = date.year();
			  while (m > 12) {
				  m -= 12;
				  y += 1;
			  }
			  while (m < 1) {
				  m += 12;
				  y -= 1;
			  }

			  Integer length = monthLength(Month(m), isLeap(y));
			  if (d > length)
				  d = length;

			  return Date(d, Month(m), y);
					   }
		  case Years: {
			  Day d = date.dayOfMonth();
			  Month m = date.month();
			  Year y = date.year()+n;

			  if (d == 29 && m == February && !isLeap(y))
				  d = 28;

			  return Date(d,m,y);
					  }
		  //default:

	}
}

bool Date::isLeap(Year y) {
	static const bool YearIsLeap[] = {
			// 1900-1909
			true,false,false,false, true,false,false,false, true,false,
			// 1910-1919
			false,false, true,false,false,false, true,false,false,false,
			// 1920-1929
			true,false,false,false, true,false,false,false, true,false,
			// 1930-1939
			false,false, true,false,false,false, true,false,false,false,
			// 1940-1949
			true,false,false,false, true,false,false,false, true,false,
			// 1950-1959
			false,false, true,false,false,false, true,false,false,false,
			// 1960-1969
			true,false,false,false, true,false,false,false, true,false,
			// 1970-1979
			false,false, true,false,false,false, true,false,false,false,
			// 1980-1989
			true,false,false,false, true,false,false,false, true,false,
			// 1990-1999
			false,false, true,false,false,false, true,false,false,false,
			// 2000-2009
			true,false,false,false, true,false,false,false, true,false,
			// 2010-2019
			false,false, true,false,false,false, true,false,false,false,
			// 2020-2029
			true,false,false,false, true,false,false,false, true,false,
			// 2030-2039
			false,false, true,false,false,false, true,false,false,false,
			// 2040-2049
			true,false,false,false, true,false,false,false, true,false,
			// 2050-2059
			false,false, true,false,false,false, true,false,false,false,
			// 2060-2069
			true,false,false,false, true,false,false,false, true,false,
			// 2070-2079
			false,false, true,false,false,false, true,false,false,false,
			// 2080-2089
			true,false,false,false, true,false,false,false, true,false,
			// 2090-2099
			false,false, true,false,false,false, true,false,false,false,
			// 2100
			false
	};
	return YearIsLeap[y-1900];
}

Date Date::endOfMonth(const Date& d) {
	Month m = d.month();
	Year y = d.year();
	return Date(monthLength(m, isLeap(y)), m, y);
}

bool Date::isEOM(const Date& d) {
	return (d.dayOfMonth() == monthLength(d.month(), isLeap(d.year())));
}

Date Date::nextWeekday(const Date& d, Weekday dayOfWeek) {
	Weekday wd = d.weekday();
	return d + ((wd>dayOfWeek ? 7 : 0) - wd + dayOfWeek);
}

Date Date::nthWeekday(ShortInteger nth, Weekday dayOfWeek, Month m, Year y) 
{
	Weekday first = Date(1, m, y).weekday();
	ShortInteger skip = nth - (dayOfWeek>=first ? 1 : 0);
	return Date(1 + dayOfWeek-first + skip*7, m, y);
}

Integer Date::monthLength(Month m, bool leapYear) {
	static const Integer MonthLength[] = {
		31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
	};
	static const Integer MonthLeapLength[] = {
		31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
	};
	return (leapYear? MonthLeapLength[m-1] : MonthLength[m-1]);
}

Integer Date::monthOffset(Month m, bool leapYear) {
	static const Integer MonthOffset[] = {
		0,  31,  59,  90, 120, 151, 181, 212, 243, 273, 304, 334,
			365     // used in dayOfMonth to bracket day
	};
	static const Integer MonthLeapOffset[] = {
		0,  31,  60,  91, 121, 152,	182, 213, 244, 274, 305, 335,
			366     // used in dayOfMonth to bracket day
	};
	return (leapYear? MonthLeapOffset[m-1] : MonthOffset[m-1]);
}
LongInteger Date::yearOffset(Year y) {
	static const LongInteger YearOffset[] = {
			// 1900-1909
			0,  366,  731, 1096, 1461, 1827, 2192, 2557, 2922, 3288,
			// 1910-1919
			3653, 4018, 4383, 4749, 5114, 5479, 5844, 6210, 6575, 6940,
			// 1920-1929
			7305, 7671, 8036, 8401, 8766, 9132, 9497, 9862,10227,10593,
			// 1930-1939
			10958,11323,11688,12054,12419,12784,13149,13515,13880,14245,
			// 1940-1949
			14610,14976,15341,15706,16071,16437,16802,17167,17532,17898,
			// 1950-1959
			18263,18628,18993,19359,19724,20089,20454,20820,21185,21550,
			// 1960-1969
			21915,22281,22646,23011,23376,23742,24107,24472,24837,25203,
			// 1970-1979
			25568,25933,26298,26664,27029,27394,27759,28125,28490,28855,
			// 1980-1989
			29220,29586,29951,30316,30681,31047,31412,31777,32142,32508,
			// 1990-1999
			32873,33238,33603,33969,34334,34699,35064,35430,35795,36160,
			// 2000-2009
			36525,36891,37256,37621,37986,38352,38717,39082,39447,39813,
			// 2010-2019
			40178,40543,40908,41274,41639,42004,42369,42735,43100,43465,
			// 2020-2029
			43830,44196,44561,44926,45291,45657,46022,46387,46752,47118,
			// 2030-2039
			47483,47848,48213,48579,48944,49309,49674,50040,50405,50770,
			// 2040-2049
			51135,51501,51866,52231,52596,52962,53327,53692,54057,54423,
			// 2050-2059
			54788,55153,55518,55884,56249,56614,56979,57345,57710,58075,
			// 2060-2069
			58440,58806,59171,59536,59901,60267,60632,60997,61362,61728,
			// 2070-2079
			62093,62458,62823,63189,63554,63919,64284,64650,65015,65380,
			// 2080-2089
			65745,66111,66476,66841,67206,67572,67937,68302,68667,69033,
			// 2090-2099
			69398,69763,70128,70494,70859,71224,71589,71955,72320,72685,
			// 2100
			73050
	};
	return YearOffset[y-1900];
}

LongInteger Date::minimumSerialNumber() {
	return 367;
}

LongInteger Date::maximumSerialNumber() {
	return 73050;    // Dec 31, 2099
}

bool Date::isBusinessDay() {
	return 1;
}

void Date::applyConvention(BusinessDayConvention convention) {
	switch (convention) {
		case Preceding:
			while (!isBusinessDay()) {_serialNumber--;};
		case ModifiedPreceding: {
			LongInteger ser=_serialNumber;
			while (!isBusinessDay()) {_serialNumber--;};
			if (Date(ser).month()!=month()) {
				_serialNumber=ser;
				while (!isBusinessDay()) {_serialNumber++;};
			}
		}
		case Following:
			while (!isBusinessDay()) {_serialNumber++;};
		case ModifiedFollowing: {
			LongInteger ser=_serialNumber;
			while (!isBusinessDay()) {_serialNumber++;};
			if (Date(ser).month()!=month()) {
				_serialNumber=ser;
				while (!isBusinessDay()) {_serialNumber--;};
			}
		}
	}
}		

Date Date::returnDateConvention(const Date& date, BusinessDayConvention convention) {
	Date d1=Date(date.serialNumber());
	d1.applyConvention(convention);
	return d1;
}

Real Date::dayCount(const Date& d,DayCountConvention dayconvention) const {
	switch (dayconvention) {
		case ACT_365:
			return (TN_REAL(d.serialNumber()-_serialNumber)/365);
		case ACT_360:
			return (TN_REAL(d.serialNumber()-_serialNumber)/360);
		case Day30_365:
			return (TN_REAL(d.dayOfMonth()+30-dayOfMonth()+30*(12*(d.year()-year())+d.month()-month()))/365);
		case Day30_360:
			return (TN_REAL(d.dayOfMonth()+30-dayOfMonth()+30*(12*(d.year()-year())+d.month()-month()))/360);
		default:
			return (TN_REAL(d.serialNumber()-_serialNumber)/365);
	}
}

char* Date::toString() const {
	char* strDate=new char[10];
	sprintf(strDate,"%d/%d/%d",month(),dayOfMonth(),year());
	return strDate;
}

Date::~Date(void)
{
}
