#include ".\VanillaSwap.h"

VanillaSwap::VanillaSwap(CashFlow cashflowReceived1, CashFlow cashflowPaid2, char* name1, char* name2, yieldCurve* curve)
:	_leg1(cashflowReceived1),
	_leg2(cashflowPaid2),
	_curve(curve)
{
	_name1=new char[MAX_LETTERS];
	_name2=new char[MAX_LETTERS];
	for (ShortNatural i=0;i<MAX_LETTERS;i++) {
		_name1[i]=name1[i];
		_name2[i]=name2[i];
	}
}

VanillaSwap::~VanillaSwap(void)
{
}

Real VanillaSwap::getFairValue1() {
	return _leg1.getFairValue(_curve);
}

Real VanillaSwap::getFairValue2() {
	return _leg2.getFairValue(_curve);
}

Real VanillaSwap::returnPrice() {
	Real result=0.;
	result=getFairValue1()-getFairValue2();
	return result;
}

Real VanillaSwap::getRho() {
	Real result=returnPrice();
	_curve->shiftZCBRateCurve();
	result=returnPrice()-result;
	_curve->shiftZCBRateCurve(-defaultshiftfactorForShortRate);
	return result;
}

Real VanillaSwap::getTheta() {
	Real result=returnPrice();
	yieldCurve curve=*_curve;
	_curve->forwardZCBCurve(1./365);
	result=returnPrice()-result;
	_curve=&curve;
	return result;
}
