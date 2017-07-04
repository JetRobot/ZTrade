#include "./cashflow.h"

CashFlow::CashFlow(SwapLeg swapLeg, Real fixedRate)
{
	flowAmount=swapLeg.returnAmounts();
	flowDates=swapLeg.returnDates();
	Date d;
	d.setDateToToday();
	flowAmount[0]=flowAmount[0]*fixedRate*(flowDates[0].serialNumber()-d.serialNumber())/365.0;
	for (Natural i=1;i<flowDates.size();i++) {
		flowAmount[i]=flowAmount[i]*fixedRate*(flowDates[i].serialNumber()-flowDates[i-1].serialNumber())/365.0;
	}
}

CashFlow::CashFlow(SwapLeg swapLeg, yieldCurve floatCurve)
{
	flowAmount=swapLeg.returnAmounts();
	flowDates=swapLeg.returnDates();
	Date d;
	d.setDateToToday();
	flowAmount[0]=flowAmount[0]*floatCurve.forwardRate(d,flowDates[0],Discrete)*(flowDates[0].serialNumber()-d.serialNumber())/365.0;
	for (Natural i=1;i<flowDates.size();i++) {
		flowAmount[i]=flowAmount[i]*floatCurve.forwardRate(flowDates[i-1],flowDates[i],Discrete)*(flowDates[i].serialNumber()-flowDates[i-1].serialNumber())/365.0;
	}
}

Real CashFlow::getFairValue(yieldCurve* curve)
{
	Real result=0;
	for (Natural i=0;i<flowAmount.size();i++) {
		result+=flowAmount[i]*curve->discountFactor(flowDates[i],Discrete);
	}
	return result;
}

CashFlow::~CashFlow(void)
{
}
