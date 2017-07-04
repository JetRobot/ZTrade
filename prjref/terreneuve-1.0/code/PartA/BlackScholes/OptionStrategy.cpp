#include ".\optionstrategy.h"

OptionStrategy::OptionStrategy(void)
{
	_nbOptions=0;
	_insideOptions.resize(maxNbOptions);
	_insideQuantities.resize(maxNbOptions);
}

OptionStrategy::~OptionStrategy(void)
{
}

Real OptionStrategy::returnPrice()
{
	return _price;
}

Real OptionStrategy::recalcPrice()
{
	Real result=0;
	for (unsigned int i=0;i<_nbOptions;i++) {
		result+=_insideQuantities[i]*_insideOptions[i]->getPrice();
	}
	_price=result;
	return result;
}

void OptionStrategy::addOneOptionToStrategy(Real spot, Real vol,bool isVol, Real r, Real K, Real T, TypeOptionBS type, Real Quantity) {
	BlackScholes* bs= new BlackScholes(spot,vol,isVol,r,K,T,type);
	_insideOptions[_nbOptions]=bs;
	_insideQuantities[_nbOptions]=Quantity;
	_price+=Quantity*bs->getPrice();
	_nbOptions++;
}

void OptionStrategy::addOneBlackScholesObject(BlackScholes* bs, Real Quantity) {
	_insideOptions[_nbOptions]=bs;
	_insideQuantities[_nbOptions]=Quantity;
	_price+=Quantity*bs->getPrice();
	_nbOptions++;
}

void OptionStrategy::addLongCallSpread(Real spot,Real volStrike1,bool isVol1,Real volStrike2,bool isVol2,Real r, Real K1, Real K2, Real T,Real Quantity) {
	/** We search to be long the call with smallest strike and short the other */
	if (K2>=K1) {
		addOneOptionToStrategy(spot,volStrike1,isVol1,r,K1,T,Call,Quantity);
		addOneOptionToStrategy(spot,volStrike2,isVol2,r,K2,T,Call,-Quantity);
	}
	else {
		addOneOptionToStrategy(spot,volStrike1,isVol1,r,K1,T,Call,-Quantity);
		addOneOptionToStrategy(spot,volStrike2,isVol2,r,K2,T,Call,Quantity);
	}
}

void OptionStrategy::addLongStraddle(Real spot,Real vol,bool isVol, Real r, Real K, Real T,Real Quantity) {
	addOneOptionToStrategy(spot,vol,isVol,r,K,T,Call,Quantity);
	addOneOptionToStrategy(spot,vol,isVol,r,K,T,Put,Quantity);
}

void OptionStrategy::addLongStrangle(Real spot,Real volStrike1,bool isVol1,Real volStrike2,bool isVol2,Real r, Real K1, Real K2, Real T,Real Quantity) {
	if (K2>=K1) {
		addOneOptionToStrategy(spot,volStrike1,isVol1,r,K1,T,Put,Quantity);
		addOneOptionToStrategy(spot,volStrike2,isVol2,r,K2,T,Call,Quantity);
	}
	else {
		addOneOptionToStrategy(spot,volStrike2,isVol2,r,K2,T,Put,Quantity);
		addOneOptionToStrategy(spot,volStrike1,isVol1,r,K1,T,Call,Quantity);
	}
}

void OptionStrategy::addLongButterflySpread(Real spot,Real volStrike1,bool isVol1,Real volStrike2,bool isVol2,Real volStrike3,bool isVol3, Real r, Real K1, Real K2, Real T,Real Quantity) {
	addOneOptionToStrategy(spot, volStrike3, isVol3, r, (K1+K2)/2, T, Call, -2*Quantity);
	addOneOptionToStrategy(spot, volStrike1, isVol1, r, K1, T, Call, Quantity);
	addOneOptionToStrategy(spot, volStrike2, isVol2, r, K2, T, Call, Quantity);
}

void OptionStrategy::addLongButterflySpread(Real spot,Real volStrike1,bool isVol1,Real volStrike2,bool isVol2,Real volStrike3,bool isVol3, Real r, Real K1, Real K2, Real K3, Real T,Real Quantity) {
	addOneOptionToStrategy(spot, volStrike3, isVol3, r, K3, T, Call, -2*Quantity);
	addOneOptionToStrategy(spot, volStrike1, isVol1, r, K1, T, Call, Quantity);
	addOneOptionToStrategy(spot, volStrike2, isVol2, r, K2, T, Call, Quantity);
}

void OptionStrategy::addLongRatioCallSpread(Real spot,Real volStrike1,bool isVol1,Real volStrike2,bool isVol2, Real r, Real K1, Real K2, Real T,Real Quantity) {
	if (K2>=K1) {
		addOneOptionToStrategy(spot,volStrike1,isVol1,r,K1,T,Call,Quantity);
		addOneOptionToStrategy(spot,volStrike2,isVol2,r,K2,T,Call,-2*Quantity);
	}
	else {
		addOneOptionToStrategy(spot,volStrike1,isVol1,r,K1,T,Call,-2*Quantity);
		addOneOptionToStrategy(spot,volStrike2,isVol2,r,K2,T,Call,Quantity);
	}
}

void OptionStrategy::addLongPutSpread(Real spot,Real volStrike1,bool isVol1,Real volStrike2,bool isVol2,Real r, Real K1, Real K2, Real T,Real Quantity) {
	if (K2>=K1) {
		addOneOptionToStrategy(spot,volStrike1,isVol1,r,K1,T,Put,-Quantity);
		addOneOptionToStrategy(spot,volStrike2,isVol2,r,K2,T,Put,Quantity);
	}
	else {
		addOneOptionToStrategy(spot,volStrike1,isVol1,r,K1,T,Put,Quantity);
		addOneOptionToStrategy(spot,volStrike2,isVol2,r,K2,T,Put,-Quantity);
	}
}

Real OptionStrategy::getGlobalDelta() {
	Real Result=0;
	for (unsigned int i=0;i<_nbOptions;i++) {
		Result+=_insideQuantities[i]*(_insideOptions[i])->getDelta();
	}
	return Result;
}

Real OptionStrategy::getGlobalGamma() {
	Real Result=0;
	for (unsigned int i=0;i<_nbOptions;i++) {
		Result+=_insideQuantities[i]*(_insideOptions[i])->getGamma();
	}
	return Result;
}

Real OptionStrategy::getGlobalVega() {
	Real Result=0;
	for (unsigned int i=0;i<_nbOptions;i++) {
		Result+=_insideQuantities[i]*(_insideOptions[i])->getVega();
	}
	return Result;
}

Real OptionStrategy::getGlobalTheta() {
	Real Result=0;
	for (unsigned int i=0;i<_nbOptions;i++) {
		Result+=_insideQuantities[i]*(_insideOptions[i])->getTheta();
	}
	return Result;
}

Real OptionStrategy::getGlobalRho() {
	Real Result=0;
	for (unsigned int i=0;i<_nbOptions;i++) {
		Result+=_insideQuantities[i]*(_insideOptions[i])->getRho();
	}
	return Result;
}


Natural OptionStrategy::returnNbOptions() const{
	return _nbOptions;
}

BlackScholes* OptionStrategy::returnOption(Natural i) const{
	if (i<_insideOptions.size()) return _insideOptions[i];
	else return NULL;
}

Real OptionStrategy::returnOptionQuantity(Natural i) const{
	if (i<_insideOptions.size()) return _insideQuantities[i];
	else return NULL;
}

void OptionStrategy::changeRate(Real addConstant) {
	for (unsigned int i=0;i<_nbOptions;i++) {
		BlackScholes& bs=*_insideOptions[i];
		bs.changeRate(bs.getRate()+addConstant);
	}
}

void OptionStrategy::changeVol(Real addConstant) {
	for (unsigned int i=0;i<_nbOptions;i++) {
		BlackScholes& bs=*_insideOptions[i];
		bs.changeVol(bs.getVolatility()+addConstant);
	}
}

void OptionStrategy::changeMaturity(Real addConstant) {
	for (unsigned int i=0;i<_nbOptions;i++) {
		BlackScholes& bs=*_insideOptions[i];
		bs.changeMaturity(bs.getMaturity()+addConstant);
	}
}

void OptionStrategy::changeSpot(Real addConstant) {
	for (unsigned int i=0;i<_nbOptions;i++) {
		BlackScholes& bs=*_insideOptions[i];
		bs.changeSpot(bs.getSpot()+addConstant);
	}
}

void OptionStrategy::changeStrike(Real addConstant) {
	for (unsigned int i=0;i<_nbOptions;i++) {
		BlackScholes& bs=*_insideOptions[i];
		bs.changeStrike(bs.getStrike()+addConstant);
	}
}

ostream& operator << (ostream &os, const OptionStrategy &optionStrategy) {
	os << "Qty,\tType,\tSpot,\tStrike,\tVolatility,\tRate,\t\tExp" << endl;
	char* type=new char[4];
	for (Natural i=0;i<optionStrategy.returnNbOptions(); i++) {
		if (optionStrategy.returnOption(i)->isCall())	sprintf(type,"Call");
		else sprintf(type,"Put");
		os << optionStrategy.returnOptionQuantity(i)<<",\t" << type << ",\t" << optionStrategy.returnOption(i)->getSpot() << ",\t" << optionStrategy.returnOption(i)->getStrike() << ",\t" << optionStrategy.returnOption(i)->getVolatility() << ",\t" << optionStrategy.returnOption(i)->getRate() << ",\t"<< optionStrategy.returnOption(i)->getMaturity() << endl;
	}
	return os;
}
