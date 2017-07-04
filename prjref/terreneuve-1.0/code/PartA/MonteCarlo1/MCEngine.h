#pragma once

#ifndef MCENGINE_H
#define MCENGINE_H

#include "Random.h"
#include "GaussianProcess.h"
#include "PayOff.h"
#include "../../common/Normals.h"
#include "../../common/matrix.h"


class MCEngine
{
public:
	/** Default constructor
	@param price: Start Date of the drift
	@param nPaths: Length of the drift to be computed
	@param nDates: Number of dates to cut the drift
	@param Vol: Yield Curve to compute the spot rate between each date
	@param Spot: Volatility Surface to compute the forward volatility between each date
	@param Strike: Strike of the option, used to compute the colatility (function of the strike)
	*/
	MCEngine(LongNatural nPaths,LongNatural nDates,valarray<Real> DiscFactors);
	~MCEngine();

	/** Default constructor
	@author Yann */ 
	MCEngine(void);
	/** for 1 date (non path dependant payOffs)
	@author Yann 
	@param nPaths: to compute the MC
	@param DFToMaturity: 1 date so only one Df needed
	*/ 
	MCEngine(LongNatural nPaths,Real DFToMaturity);

	/** @author Yann - Rainbow MC Engines using Simon's framework*/
	/** Price Spread option with 2 assets */
	void RunEngineRainbow2SpreadOptionMax(Random* pRandom,valarray<GaussianProcess> pHazardRateProcesses,PayOff thePayOff,Real gaussianSample,valarray<Real> TerminalPoints,valarray<Real> weights,Matrix Correlation,Real Mult);
	
	/** Price basker option with 2 assets */
	void RunEngineRainbow2AssetsBasketMax(Random* pRandom,valarray<GaussianProcess> pHazardRateProcesses,PayOff thePayOff,Real gaussianSample,valarray<Real> TerminalPoints,valarray<Real> weights,Matrix Correlation,Real Mult);
	
	/** Price best of + cash option with 2 assets */
	void RunEngineRainbowBestOf2AssetsCash(Random* pRandom,valarray<GaussianProcess> pHazardRateProcesses,PayOff thePayOff,Real gaussianSample,valarray<Real> TerminalPoints,valarray<Real> weights,Matrix Correlation);
	
	/** Price worst of + cash option with 2 assets */
	void RunEngineRainbowWorstOf2AssetsCash(Random* pRandom,valarray<GaussianProcess> pHazardRateProcesses,PayOff thePayOff,Real gaussianSample,valarray<Real> TerminalPoints,valarray<Real> weights,Matrix Correlation);
	
	/** Price max call option with 2 assets */
	void RunEngineRainbowMax2AssetsCall(Random* pRandom,valarray<GaussianProcess> pHazardRateProcesses,PayOff thePayOff,Real gaussianSample,valarray<Real> TerminalPoints,valarray<Real> weights,Matrix Correlation,Real Mult);
	
	/** Price min call option with 2 assets */
	void RunEngineRainbowMin2AssetsCall(Random* pRandom,valarray<GaussianProcess> pHazardRateProcesses,PayOff thePayOff,Real gaussianSample,valarray<Real> TerminalPoints,valarray<Real> weights,Matrix Correlation,Real Mult);
	
	/** Price max put option with 2 assets */
	void RunEngineRainbowMax2AssetsPut(Random* pRandom,valarray<GaussianProcess> pHazardRateProcesses,PayOff thePayOff,Real gaussianSample,valarray<Real> TerminalPoints,valarray<Real> weights,Matrix Correlation,Real Mult);
	
	/** Price min put option with 2 assets */
	void RunEngineRainbowMin2AssetsPut(Random* pRandom,valarray<GaussianProcess> pHazardRateProcesses,PayOff thePayOff,Real gaussianSample,valarray<Real> TerminalPoints,valarray<Real> weights,Matrix Correlation,Real Mult);

	/** Price Asian Call */
	void RunEngineAsianCall(Random* pRandom,GaussianProcess* pHazardRateProcess,PayOff thePayOff,valarray <Real> gaussianSample,valarray<Real> vHazardRatePath);
	
	/** Price Asian Put */
	void RunEngineAsianPut(Random* pRandom,GaussianProcess* pHazardRateProcess,PayOff thePayOff,valarray <Real> gaussianSample,valarray<Real> vHazardRatePath);
	
	/** Price European standard Call */
	void RunEngineCall(Random* pRandom,GaussianProcess* pHazardRateProcess,PayOff thePayOff,valarray<Real> gaussianSample,valarray<Real> vHazardRatePath);
	
	/** Price uropean standard Put */
	void RunEnginePut(Random* pRandom,GaussianProcess* pHazardRateProcess,PayOff thePayOff,valarray <Real> gaussianSample,valarray<Real> vHazardRatePath);
	
	/** Price Lokback Call */
	void RunEngineRevLookbackCall(Random* pRandom,GaussianProcess* pHazardRateProcess,PayOff thePayOff,valarray <Real> gaussianSample,valarray<Real> vHazardRatePath);
	
	/** Price Lokback Put */
	void RunEngineRevLookbackPut(Random* pRandom,GaussianProcess* pHazardRateProcess,PayOff thePayOff,valarray <Real> gaussianSample,valarray<Real> vHazardRatePath);

	/** Price Barrier Call */
	void RunEngineBarrierCall(Random* pRandom,GaussianProcess* pHazardRateProcess,PayOff thePayOff,valarray <Real> gaussianSample,valarray<Real> vHazardRatePath);
	
	/** Price Barrier Put */
	void RunEngineBarrierPut(Random* pRandom,GaussianProcess* pHazardRateProcess,PayOff thePayOff,valarray <Real> gaussianSample,valarray<Real> vHazardRatePath);

	/** Price FlooredCliquet */
	void RunEngineFlooredCliquet(Random* pRandom,GaussianProcess* pHazardRateProcess,PayOff thePayOff,valarray <Real> gaussianSample,valarray<Real> vHazardRatePath);

	/** Price CappedCliquet */
	void RunEngineCappedCliquet(Random* pRandom,GaussianProcess* pHazardRateProcess,PayOff thePayOff,valarray <Real> gaussianSample,valarray<Real> vHazardRatePath);

	/** Run Monte Carlo Engine with code for the product */
	void RunEngineGeneral(Random* pRandom,GaussianProcess* pHazardRateProcess,PayOff thePayOff,valarray<Real> gaussianSample,valarray<Real> vHazardRatePath,Natural Product);
	
	/** Return result of Monte carlo simulation */
	Real MCResult();

private:
	Real m_price,m_DiscFactor;
	LongNatural m_nPaths;
	LongNatural m_nDates;
};

#endif
