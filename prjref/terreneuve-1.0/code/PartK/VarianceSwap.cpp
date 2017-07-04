#include "./VarianceSwap.h"

VarianceSwap::VarianceSwap(OptionStrategy* options, Real maturity, Real forwardprice)
	:	_options(options),
		_maturity(maturity),
		_forward(forwardprice)
{
}

VarianceSwap::~VarianceSwap(void)
{
}

Real VarianceSwap::getPrice() {
	Real result=0;
	//Create vectors to store indexes of options to use
	valarray<Integer> putsToUse;
	valarray<Integer> callsToUse;
	putsToUse.resize(_options->returnNbOptions());
	callsToUse.resize(_options->returnNbOptions());
	for (Natural i=0;i<putsToUse.size();i++) {
		putsToUse[i]=-1;
		callsToUse[i]=-1;
	}

	Integer countCalls=0;
	Integer countPuts=0;
	BlackScholes* bs;
	BlackScholes* bs2;
	Real Strike1;
	Real Strike2;

	//Search _options for options to use
	for (Integer i=0;i<(Integer)_options->returnNbOptions();i++) {
		bs=_options->returnOption(i);
		if (bs->isCall() && (bs->getStrike()>=_forward)) {
			callsToUse[countCalls]=i;
			countCalls++;
		}
		if (!(bs->isCall()) && (bs->getStrike()<=_forward)) {
			putsToUse[countPuts]=i;
			countPuts++;
		}
	}

	//Compute the formula
	for (Integer i=0;i<countPuts-1;i++) {
		bs2=_options->returnOption(putsToUse[i+1]);
		bs=_options->returnOption(putsToUse[i]);
		Strike1=bs->getStrike();
		Strike2=bs2->getStrike();
		result+=(Strike2-Strike1)*1/(Strike1*Strike1)*bs->getPrice();
	}
	if (countCalls>0 && countPuts>0) {
		bs=_options->returnOption(putsToUse[countPuts-1]);
		bs2=_options->returnOption(callsToUse[0]);
		Strike1=bs->getStrike();
		Strike2=bs2->getStrike();
		result+=(Strike2-Strike1)*1/(Strike1*Strike1)*bs->getPrice();
	}
	for (Integer i=0;i<countCalls-1;i++) {
		bs2=_options->returnOption(callsToUse[i+1]);
		bs=_options->returnOption(callsToUse[i]);
		Strike1=bs->getStrike();
		Strike2=bs2->getStrike();
		result+=(Strike2-Strike1)*1/(Strike1*Strike1)*bs->getPrice();
	}

	return 2./_maturity*result;
}

Real VarianceSwap::getRho(Real shiftCurve) {
	double result=getPrice();
	_options->changeRate(shiftCurve);
	result=getPrice()-result;
	return result;
}

Real VarianceSwap::getVega(Real shiftVol) {
	double result=getPrice();
	_options->changeVol(shiftVol);
	result=getPrice()-result;
	return result;
}

Real VarianceSwap::getTheta(Real shiftMat) {
	double result=getPrice();
	_options->changeMaturity(shiftMat);
	result=getPrice()-result;
	return result;
}
