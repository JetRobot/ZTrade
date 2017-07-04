#pragma once
#ifndef PAYOFF_H
#define PAYOFF_H

#include "../../common/types.h"
#include <valarray>
using namespace std;

class PayOff
{
public:

	/** Default constructor: set Strike */
    PayOff(Real Strike_);

	/** void constructor 
	@author Yann */
	PayOff(void);

	void SetStrike(Real Strike_);

	/** Nice operator for european call options only */
    virtual Real operator()(Real Spot) const;
    virtual ~PayOff(){}
	
	/** Return the payoff of a call */
	virtual Real Call (Real Fwd);
	
	/** Return the payoff of a put */
	virtual Real Put (Real Fwd);
	
	/** Return the payoff of an asian call */
	virtual Real AsianCall (valarray<Real> Path,LongNatural nDates);
	
	/** Return the payoff of an asian put */
	virtual Real AsianPut (valarray<Real> Path,LongNatural nDates);
	
	/** Return the payoff of a look back call */
	virtual Real RevLookbackCall (valarray<Real> Path,LongNatural nDates);
	
	/** Return the payoff of a look back put */
	virtual Real RevLookbackPut (valarray<Real> Path,LongNatural nDates);

	/** Return the payoff of a barrier call */
	virtual Real BarrierCall (valarray<Real> Path,LongNatural nDates);

	/** Return the payoff of a barrier put */
	virtual Real BarrierPut (valarray<Real> Path,LongNatural nDates);

	/** Return the payoff of a floor */
	virtual Real FlooredCliquet (Real Spot, Real Fwd);
	
	/** Return the payoff of a cap */
	virtual Real CappedCliquet (Real Spot, Real Fwd);

	/** @author Yann - Rainbow Payoffs using Simon's framework*/
	/** @return the payoff of a Spread option with 2 assets */
	virtual Real Rainbow2SpreadOptionMax(Real Fwd1,Real Fwd2,Real W1,Real W2,Real Mult);

	/** @return the payoff of a basket option with 2 assets */
	virtual Real Rainbow2AssetsBasketMax(Real Fwd1,Real Fwd2,Real W1,Real W2,Real Mult);

	/** @return the payoff of a best of + cash option with 2 assets */
	virtual Real RainbowBestOf2AssetsCash(Real Fwd1,Real Fwd2,Real W1,Real W2);

	/** @return the payoff of a worst of + cash option with 2 assets */
	virtual Real RainbowWorstOf2AssetsCash(Real Fwd1,Real Fwd2,Real W1,Real W2);

	/** @return the payoff of a Max call option with 2 assets */
	virtual Real RainbowMax2AssetsCall(Real Fwd1,Real Fwd2,Real W1,Real W2,Real Mult);

	/** @return the payoff of a Min call option with 2 assets */
	virtual Real RainbowMin2AssetsCall(Real Fwd1,Real Fwd2,Real W1,Real W2,Real Mult);

	/** @return the payoff of a Max put option with 2 assets */
	virtual Real RainbowMax2AssetsPut(Real Fwd1,Real Fwd2,Real W1,Real W2,Real Mult);

	/** @return the payoff of a Min put option with 2 assets */
	virtual Real RainbowMin2AssetsPut(Real Fwd1,Real Fwd2,Real W1,Real W2,Real Mult);

	/** @return the payoff of a Convertible bond 
		
		If CallPrice and PutPrice are omitted they are set to infinity and zero
		respectively so that they don't factor into the calculation */
	virtual Real Convertible(Real Fwd, Real ConversionRatio, Real BondPrice, Real CallPrice = TN_INFINITY, Real PutPrice = 0);

private:
    Real Strike;
};

#endif
