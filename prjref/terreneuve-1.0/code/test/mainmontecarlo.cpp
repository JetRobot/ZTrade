#include "../Interface/main.h"
#include"../PartA/MonteCarlo1/ParkMiller.h"
#include"../PartA/MonteCarlo1/MersenneTwister.h"
#include"../PartA/MonteCarlo1/RandC.h"
#include"../PartA/MonteCarlo1/Sobol.h"
#include"../PartA/MonteCarlo1/GaussianProcess.h"
#include"../common/Normals.h"
#include"../PartA/MonteCarlo1/PayOff.h"
#include"../PartA/MonteCarlo1/Random.h"
#include"../PartB/YieldCurve.h"
#include"../PartA/MonteCarlo1/Drift.h"
#include"../PartA/MonteCarlo1/MCEngine.h"
#include "time.h"

#include<iostream>
#include<valarray>
using namespace std;

bool mainmontecarlo(void) {
    Real spot=100, strike=110, vol=0.2, mat=1.,rate=0.02;
	LongNatural nPaths=100000,nDates=1;
	volsurface* myvol=new volsurface(vol);
	yieldCurve* mycurve=new yieldCurve(rate);
	//Set time
	Real tme;
	tme=clock();
	Real mcPrice=mainmc(mat,strike,spot,myvol,mycurve,nPaths,nDates,1);
	tme=clock()-tme;
	cout<<"MC price"<<endl;
	cout<<mcPrice<<endl;
	cout<<"MC time"<<endl;
	cout<<tme/1000.<<endl;
	if (mcPrice>4.96 || mcPrice<4.92) {
		cout <<"did not get expected result for MC pricer - Part A"<<endl;
		return false;
	} else return true;
}

Real 
mainmc(Real Expiry, Real Strike, Real Spot, volsurface* pvolsurface, yieldCurve* pyieldCurve,
	   LongNatural nPaths, LongNatural nDates, Integer PrdName)
{
	Real initialHazardRate,meanReversionSpeed,price=0;
	valarray <Real> vStepIncrements,vDrift;
	valarray <Real> vPath;
	valarray <Real> vDiscFactors;
	valarray<Real> uniformSample;
	valarray <Real> gaussianSample;
	valarray<LongInteger> vDates;
	LongNatural i,seed;
	
	//seed  
	seed = 100000000;

	//initialisation of arrays
	vPath.resize(nDates);
	vStepIncrements.resize(nDates);
	vDiscFactors.resize(nDates);
	vDrift.resize(nDates);
	vDates.resize(nDates+1);
	uniformSample.resize(nDates);
	gaussianSample.resize(nDates);

	const Real constRate=r;
	initialHazardRate=Spot;
	meanReversionSpeed=0.;

	Date Today=Date();
	Today.setDateToToday();

	//initialisation of drift
	Drift* pDrift = new Drift(Today, Expiry,nDates,pyieldCurve,pvolsurface,Strike);
	vDrift=pDrift->GetvDrift();
	vDates=pDrift->GetvDates();

	//initialisation of uniform number generator
	MersenneTwister* pRandGen = new MersenneTwister(seed);

	//initialisation of random generator
	Random* pRandom = new Random(nDates,pRandGen);
	
	//initialisation of gaussian process
	GaussianProcess* pGaussianProcess = new GaussianProcess(vDates,nDates,initialHazardRate,vDrift,meanReversionSpeed,pvolsurface,Strike);
	
	//initialisation of default jump process 
	pGaussianProcess->GetStepIncrements(vStepIncrements);
	
	//initialisation of payoff function
	PayOff thePayOff(Strike);

	//initialisation of yield curve
	for (i=0;i<nDates;++i)
		vDiscFactors[i] = pyieldCurve->discountFactor((i+1)*Expiry/nDates);

	MCEngine* pMCEngine = new MCEngine(nPaths,nDates,vDiscFactors);
	pMCEngine->RunEngineGeneral(pRandom,pGaussianProcess,thePayOff,gaussianSample,vPath,PrdName);
	price=pMCEngine->MCResult();
	pRandom->SetSeed(seed);

	delete pyieldCurve;
	delete pvolsurface;
	delete pDrift;
	delete pRandom;
	delete pGaussianProcess;

	return price;

}
