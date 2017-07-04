#pragma once
#ifndef GAUSSIANROCESS_H
#define GAUSSIANPROCESS_H

#include "../../common/Date.h"
#include "../../common/types.h"
#include "../../PartE/volsurface.h"

#include<valarray>
using namespace std;


class GaussianProcess
{
public:
	/** Default constructor
	@param schedule: Start Date of the drift
	@param nDates: Number of dates to simulate the path
	@param initialRate: 
	@param pyieldCurve: Yield Curve to compute the spot rate between each date
	@param pvolsurface: Volatility Surface to compute the forward volatility between each date
	@param Strike: Strike of the option, used to compute the colatility (function of the strike)
	*/
	GaussianProcess(const valarray<LongInteger> schedule,const LongNatural& nDates,const Real& initialRate,const valarray<Real> drift,const Real &meanReversionSpeed,Real &vol);

	GaussianProcess(const valarray<LongInteger> schedule,const LongNatural& nDates,const Real& initialRate,const valarray<Real> drift,const Real &meanReversionSpeed,volsurface* vol,Real strike);

	GaussianProcess(void);
	~GaussianProcess();

	/** Build the Path according to given gaussian shocks */
	valarray<Real> BuildPath(valarray<Real> gaussianShocks);

	/** For a 1D, only one termial Point  (non path dependant)
	@author Yann 
	@param gaussianShock: the shock that we'll get from the sample given drift, etc*/
	Real BuildTerminalPoint(Real gaussianShock);

	/** Return Step Increments */
	void GetStepIncrements(valarray<Real> stepIncrements);

private:
	valarray<LongInteger>	m_vDates;//size is m_nDates
	valarray<Real>	m_vDrift;//size is m_nDates

	valarray<Real> m_vStepSize;//size is m_nDates
	Real	m_dbMeanReversionSpeed;
	Real m_dbVol;
	volsurface*	_vol;
	Real _strike;
	Real  m_dbInitialRate;
	LongNatural	m_nDates;//number of Dates excluding 0
};

#endif
