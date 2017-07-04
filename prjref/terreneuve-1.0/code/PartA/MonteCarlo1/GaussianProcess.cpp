#include "GaussianProcess.h"
#include <math.h>
#include <cmath>
#include<valarray>
using namespace std;

GaussianProcess::GaussianProcess(const valarray<LongInteger> schedule,
									 const LongNatural& nDates,
									 const Real &initialRate,
									 const valarray<Real> drift,
									 const Real &meanReversionSpeed,
									 Real &vol)
{
	m_vDates.resize(nDates+1);
	m_vDrift.resize(nDates);
	m_vDates=schedule;
	m_vDrift=drift;
	m_dbMeanReversionSpeed = meanReversionSpeed;
	m_dbVol = vol;
	m_dbInitialRate = initialRate;
	m_nDates = nDates;
	m_vStepSize.resize(m_nDates);
	
	for(Natural i=0;i<nDates;++i)
		m_vStepSize[i] = (schedule[i+1]-schedule[i])/365;
}

GaussianProcess::GaussianProcess(const valarray<LongInteger> schedule,
									 const LongNatural& nDates,
									 const Real &initialRate,
									 const valarray<Real> drift,
									 const Real &meanReversionSpeed,
									 volsurface* vol,
									 Real strike)
{
	m_vDates.resize(nDates+1);
	m_vDrift.resize(nDates);
	m_vDates=schedule;
	m_vDrift=drift;
	m_dbMeanReversionSpeed = meanReversionSpeed;
	_vol = vol;
	m_dbVol=-1.;
	m_dbInitialRate = initialRate;
	m_nDates = nDates;
	m_vStepSize.resize(m_nDates);
	_strike=strike;
	
	for(Natural i=0;i<nDates;++i)
		m_vStepSize[i] = (schedule[i+1]-schedule[i])/365;
}

GaussianProcess::GaussianProcess(void)
{
}

GaussianProcess::~GaussianProcess()
{
}

valarray<Real> GaussianProcess::BuildPath(valarray<Real> gaussianShocks)
{
	valarray<Real> path;
	path.resize(m_nDates);
	Real RandomPath,logRandomPath;
	RandomPath = m_dbInitialRate;
	logRandomPath = log(RandomPath);
	if (m_dbVol=-1.) {
		Date t=Date();
		for (LongNatural i=0;i<m_nDates;++i)
		{
			t=Date(m_vDates[i]);
			logRandomPath += (m_vDrift[i]-m_dbMeanReversionSpeed*logRandomPath) + _vol->volatility(_strike,t)*gaussianShocks[i]*sqrt(Real(m_vDates[i+1]-m_vDates[i])/365);
			path[i] = exp(logRandomPath);
		}
	}
	else {
		for (LongNatural i=0;i<m_nDates;++i)
		{
			logRandomPath += (m_vDrift[i]-m_dbMeanReversionSpeed*logRandomPath) + m_dbVol*gaussianShocks[i]*sqrt(Real(m_vDates[i+1]-m_vDates[i])/365);
			path[i] = exp(logRandomPath);
		}
	}
	return path;
}

Real GaussianProcess::BuildTerminalPoint(Real gaussianShock){
	Real path;
	Real logRandomPath;
	logRandomPath = log(m_dbInitialRate)+(m_vDrift[0]) + m_dbVol*gaussianShock*sqrt(Real((m_vDates[1]-m_vDates[0])/365));
	path = exp(logRandomPath);
	return path;
}


void GaussianProcess::GetStepIncrements(valarray<Real> stepIncrements)
{
	LongNatural i;
	for(i=0;i<m_nDates;++i)
		stepIncrements[i] = m_vStepSize[i];
}