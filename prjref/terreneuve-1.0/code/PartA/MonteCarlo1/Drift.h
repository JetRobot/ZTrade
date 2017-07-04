#pragma once
#ifndef DRIFT_H
#define DRIFT_H

#include "../../PartB/yieldCurve.h"
#include "../../PartE/volsurface.h"
#include "../../common/date.h"
/**
@author Simon
*/
class Drift
{
public:
	/** Default constructor
	@param startDate: Start Date of the drift
	@param ExpiryInYears: Length of the drift to be computed
	@param nDates: Number of dates to cut the drift
	@param pyieldCurve: Yield Curve to compute the spot rate between each date
	@param pvolsurface: Volatility Surface to compute the forward volatility between each date
	@param Strike: Strike of the option, used to compute the colatility (function of the strike)
	*/
	Drift(Date startDate, Real ExpiryInYears,LongNatural& nDates,yieldCurve* pyieldCurve,volsurface* pvolsurface,Real Strike);

	/**
	Default constructor needed 
	@author Yann
	*/
	Drift(void);
	/**
	Simpler constructor - for 1 date (non path dependant payoffs, not need to have yc and volsurface 
	@author Yann
	@param startDate: Start Date of the drift
	@param ExpiryInYears: Length of the drift to be computed
	@param rateToMaturity: spot rate to matrurity - can come from a yc in calling classes
	@param volToMaturity: volatility to matrurity - can come from a volsurface in calling classes
	*/
	Drift(Date startDate, Real ExpiryInYears,Real rateToMaturity,Real volToMaturity);

	~Drift();
	/** Return Drift */
	valarray<Real> GetvDrift(void);
	/** Get Drift for time i */
	Real GetDriftattimei(LongNatural i);
	/** Return serial numbers of dates computed for the drift */
	valarray<LongInteger> GetvDates(void);
	/** Return Difference in days between two dates of the drift*/
	LongInteger GetTimeBtwDates(LongNatural i,LongNatural j);

private:
	valarray<LongInteger> vDates;
	valarray<Real> vDrift;
	unsigned long m_nDates;
};

#endif
