#pragma once

#include "./CashFlow.h"

static const ShortNatural MAX_LETTERS=30;
static const Real defaultprecisionInPrice=1.;

class VanillaSwap
{
public:
	/** Default constructor : create a vanilla swap with two legs */
	VanillaSwap(CashFlow cashflowReceived1, CashFlow cashflowPaid2, char* name1, char* name2,yieldCurve* curve);
	~VanillaSwap(void);

	/** Return fair value of first leg */
	Real getFairValue1();

	/** Return fair value of second leg */
	Real getFairValue2();

	/** Return price of the swap */
	Real returnPrice();

	/** Return sensitivity to the interest rate */
	Real getRho();

	/** Return sensitivity to the time */
	Real getTheta();

private:
	yieldCurve* _curve;
	CashFlow _leg1;
	CashFlow _leg2;
	char* _name1;
	char* _name2;
};
