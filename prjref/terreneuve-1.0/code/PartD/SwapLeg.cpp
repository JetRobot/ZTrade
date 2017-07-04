#include ".\swapleg.h"

SwapLeg::SwapLeg(Date startDate, Real Frequency, Date endDate, Real Notional, Real AmortizingConstant, BusinessDayConvention convention)
{
	Natural i=1;
	LongInteger serialEnd=endDate.serialNumber();
	LongInteger serialCurrent=startDate.serialNumber();
	Natural test;
	test=Natural((2+serialEnd-serialCurrent)*Frequency/365.25)+1;
	_dateSchedule.resize(Natural((2+serialEnd-serialCurrent)*Frequency/365.25)+2);
	_flowSchedule.resize(Natural((2+serialEnd-serialCurrent)*Frequency/365.25)+2);
	_dateSchedule[0]=startDate;
	_flowSchedule[0]=0.;

	while (serialCurrent<=serialEnd) {
		Date Current;
		startDate=startDate.plusDays(Integer(365.25/Frequency));
		Current=startDate;
		Current.applyConvention(convention);
		serialCurrent=Current.serialNumber();
		_flowSchedule[i]=Notional-(i-1)*AmortizingConstant;
		_dateSchedule[i]=Current;
		i++;
	}
}

SwapLeg::SwapLeg(valarray<Date> dates, valarray<Real> Notionals)
{
	_dateSchedule.resize(dates.size());
	_flowSchedule.resize(Notionals.size());
	_dateSchedule=dates;
	_flowSchedule=Notionals;
}

LongInteger SwapLeg::returnSize()
{
	return _dateSchedule.size();
}

valarray<Date> SwapLeg::returnDates()
{
	return _dateSchedule;
}

valarray<Real> SwapLeg::returnAmounts()
{
	return _flowSchedule;
}

SwapLeg::~SwapLeg(void)
{
}
