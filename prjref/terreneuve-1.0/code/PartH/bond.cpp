#include ".\bond.h"

cashflow::cashflow(valarray<Date> dates, valarray<Real> cashflows)
:	_dates(dates),
	_cashflows(cashflows)
{
}

valarray<Date> cashflow::getDates(){
	return _dates;
}

valarray<Real>	cashflow::getCashflows(){
	return _cashflows;
}


bond::bond(Date issue, Date maturity, Date firstcoupondate, Real coupon, Frequency freq, Real faceamount,DayCountConvention daycount, yieldCurve yc)
:	_issue(issue),
	_maturity(maturity),
	_firstcoupondate(firstcoupondate),
	_coupon(coupon),
	_freq(freq),
	_faceamount(faceamount),
	_daycount(daycount),
	_yc(yc)
{
}


cashflow bond::getCashflow(){
	Real timetomaturity = _firstcoupondate.dayCount(_maturity, _daycount);
	Real numberofcashflow;
	Real coupon;

	Integer nbofmonth;

	switch (_freq) {
		case NoFrequency:
			numberofcashflow = 1.0;
			coupon = 0;
			nbofmonth = 0;
			break;
		case Once:
			numberofcashflow = 1.0;
			coupon = 0;
			nbofmonth = 0;
			break;
		case Annual:
			numberofcashflow = timetomaturity + 1;
			coupon = _coupon;
			nbofmonth = 12;
			break;
		case Semiannual:
			numberofcashflow = 2.0*timetomaturity + 1;
			coupon = _coupon/2.0;
			nbofmonth = 6;
			break;
		case EveryFourthMonth:
			numberofcashflow = 3.0*timetomaturity + 1;
			coupon = _coupon/3.0;
			nbofmonth = 4;
			break;
		case Quarterly:
			numberofcashflow = 4.0*timetomaturity + 1;
			coupon = _coupon/4.0;
			nbofmonth = 3;
			break;
		case Bimonthly:
			numberofcashflow = 6.0*timetomaturity + 1;
			coupon = _coupon/6.0;
			nbofmonth = 2;
			break;
		case Monthly:
			numberofcashflow = 12.0*timetomaturity + 1;
			coupon = _coupon/12.0;
			nbofmonth = 1;
			break;
		default:
			numberofcashflow = 1.0;
			coupon = _coupon;
			nbofmonth = 0;
			break;
	}



	
	Date d(_firstcoupondate);
	Natural numberofCF = (int) (numberofcashflow + 0.1);

	valarray<Date> dates(numberofCF);
	valarray<Real> cashflows(numberofCF);
	
	dates[0]=d;
	Date dTest;
	cashflows[0] = _faceamount*coupon;
	Natural i;
	for(i=1;i<numberofCF;i++)
	{
		dTest=d.plusMonths(i*nbofmonth);
		dTest.applyConvention(Following);
		dates[i]=dTest;
		cashflows[i] = _faceamount*coupon;
	}

	if (_freq==Once)
		cashflows[0] = _faceamount;
	else
		cashflows[numberofCF-1] += _faceamount;

	cashflow CF(dates, cashflows);
	return CF;
}
	
Real bond::fairvalue(Date today){

	Real PV = quotedPrice(today);
	Real accruedinterest;

	if(today<=_issue ||today>=_maturity)
		accruedinterest = 0;
	else
	{
		cashflow CF = getCashflow();
		valarray<Date> dates = CF.getDates();
		valarray<Real> cashflows = CF.getCashflows();

		Natural i=0;
		while((dates[i]<today) && (i<dates.size()))
			i++;
	
		Date interestaccrualdate;
		if (today<_firstcoupondate)
			interestaccrualdate = _issue;
		else
			interestaccrualdate = dates[i-1];

		Real numberofdays, numberofdays2, numberofcoupon;


		switch(_freq){
			case NoFrequency:
				numberofcoupon = 1.0;//to avoid division by zero
				break;
			case Once:
				numberofcoupon = 1.0;
				break;
			case Annual:
				numberofcoupon = 1.0;
				break;
			case Semiannual:
				numberofcoupon = 2.0;
				break;
			case EveryFourthMonth:
				numberofcoupon = 3.0;
				break;
			case Quarterly:
				numberofcoupon = 4.0;
				break;
			case Bimonthly:
				numberofcoupon = 6.0;
				break;
			case Monthly:
				numberofcoupon = 12.0;
				break;
			default:
				numberofcoupon = 1.0;
				break;
		}


		switch (_daycount) {
			case ACT_365:
				numberofdays = TN_REAL(today.serialNumber()-interestaccrualdate.serialNumber());
				numberofdays2 = 365.0/numberofcoupon;
				break;
			case ACT_360:
				numberofdays = TN_REAL(today.serialNumber()-interestaccrualdate.serialNumber());
				numberofdays2 = 360.0/numberofcoupon;
				break;
			case Day30_365:
				numberofdays = TN_REAL(today.dayOfMonth()+30-interestaccrualdate.dayOfMonth()+30*(12*(today.year()-interestaccrualdate.year())+today.month()-interestaccrualdate.month()));
				numberofdays2 = 365.0/numberofcoupon;
				break;
			case Day30_360:
				numberofdays = TN_REAL(today.dayOfMonth()+30-interestaccrualdate.dayOfMonth()+30*(12*(today.year()-interestaccrualdate.year())+today.month()-interestaccrualdate.month()));
				numberofdays2 = 360.0/numberofcoupon;
				break;
			default:
				numberofdays = TN_REAL(today.serialNumber()-dates[i].serialNumber()); 
				numberofdays2 = 365.0/numberofcoupon;
		}
		accruedinterest = numberofdays/numberofdays2*_coupon/numberofcoupon*_faceamount;
	}

	return PV + accruedinterest;
}

Real bond::quotedPrice(Date today){
	
	cashflow CF = getCashflow();
	valarray<Date> dates = CF.getDates();
	valarray<Real> cashflows = CF.getCashflows();

	Natural i=0;
	while((dates[i]<today) && (i<dates.size()))
		i++;

	Real PV = 0;
	if (i==dates.size()) return 0;
	else if (i!=dates.size()-1){
		Natural j;
		Real timetonextCF;
		for(j=i;j<dates.size();j++){
			timetonextCF = today.dayCount(dates[j], _daycount);
			PV += cashflows[j]*_yc.discountFactor(timetonextCF);
		}
	}


	return PV;

}

Real bond::duration(Date today){
	Real yield = yieldToMaturity(today);//, _yc);
	cashflow CF = getCashflow();
	valarray<Date> dates = CF.getDates();
	valarray<Real> cashflows = CF.getCashflows();

	Natural i=0;
	while((dates[i]<today) && (i<dates.size()))
		i++;
	if (i==dates.size()) return 0;
	Real result = 0;
	Real PV = 0;
	Real timetonextCF;
	Natural j;
	for(j=i;j<dates.size();j++){
			timetonextCF = today.dayCount(dates[j], _daycount);
			result += cashflows[j]*exp(-yield*timetonextCF)*timetonextCF;
			PV += cashflows[j]*exp(-yield*timetonextCF);
	}

	result /= PV;
	return result;

}

Real bond::convexity(Date today){
	Real yield = yieldToMaturity(today);//, _yc);
	cashflow CF = getCashflow();
	valarray<Date> dates = CF.getDates();
	valarray<Real> cashflows = CF.getCashflows();

	Natural i=0;
	while((dates[i]<today) && (i<dates.size()))
		i++;
	if (i==dates.size()) return 0;
	Real result = 0;
	Real PV = 0;
	Real timetonextCF;
	Natural j;
	for(j=i;j<dates.size();j++){
			timetonextCF = today.dayCount(dates[j], _daycount);
			result += cashflows[j]*exp(-yield*timetonextCF)*timetonextCF*timetonextCF;
			PV += cashflows[j]*exp(-yield*timetonextCF);
	}

	result /= PV;
	return result;

}

Real bond::yieldToMaturity(Date today){
	Real price = fairvalue(today);//, _yc);
	Real yield = 0.05;
	Real deriv_price, yield_price;


	cashflow CF = getCashflow();
	valarray<Date> dates = CF.getDates();
	valarray<Real> cashflows = CF.getCashflows();

	Natural i=0;
	while((dates[i]<today) && (i<dates.size()))
		i++;
	if (i==dates.size()) return 0;
	
	Real timetonextCF;
	Natural k,j;
	for(k=0;k<100;k++){
		deriv_price = 0;
		yield_price = 0;
		for(j=i;j<dates.size();j++){
			timetonextCF = today.dayCount(dates[j], _daycount);
			yield_price += cashflows[j]*exp(-yield*timetonextCF);
			deriv_price += -timetonextCF*cashflows[j]*exp(-yield*timetonextCF);
		}
		yield += (price - yield_price)/deriv_price;
	}
	return yield;

}

treasurybond::treasurybond(Date issue, Date maturity, Date firstcoupondate, Real coupon, Frequency freq, Real faceamount, DayCountConvention daycount, yieldCurve yc)
:	bond(issue, maturity, firstcoupondate, coupon, freq, faceamount, daycount, yc)
{
}

treasurybond::treasurybond(Date issue, Date maturity, Date firstcoupondate, Real coupon, yieldCurve yc)
:	bond(issue, maturity, firstcoupondate, coupon, Semiannual, 100.0, ACT_360, yc)
{
}

treasurybond::treasurybond(Date issue, Date maturity, Real faceamount, DayCountConvention daycount, yieldCurve yc)
:	bond(issue, maturity, maturity, 0.0, Once, faceamount, daycount, yc)
{
}	

treasurybond treasurybond::shiftedbond(Real shift){
	yieldCurve shiftedyc(_yc.shiftZCBRateCurve(shift));
	treasurybond shiftedtbond(_issue, _maturity, _firstcoupondate, _coupon, _freq, _faceamount, _daycount, shiftedyc);
	return shiftedtbond;
}

Real treasurybond::rho(Date today){
	treasurybond tb = shiftedbond(0.001);
	Real Rho = (tb.fairvalue(today) - fairvalue(today))/0.001;

	return Rho;
}


riskybond::riskybond(Date issue, Date maturity, Date firstcoupondate, Real coupon, Frequency freq, Real faceamount,DayCountConvention daycount, yieldCurve yc,  creditCurve cc)
:	bond(issue, maturity, firstcoupondate, coupon, freq, faceamount, daycount, yc),
	_cc(cc)
{
}

riskybond::riskybond(Date issue, Date maturity, Real faceamount, DayCountConvention daycount, yieldCurve yc,  creditCurve cc)
:	bond(issue, maturity, maturity, 0.0, Once, faceamount, daycount, yc),
	_cc(cc)
{
}

Real riskybond::quotedPrice(Date today){

	cashflow CF = getCashflow();
	valarray<Date> dates = CF.getDates();
	valarray<Real> cashflows = CF.getCashflows();

	Natural i=0;
	while((dates[i]<today) && (i<dates.size()))
		i++;

	Real PV = 0;
	if (i==dates.size()) return 0;
	else if (i!=dates.size()-1){
		Natural j;
		Real timetonextCF;
		for(j=i+1;j<dates.size();j++){
			timetonextCF = today.dayCount(dates[j], _daycount);
			PV += cashflows[j]*_cc.riskyDiscountFactor(timetonextCF);
		}
	}


	return PV;

}

riskybond riskybond::shiftedbond(Real shift){
	yieldCurve shiftedyc(_yc.shiftZCBRateCurve(shift));
	riskybond shiftedtbond(_issue, _maturity, _firstcoupondate, _coupon, _freq, _faceamount, _daycount, shiftedyc, _cc);
	return shiftedtbond;
}

Real riskybond::rho(Date today){
	riskybond tb = shiftedbond(0.001);
	Real Rho = (tb.fairvalue(today) - fairvalue(today))/0.001;

	return Rho;
}
