#pragma once
#ifndef rainbowoption_h
#define rainbowoption_h

#include "../common/types.h"
#include "../common/matrix.h" 
#include "../common/date.h" 
#include "..\PartB\yieldCurve.h"
#include "..\PartE\volsurface.h"
#include <valarray> 

#include"../PartA/MonteCarlo1/MersenneTwister.h"
#include"../PartA/MonteCarlo1/GaussianProcess.h"
#include"../PartA/MonteCarlo1/PayOff.h"
#include"../PartA/MonteCarlo1/Random.h"
#include"../PartA/MonteCarlo1/Drift.h"
#include"../PartA/MonteCarlo1/MCEngine.h"
#include"../PartA/MonteCarlo1/PayOff.h"

#include "../PartA/BlackScholes/BlackScholes.h"

#define RO_DEFAULT_STRIKE 100
#define RO_DEFAULT_VOL 0.15
#define RO_DEFAULT_RATE 0.02
#define RO_DEFAULT_MATURITY 1
#define RO_DEFAULT_NB_ASSETS 2
#define RO_DEFAULT_MULTIPLIER 1

#define RO_NPATHS 100000
#define RO_SEED 100000000

#define EPSILON 0.00000001
#define GREEKAPPROX 0.01

using namespace std;
/**
@author Yann
// to simplify, only non path dependant Rainbows, and no quanto
*/

enum priceType {
	MonteCarlo,		// 
	ClosedForm		//
};

enum rainbowType {
	SpreadOptionMax,
	AssetsBasketMax,
	BestOf2AssetsCash,
	WorstOf2AssetsCash,
	BetterOf2Assets,
	WorseOf2Assets,
	Max2AssetsCall,
	Min2AssetsCall,
	Max2AssetsPut,
	Min2AssetsPut
};

class RainbowOption
{
public:
	/**
		The default constructor will instantiate a rainbow such with :
		- a non correlated basket 
		- of RO_DEFAULT_NB_ASSETS assets, 
		- equally weighted, and
		- with RO_DEFAULT_MULTIPLIER
	*/
	RainbowOption(void);

	/**
		Full general constructor with n assets
	*/
	RainbowOption(rainbowType type,Date startDate,Real expiry,Real Strike,yieldCurve yc,valarray<volsurface> vols,valarray <Real> spots=valarray<Real>(RO_DEFAULT_STRIKE,RO_DEFAULT_NB_ASSETS),Real Multiplier=RO_DEFAULT_MULTIPLIER,Matrix Correl = IdentityMatrix(RO_DEFAULT_NB_ASSETS),valarray<Real> weights=valarray<Real>(1/(Real)RO_DEFAULT_NB_ASSETS,RO_DEFAULT_NB_ASSETS),bool outputMsgs=false);

	/**
		For 2 assets
		@param: Real Correl: the correlation between 1 and 2 - default is 0
	*/
	RainbowOption(rainbowType type,Date start,Real exp,Real Strike,yieldCurve yc,valarray<volsurface> vols,Real Spot1,Real Spot2,Real Mult=RO_DEFAULT_MULTIPLIER,Real Correl12=0,Real weight1=0.5,Real weight2=0.5,bool outputMsgs=false);
	~RainbowOption(void);

	/**
		@param price type : MC (default as for this one we have all prices) - or CF
		@param nPaths - only be used for MC, so default
	*/
	Real getPrice(priceType priceMethod=ClosedForm,LongNatural nPaths=RO_NPATHS);

	/** by convention, security 1 is the 0th spot in the array, so user "logical"*/
	Real getPartialDelta(Natural security,priceType priceMethod=ClosedForm);

	/** by convention, security 1 is the 0th spot in the array, so user "logical"*/
	Real getPartialGamma(Natural security,priceType priceMethod=ClosedForm);

	/** by convention, security 1 is the 0th spot in the array, so user "logical"*/
	Real getPartialVega(Natural security,priceType priceMethod=ClosedForm);

	/** @return overall delta (if all the market shifts : sum of deltas)*/
	Real getDelta(priceType priceMethod=ClosedForm);

	/** @return overall gamma (if all the market shifts : sum of gamma)*/
	Real getGamma(priceType priceMethod=ClosedForm);

	/** @return overall vega (if all the market shifts : sum of Vega)*/
	Real getVega(priceType priceMethod=ClosedForm);

	/** Correl Risk*/
	Real getCorrelRisk(priceType priceMethod=ClosedForm);

	/** Rho risk*/
	Real getRho(priceType priceMethod=ClosedForm);

	/** Theata risk*/
	Real getTheta(priceType priceMethod=ClosedForm);

	rainbowType getRainbowType(){return _type;}
	void setRainbowType(rainbowType newType){_type=newType;}

private:
	// MC Prices
	/*1 */Real PriceByMc_2SpreadOptionMax(LongNatural nPaths=RO_NPATHS);
	/*2 */Real PriceByMc_2AssetsBasketMax(LongNatural nPaths=RO_NPATHS);
	/*3 */Real PriceByMc_BestOf2AssetsCash(LongNatural nPaths=RO_NPATHS);
	/*4 */Real PriceByMc_WorstOf2AssetsCash(LongNatural nPaths=RO_NPATHS);
	/*5 */Real PriceByMc_BetterOf2Assets(LongNatural nPaths=RO_NPATHS);
	/*6 */Real PriceByMc_WorseOf2Assets(LongNatural nPaths=RO_NPATHS);
	/*7 */Real PriceByMc_Max2AssetsCall(LongNatural nPaths=RO_NPATHS);
	/*8 */Real PriceByMc_Min2AssetsCall(LongNatural nPaths=RO_NPATHS);
	/*9 */Real PriceByMc_Max2AssetsPut(LongNatural nPaths=RO_NPATHS);
	/*10*/Real PriceByMc_Min2AssetsPut(LongNatural nPaths=RO_NPATHS);

	// Closed forms
	/*1 */ // no Spread - see Rubinstein
	/*2 */ // no Basket - see Rubinstein
	/*3 */Real PriceByClosedForm_BestOf2_plusCash();
	/*4 */ // no worse of + cash
	/*5 */Real PriceByClosedForm_BetterOf2();
	/*6 */Real PriceByClosedForm_WorseOf2();
	/*7 */Real PriceByClosedForm_MaxOf2_call();
	/*8 */Real PriceByClosedForm_MinOf2_call();
	/*9 */Real PriceByClosedForm_MaxOf2_put();
	/*10*/Real PriceByClosedForm_MinOf2_put();



private:
	// output messages ?
	bool _outputMsgs;

	// general members
	Date _startDate;
	Real _expiryInYears;
	Natural _NumberOfAssets;
	Real _Strike;
	valarray <Real> _spots;   
	Real _Multiplier;
	valarray <Real> _weights;   
	Matrix _CorrelationMatrix; // useless for 2 assets, but if we have an eye on more than 2, will be useful
	valarray<Real> _volatilities;
	valarray<volsurface> _volatilitiesSurfaces; // need to store these 2, as for the spread, the vol is NOT from the strike. say ATM
	yieldCurve _yc; // no quanto so risk free rate is unique.

	rainbowType _type;


	void reassignVolsAtThemoney();
	void reassignVolsAtThestrike();

	// for MC Pricing
	PayOff _thePayOff;
	Real _DFTomaturity;
	valarray<Drift> _Drifts;
	Random *_pRandom;
	MCEngine _MCEngine;
	Real _gaussianSample;
	valarray<Real> _TerminalPoints;
	valarray<GaussianProcess> _pHazardRateProcesses;

	void instanciateMCVariables();
	LongNatural _seed;

	//For closed form solutions for 2 assets
	Real sigmaA;
	void compute_sigmaA();
	Real rho1;
	void compute_rho1();
	Real rho2;
	void compute_rho2();
	Real d1;
	void compute_d1();
	Real d2;
	void compute_d2();
	Real d3;
	void compute_d3();
	Real d4;
	void compute_d4();
	Real A;
	void compute_A();
	Real B;
	void compute_B();
	Real C;
	void compute_C();

	bool haveClosedFormVariablesBeenComputed;
	void compute_ClosedFormsParameters();


};
#endif

