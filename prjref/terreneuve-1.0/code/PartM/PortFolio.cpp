#include "./PortFolio.h"

Portfolio::Portfolio(char* name,Currency currency)
{
	char* _name=new char[MAX_SIZE_NAME];
	_name=name;
	_currency=currency;
	_nbRainbowOptions=0;
	_nbVanSwaps=0;
	_nbVarSwaps=0;
	_nbBonds=0;
	_nbAssets=0;
	_optionStrategy=OptionStrategy();
	_rainbowOptions.resize(MAX_SIZE);
	_vanSwaps.resize(MAX_SIZE);
	_varSwaps.resize(MAX_SIZE);
	_bonds.resize(MAX_SIZE);
	_assets.resize(MAX_SIZE);
}

Portfolio::~Portfolio(void)
{
}

char* Portfolio::getName() {
	return _name;
}

Currency Portfolio::getCurrency() {
	return _currency;
}

char* Portfolio::getCurrencyAsString() {
	char* result=new char[MAX_SIZE_NAME];
	switch (_currency) {
		case (USD):
			sprintf(result,"USD");
			break;
		case (EUR):
			sprintf(result,"EUR");
			break;
		case (CAD):
			sprintf(result,"CAD");
			break;
		default:
			break;
	}
	return result;
}

void Portfolio::addOptionStrategy(OptionStrategy* optionStrategy) {
	_optionStrategy=*optionStrategy;
}

void Portfolio::addRainbowOption(RainbowOption* rainbowOption, Real quantity) {
	_rainbowOptions[_nbRainbowOptions]=rainbowOption;
	_quantityRainbowOptions[_nbRainbowOptions]=quantity;
	_nbRainbowOptions++;
}

void Portfolio::addExoticOption(Exotics* exoticOption, Real quantity) {
	_exoticsOptions[_nbExoticsOptions]=exoticOption;
	_quantityExoticsOptions[_nbExoticsOptions]=quantity;
	_nbExoticsOptions++;
}

void Portfolio::addVanillaSwap(VanillaSwap* vanillaSwap, Real quantity) {
	_vanSwaps[_nbVanSwaps]=vanillaSwap;
	_quantityVanSwaps[_nbVanSwaps]=quantity;
	_nbVanSwaps++;
}

void Portfolio::addVarianceSwap(VarianceSwap* varSwap, Real quantity) {
	_varSwaps[_nbVarSwaps]=varSwap;
	_quantityVarSwaps[_nbVarSwaps]=quantity;
	_nbVarSwaps++;
}

void Portfolio::addBond(bond* oneBond, Real quantity) {
	_bonds[_nbBonds]=oneBond;
	_quantityBonds[_nbBonds]=quantity;
	_nbBonds++;
}

void Portfolio::addAsset(asset* oneAsset, Real quantity) {
	_assets[_nbAssets]=oneAsset;
	_quantityAssets[_nbAssets]=quantity;
	_nbAssets++;
}

Real Portfolio::getPrice() {
	Real result=0.;
	result+=_optionStrategy.returnPrice();
	for (Integer i=0;i<(Integer)_nbRainbowOptions;i++) {
		result+=_rainbowOptions[i]->getPrice();
	}
	for (Integer i=0;i<(Integer)_nbExoticsOptions;i++) {
		result+=_exoticsOptions[i]->getPrice();
	}
	for (Integer i=0;i<(Integer)_nbVarSwaps;i++) {
		result+=_varSwaps[i]->getPrice();
	}
	for (Integer i=0;i<(Integer)_nbVanSwaps;i++) {
		result+=_vanSwaps[i]->returnPrice();
	}
	for (Integer i=0;i<(Integer)_nbBonds;i++) {
		result+=_bonds[i]->fairvalue(Date());
	}
	for (Integer i=0;i<(Integer)_nbAssets;i++) {
		result+=_assets[i]->getPrice();
	}
	return result;
}

Real Portfolio::returnSensibilityToRate() {
	Real result=0;
	result+=_optionStrategy.getGlobalRho();
	for (Integer i=0;i<(Integer)_nbRainbowOptions;i++) {
		result+=_rainbowOptions[i]->getRho();
	}
	for (Integer i=0;i<(Integer)_nbExoticsOptions;i++) {
		result+=_exoticsOptions[i]->getRho();
	}
	for (Integer i=0;i<(Integer)_nbVanSwaps;i++) {
		result+=_vanSwaps[i]->getRho();
	}
	for (Integer i=0;i<(Integer)_nbVarSwaps;i++) {
		result+=_varSwaps[i]->getRho();
	}
	for (Integer i=0;i<(Integer)_nbBonds;i++) {
		result+=_bonds[i]->duration(Date());
	}
	for (Integer i=0;i<(Integer)_nbAssets;i++) {
		result+=_assets[i]->getRho(1.); //TOBECHANGED******************
	}
	return result;
}

Real Portfolio::returnSensibilityToVol() {
	Real result=0;
	result+=_optionStrategy.getGlobalVega();
	for (Integer i=0;i<(Integer)_nbRainbowOptions;i++) {
		result+=_rainbowOptions[i]->getVega();
	}
	for (Integer i=0;i<(Integer)_nbExoticsOptions;i++) {
		result+=_exoticsOptions[i]->getVega();
	}
	for (Integer i=0;i<(Integer)_nbVarSwaps;i++) {
		result+=_varSwaps[i]->getVega();
	}
	return result;
}

Real Portfolio::returnSensibilityToTime() {
	Real result=0;
	result+=_optionStrategy.getGlobalTheta();
	for (Integer i=0;i<(Integer)_nbRainbowOptions;i++) {
		result+=_rainbowOptions[i]->getTheta();
	}
	for (Integer i=0;i<(Integer)_nbExoticsOptions;i++) {
		result+=_exoticsOptions[i]->getTheta();
	}
	for (Integer i=0;i<(Integer)_nbVanSwaps;i++) {
		result+=_vanSwaps[i]->getTheta();
	}
	for (Integer i=0;i<(Integer)_nbVarSwaps;i++) {
		result+=_varSwaps[i]->getTheta();
	}
	return result;
}
