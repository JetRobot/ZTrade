#include "./Exotics.h"

Exotics::Exotics(exoticsType type,yieldCurve* curve,volsurface* surface,Real spot,Real strike,
		Real Expiry,LongNatural nDates,Real strike2,LongNatural nPaths)
		:	_type(type),
			_yieldCurve(curve),
			_volSurface(surface),
			_spot(spot),
			_strike(strike),
			_strike2(strike2),
			_expiry(Expiry),
			_nPaths(nPaths),
			_nDates(nDates)
{
}

Exotics::~Exotics(void)
{
}

Real Exotics::getPrice() {
	switch (_type) {
		case AsianCall:
			return mainmc(_expiry,_strike,_spot,_volSurface,_yieldCurve,_nPaths,_nDates,3);
		case AsianPut:
			return mainmc(_expiry,_strike,_spot,_volSurface,_yieldCurve,_nPaths,_nDates,4);
		case RevLookbackCall:
			return mainmc(_expiry,_strike,_spot,_volSurface,_yieldCurve,_nPaths,_nDates,5);
		case RevLookbackPut:
			return mainmc(_expiry,_strike,_spot,_volSurface,_yieldCurve,_nPaths,_nDates,6);
		case BarrierCall:
			return mainmc(_expiry,_strike,_spot,_volSurface,_yieldCurve,_nPaths,_nDates,7);
		case BarrierPut:
			return mainmc(_expiry,_strike,_spot,_volSurface,_yieldCurve,_nPaths,_nDates,8);
		case FlooredCliquet:
			return mainmc(_expiry,_strike,_spot,_volSurface,_yieldCurve,_nPaths,_nDates,9);
		case CappedCliquet:
			return mainmc(_expiry,_strike,_spot,_volSurface,_yieldCurve,_nPaths,_nDates,10);
		case CollaredCliquet:
			return mainmc(_expiry,_strike,_spot,_volSurface,_yieldCurve,_nPaths,_nDates,9)+mainmc(_expiry,_strike2,_spot,_volSurface,_yieldCurve,_nPaths,_nDates,10);
		default:
			return 0.;
	}
}

Real Exotics::getDelta() {
	Real result=0.;
	result=getPrice();
	_spot*=1.01;
	result-=getPrice();
	_spot/=1.01;
	return result;
}

Real Exotics::getRho() {
	Real result=0.;
	result=getPrice();
	_yieldCurve->shiftZCBRateCurve();
	result-=getPrice();
	_yieldCurve->shiftZCBRateCurve(-defaultshiftfactorForShortRate);
	return result;
}

Real Exotics::getTheta() {
	Real result=0.;
	result=getPrice();
	yieldCurve yield=*_yieldCurve;
	_yieldCurve->forwardZCBCurve(defaultAdvDays/365);
	volsurface volsurf=*_volSurface;
	Date advDate=Date();
	advDate.setDateToToday();
	advDate.plusDays((Integer)defaultAdvDays);
	_volSurface=&_volSurface->forwardvolsurface(advDate);
	result-=getPrice();
	_yieldCurve=&yield;
	_volSurface=&volsurf;
	return result;
}

Real Exotics::getVega() {
	Real result=0.;
	result=getPrice();
	_volSurface->shiftedvolsurface(defaultShiftVolSurface);
	result-=getPrice();
	_volSurface->shiftedvolsurface(-defaultShiftVolSurface);
	return result;
}
