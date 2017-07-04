#include ".\asset.h"


/*------------------------------------------------------*/
/*                 Flow Natural Implementation           */
/*------------------------------------------------------*/

flowSchedule::flowSchedule(void) 
{
	_dateOfFlowPayment=Date();
	_FlowAmountInPercent=0;
	_businessDayConventionOnPaymentDate=Unadjusted;
}

flowSchedule::flowSchedule(Date date,Real percent,BusinessDayConvention bd)
{
	_dateOfFlowPayment=date;
	_FlowAmountInPercent=percent;
	_businessDayConventionOnPaymentDate=bd;                           
}

flowSchedule::~flowSchedule(void)
{
};


/*------------------------------------------------------*/
/*                 Asset Natural Implementation           */
/*------------------------------------------------------*/


asset::asset(void)
{
	_currentPrice=0.0;
	_yc=yieldCurve();
	_areDividendsAsGrowingRate=false;
	_dividendGrowingRate=0.0;
	_announcedDividendFlows=valarray<flowSchedule>();
	_denomCur=USD;
	_volatility=ASSET_DEFAULT_VOL;
}

asset::asset(Real price,yieldCurve yc,bool areDividendsAsRate,Real divRate,valarray<flowSchedule> flowsc,Currency ccy, Real volatility)
{
	_currentPrice=price;
	_yc=yc;
	_areDividendsAsGrowingRate=areDividendsAsRate;
	_dividendGrowingRate=divRate;
	_announcedDividendFlows=flowsc;
	_denomCur=ccy;
	_volatility=volatility;
}

asset::asset(Real price, Real volatility)
{
	_currentPrice=price;
	_volatility=volatility;

	// defaults
	_yc=yieldCurve();
	_areDividendsAsGrowingRate=false;
	_dividendGrowingRate=0.0;
	_announcedDividendFlows=valarray<flowSchedule>();
	_denomCur=USD;
}

Real asset::getRho(Real T){
	Real pu,pd;
	yieldCurve old=_yc,up=_yc.shiftZCBRateCurve(+0.001),down=_yc.shiftZCBRateCurve(-0.001);
	_yc=up;
	pu=forwardPrice(T);
	_yc=down;
	pd=forwardPrice(T);
	_yc=old;
	return (pu-pd)/0.002;
}

Real asset::forwardPrice(Real T){
	Date start;
	start.setDateToToday ();
	return forwardPrice (start.plusDays((Natural)(365*T)));
}

void asset::setDivAsRate(Real rate)
{
	_areDividendsAsGrowingRate=true;
	_dividendGrowingRate =rate;
}

Real asset::Price()
{
	return _currentPrice;
}



Real asset::forwardPrice(Date maturityDate){
	if(_areDividendsAsGrowingRate){
		Date today=Date();
		today.setDateToToday();
		Real timetomaturity=today.dayCount(maturityDate,Day30_360);		
		return _currentPrice*exp(-timetomaturity*_dividendGrowingRate);
	}
	else{
		Real sum=0;
		Natural i = 0;
		while (i<_announcedDividendFlows.size() && _announcedDividendFlows[i].getDate()<=maturityDate){
			sum+=_announcedDividendFlows[i].getAmount()*_yc.discountFactor(_announcedDividendFlows[i].getDate());
			i++;
		}
		return _currentPrice*(1-sum);
	}
}

asset::~asset(void)
{
}
