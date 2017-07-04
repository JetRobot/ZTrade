#pragma once

#ifndef OPTIONSTRATEGY_H
#define OPTIONSTRATEGY_H

#include "BlackScholes.h"
#include "../../common/types.h"

#include <valarray>

using namespace std;

static const Natural maxNbOptions=500;
static const Real defaultshiftRate=0.0001;
static const Real defaultshiftVol=0.0001;
static const Real defaultshiftMat=0.01; // 1/100 of a year
static const Real defaultshiftSpot=0.0001;
static const Real defaultshiftStrike=0.0001;

class OptionStrategy
{
/** display parameters of options in the optionstrategy object 
	@param os: the output stream to direct output to
	@param optionStrategy: the option strategy to display
	@returns output stream as is standard for operator<<
	*/
	friend ostream& operator << (ostream &os, const OptionStrategy& optionStrategy);
	friend ostream& operator << (ostream &os, const OptionStrategy* optionStrategy) {
		return os << *optionStrategy;
	}

public:
	/** Default constructor: initialize parameters */
	OptionStrategy();
	~OptionStrategy();
	/** Generic function to add Options to the Strategy */
	void addOneOptionToStrategy(Real spot, Real vol,bool isVol, Real r, Real K, Real T,TypeOptionBS type,Real Quantity);
	void addOneBlackScholesObject(BlackScholes* bs,Real Quantity);

	/** Create a long call spread in the portfolio */
	void addLongCallSpread(Real spot,Real volStrike1,bool isVol1,Real volStrike2,bool isVol2,Real r, Real K1, Real K2, Real T,Real Quantity);
	/** Create a long straddle in the portfolio */
 	void addLongStraddle(Real spot,Real vol,bool isVol, Real r, Real K, Real T,Real Quantity);
	/** Create a long strangle in the portfolio */
	void addLongStrangle(Real spot,Real volStrike1,bool isVol1,Real volStrike2,bool isVol2,Real r, Real K1, Real K2, Real T,Real Quantity);
	/** Create a butterfly in the portfolio */
	/** There are two functions for butterfly if you want to specify or not K3=(K1+K2)/2 by default */
	void addLongButterflySpread(Real spot,Real volStrike1,bool isVol1,Real volStrike2,bool isVol2,Real volStrike3,bool isVol3, Real r, Real K1, Real K2, Real T,Real Quantity);
	void addLongButterflySpread(Real spot,Real volStrike1,bool isVol1,Real volStrike2,bool isVol2,Real volStrike3,bool isVol3, Real r, Real K1, Real K2, Real K3, Real T,Real Quantity);
	/** Create a long ratio call spread in the portfolio */
	void addLongRatioCallSpread(Real spot,Real volStrike1,bool isVol1,Real volStrike2,bool isVol2, Real r, Real K1, Real K2, Real T,Real Quantity);
	/** Create a long put spread in the portfolio */
	void addLongPutSpread(Real spot,Real volStrike1,bool isVol1,Real volStrike2,bool isVol2,Real r, Real K1, Real K2, Real T,Real Quantity);

	/** Get Greeks for global Portfolio */
	Real getGlobalDelta();
	Real getGlobalGamma();
	Real getGlobalVega();
	Real getGlobalTheta();
	Real getGlobalRho();

	/** Return global price of the portfolio */
	Real returnPrice();
	/** Recalculate global price of the portfolio in case of change*/
	Real recalcPrice();
	/** Return number of options in the portfolio */
	Natural returnNbOptions() const;

	/** Return pointer on blackscholes object inside, used for variance swaps */
	BlackScholes* returnOption(Natural i) const;

	/** Return quantity on blackscholes object inside */
	Real returnOptionQuantity(Natural i) const;

	/** Add constant rate to the inside rate of all BlackScholes objects */
	void changeRate(Real addConstant=defaultshiftRate);

	/** Add constant vol to the inside vol of all BlackScholes objects */
	void changeVol(Real addConstant=defaultshiftVol);

	/** Add constant maturity to the inside maturity of all BlackScholes objects */
	void changeMaturity(Real addConstant=defaultshiftMat);

	/** Add constant spot to the inside spot of all BlackScholes objects */
	void changeSpot(Real addConstant=defaultshiftSpot);

	/** Add constant strike to the inside strike of all BlackScholes objects */
	void changeStrike(Real addConstant=defaultshiftStrike);

private:
	Real _price;
	Natural _nbOptions;
	valarray <BlackScholes*> _insideOptions;
	valarray <Real> _insideQuantities;
};

#endif

