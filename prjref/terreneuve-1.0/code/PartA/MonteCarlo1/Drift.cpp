#include "../../common\types.h"
#include "./drift.h"
#include <math.h>

Drift::Drift(Date startDate, Real ExpiryInYears,LongNatural& nDates,yieldCurve* pyieldCurve,volsurface* pvolsurface, Real Strike)
									:	m_nDates(nDates)
{
	vDates.resize(nDates+1);
	Date stepDate=Date();
	//Build dates with constant step
	for (Natural i=0;i<nDates+1;++i) {
		stepDate=startDate.plusDays((Natural)(365*ExpiryInYears*i/nDates));
		vDates[i]=stepDate.serialNumber();
	}
	//Memory allocation for vDrift
	vDrift.resize(nDates);
	//Build drift path according to Ito's formula
	Date startStep,endStep;
	for (i=0;i<nDates;++i) {
		startStep=Date(vDates[i]);
		endStep=Date(vDates[i+1]);
		vDrift[i]=(pyieldCurve->forwardRate(startStep,endStep)-0.5*pow(pvolsurface->forwardVolatility(Strike,startStep,endStep),2))*(vDates[i+1]-vDates[i])/365;
	}
}

Drift::Drift(void)
{
}

Drift::Drift(Date startDate, Real ExpiryInYears,Real rateToMaturity,Real volToMaturity)
{
	m_nDates=1;
	vDates.resize(m_nDates+1);
	Date stepDate=Date();
	//Build dates with constant step -- useless but allocation for the getvDates necessary for Gaussian process.
	for (Natural i=0;i<m_nDates+1;++i) {
		stepDate=startDate.plusDays((Natural)(365*ExpiryInYears*i/m_nDates));
		vDates[i]=stepDate.serialNumber();
	}

	//Memory allocation for vDrift
	vDrift.resize(m_nDates);
	//Build drift path according to Ito's formula
	vDrift[0]=(rateToMaturity-0.5*pow(volToMaturity,2))*ExpiryInYears;
}

Drift::~Drift()
{
}

valarray<Real> Drift::GetvDrift(void)
{
	valarray<Real> Driftarray;
	Driftarray.resize(m_nDates);
	for (Natural i=0;i<m_nDates;++i)
		Driftarray[i]=vDrift[i];
	return Driftarray;
}

Real Drift::GetDriftattimei(LongNatural i)
{
	if (i<m_nDates)
		return vDrift[i];
	else
		return 0.;
}

valarray<LongInteger> Drift::GetvDates(void)
{
	valarray<LongInteger> Datesarray;
	Datesarray.resize(m_nDates+1);
	for (Natural i=0;i<m_nDates+1;++i){
		Datesarray[i]=vDates[i];
	}
	return Datesarray;
}

LongInteger Drift::GetTimeBtwDates(LongNatural i,LongNatural j)
{
	if (i<m_nDates && j<m_nDates)
		return vDates[j]-vDates[i];
	else
		return 0;
}