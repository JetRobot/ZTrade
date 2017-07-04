#pragma once
#ifndef productsCreation_h
#define productsCreation_h
#include"../PartB/yieldCurve.h"
#include"../PartE/volsurface.h"
#include "../PartA/BlackScholes/BlackScholes.h"
#include "../PartA/BlackScholes/OptionStrategy.h"
#include "../PartL/Exotics.h"
#include "../PartH/bond.h"
#include "../PartD/VanillaSwap.h"
#include "../PartI/rainbowoption.h"
#include "../PartJ/convertiblebond.h"
#include "./importData.h"
#include <minmax.h>

using namespace std;

/**
@author Yann
*/

/** Menu for this category */
bool productsCreationMenu(marketData data);

/** User interface to input and store a BS option */
BlackScholes* inputBSOption(marketData data);
/** Transform the input c/C/p/P into "Call" or "Put" */
string outputCallPut(char c);


/** User interface to input and store an option strategy and options in it */
OptionStrategy inputOptionStrategy(marketData data);
/** add something else to the strategy than a Call/Put */
void inputSpecificOptionStrategy(marketData data,OptionStrategy& strategy);
/** add a butterfly spread to the strategy */
void inputButterflySpread(marketData data,OptionStrategy& strategy,bool useMarketData);
/** add a call spread to the strategy */
void inputCallSpread(marketData data,OptionStrategy& strategy,bool useMarketData);
/** add a put spread to the strategy */
void inputPutSpread(marketData data,OptionStrategy& strategy,bool useMarketData);
/** add a ratio call spread to the strategy */
void inputRatioCallSpread(marketData data,OptionStrategy& strategy,bool useMarketData);
/** add a Straddle to the strategy */
void inputStraddle(marketData data,OptionStrategy& strategy,bool useMarketData);
/** add a strangle to the strategy */
void inputStrangle(marketData data,OptionStrategy& strategy,bool useMarketData);

/** User interface to input and store an Exotic Option with a single underlying (MC Price) */
Exotics* inputExoticOptionOnSingleAsset(marketData &data);
/** converts the 1 to 9 choice in a Exotics Type*/
exoticsType choiceToType(Natural choice);

/** User interface to input a bond */
bond* inputBond(marketData &data);
/** User interface to input a convertible bond */
convertiblebond* inputConvertibleBond(marketData &data);
/** User interface to input an interest rate vanilla swap */
VanillaSwap* inputVanillaSwap(marketData data);

/** For Rainbow options, converts the input into a priceType*/
priceType choosePricingType(Natural choice);
/** For Rainbow options, converts the input into a rainbowType*/
rainbowType chooseRainbowType(Natural choice);
/** To input a Rainbow Option*/
RainbowOption* inputRainbowOption(marketData data);


#endif