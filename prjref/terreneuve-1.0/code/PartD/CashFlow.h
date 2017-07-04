#pragma once

#ifndef CASHFLOW_H
#define CASHFLOW_H

#include "./../PartB/yieldCurve.h"
#include "./SwapLeg.h"
#include "./../Common/types.h"
#include <iostream>
class CashFlow
{
public:
	/** Constructor for fixed leg */
	CashFlow(SwapLeg swapLeg, Real fixedRate);

	/** Constructor for float leg by giving a yield curve object */
	CashFlow(SwapLeg swapLeg, yieldCurve floatCurve);

	/** Get the fair value of the swap : discounted value of cash flows */
	Real getFairValue(yieldCurve* curve);

	~CashFlow(void);

private:
	valarray<Real> flowAmount;
	valarray<Date> flowDates;
};

#endif;
