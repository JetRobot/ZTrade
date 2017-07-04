#pragma once

#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include "./../PartA/BlackScholes/OptionStrategy.h"
#include "./../PartK/VarianceSwap.h"
#include "./../PartD/VanillaSwap.h"
#include "./../PartI/rainbowoption.h"
#include "./../PartH/bond.h"
#include "./../PartC/asset.h"
#include "./../PartL/Exotics.h"

#include<valarray>
using namespace std;

const Natural MAX_SIZE=100;
const Natural MAX_SIZE_NAME=50;

/**
@author Simon
*/

class Portfolio
{
public:
	/** Default Constructor : just initialization of size
	@param name: Name of the portfolio
	@param currency: Currency of the portfolio
	*/
	Portfolio(char* name,Currency currency);
	~Portfolio(void);

	/** Return name of the portfolio */
	char* getName();

	/** Return currency of the portfolio */
	Currency getCurrency();
	
	/** Return currency of the portfolio as a string */
	char* getCurrencyAsString();

	/** Add one option to the Portfolio */
	void addOptionStrategy(OptionStrategy* optionStrategy);
	
	/** Add one rainbow option to the Portfolio */
	void addRainbowOption(RainbowOption* rainbowOption, Real quantity);

	/** Add one exotic option to the Portfolio */
	void addExoticOption(Exotics* exoticOption, Real quantity);

	/** Add one vanilla swap to the Portfolio */
	void addVanillaSwap(VanillaSwap* vanillaSwap, Real quantity);
	
	/** Add one variance swap to the Portfolio */
	void addVarianceSwap(VarianceSwap* varSwap, Real quantity);
	
	/** Add one bond to the Portfolio */
	void addBond(bond* oneBond, Real quantity);

	/** Add one asset to the Portfolio */
	void addAsset(asset* oneAsset, Real quantity);

	/** Return Price of the whole Portfolio */
	Real getPrice();

	/** Return sensibility to interest rate */
	Real returnSensibilityToRate();

	/** Return sensibility to volatility */
	Real returnSensibilityToVol();

	/** Return sensibility to time */
	Real returnSensibilityToTime();


private:

	char* _name;
	Currency _currency;

	OptionStrategy _optionStrategy;
	valarray<RainbowOption*> _rainbowOptions;
	valarray<Exotics*> _exoticsOptions;
	valarray<VanillaSwap*> _vanSwaps;
	valarray<VarianceSwap*> _varSwaps;
	valarray<bond*> _bonds;
	valarray<asset*> _assets;

	valarray<Real> _quantityRainbowOptions;
	valarray<Real> _quantityExoticsOptions;
	valarray<Real> _quantityVanSwaps;
	valarray<Real> _quantityVarSwaps;
	valarray<Real> _quantityBonds;
	valarray<Real> _quantityAssets;

	Natural _nbRainbowOptions;
	Natural _nbExoticsOptions;
	Natural _nbVanSwaps;
	Natural _nbVarSwaps;
	Natural _nbBonds;
	Natural _nbAssets;
};

#endif;
