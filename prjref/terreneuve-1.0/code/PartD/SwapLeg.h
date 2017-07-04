#pragma once

#ifndef SWAPLEG_H
#define SWAPLEG_H

#include "./../Common/Date.h"
#include "./../Common/types.h"
#include <valarray>
using namespace std;

class SwapLeg
{
public:

	/** Constructor that takes a notional and the constant value of which the 
	notional should be decreasing every time. If endDate!=startDate + multiple
	of frequency then endDate is reduced until it satisfies the condition*/
	SwapLeg(Date startDate, Real Frequency, Date endDate, Real Notional, Real AmortizingConstant, BusinessDayConvention convention);
	
	/** For more general swaps : takes a valarray of notionals for every period 
	and a valarray of dates */
	SwapLeg(valarray<Date> dates, valarray<Real> Notionals);

	/** Return size of valarray */
	LongInteger returnSize();

	/** Return dates valarray */
	valarray<Date> returnDates();

	/** Return dates valarray */
	valarray<Real> returnAmounts();

	~SwapLeg(void);

private:
	valarray<Date> _dateSchedule;
	valarray<Real> _flowSchedule;
};

#endif;

