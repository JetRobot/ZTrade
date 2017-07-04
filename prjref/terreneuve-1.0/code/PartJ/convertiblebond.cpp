#include ".\convertiblebond.h"
#include ".\binomialTree.h"
#include "../PartA/MonteCarlo1/PayOff.h"

convertiblebond::convertiblebond(asset Stock, 
								 riskybond Bond, 
								 Real conversionRatio, 
								 Natural nSteps, 
								 Real callPrice, 
								 Real putPrice) :
riskybond(Bond), 
_bond(Bond),
_stock(Stock), 
_conversionRatio(conversionRatio), 
_n(nSteps), 
_callPrice(callPrice), 
_putPrice(putPrice) {
	_priceCached = 
		_deltaCached = 
		_interestRateDeltaCached = 
		_gammaCached = 
		_btCached =
		false;

	// cout << "DEBUG convertible underlying yc: " << _yc << endl;
	// cout << "issue: " << _issue.toString() << " maturity: " << _maturity.toString() << endl;;
}

convertiblebond::~convertiblebond(void) { 
}

Real
convertiblebond::fairvalue(Date today){
	if ((_priceCached) && (_datepriceCached == today)) {
		return _price;
	}

	if (_btCached) {
		delete _bt;
	}
	_bt = 
		new binomialTree(_stock, 
		_yc, getMaturityInYears(today), _n);
	PayOff *payoff = 
		new PayOff();

	_bt->runEngineConvertibleBond(*payoff, 
		_conversionRatio / getFaceAmount() * 100, 
		_callPrice, 
		_putPrice);

	_btCached = true;

	// cout << "DEBUG convertible pricing tree: " << endl << _bt << endl;

	const valarray<Real> *prices = _bt->getClaimProcess(0);
	_price = (*prices)[0];
	_priceCached = true;
	_datepriceCached = today;

	return _price;
}

Real
convertiblebond::delta(Date today) const {
	if ((_deltaCached) && (_datedeltaCached == today)) {
		return _delta;
	}

	// create a new convertible bond with delta stock price
	Real So = _stock.getPrice();
	// workaround to preserve const-correctness
	Real price = const_cast<convertiblebond *>(this)->fairvalue(today);
	Real Soplus = So * (1 + 0.01);
	asset *stockDelta = new asset(Soplus, _stock.GetVolatility());
	
	// instantiate a new convertiblebond
	convertiblebond *cbDelta = 
		new convertiblebond(*stockDelta,
		_bond, 
		_conversionRatio, 
		_n, 
		_callPrice, 
		_putPrice);

	Real priceplus = cbDelta->fairvalue(today);
	
	_delta = (priceplus - price)/(Soplus - So);
	_deltaCached = true;
	_datedeltaCached = today;

	delete cbDelta;
	delete stockDelta;

	return _delta;
}

convertiblebond
convertiblebond::shiftedcbond(Real shift) {
	riskybond shiftedrbond = _bond.shiftedbond(shift);
	convertiblebond shiftedcbond = 
		convertiblebond(_stock, shiftedrbond, _conversionRatio, _n, _callPrice, _putPrice);
	return shiftedcbond;
}

Real 
convertiblebond::interestRateDelta(Date today) const {
	if ((_interestRateDeltaCached) && (_dateinterestRateDeltaCached == today)) {
		return _interestRateDelta;
	}

	convertiblebond cb = 
		const_cast<convertiblebond *>(this)->shiftedcbond(0.001);
	_interestRateDelta = (cb.fairvalue(today) - 
		const_cast<convertiblebond *>(this)->fairvalue(today))/0.001;
	_interestRateDeltaCached = true;
	_dateinterestRateDeltaCached = today;

	return _interestRateDelta;
}

Real 
convertiblebond::gamma(Date today) const {
	if ((_gammaCached) && (_dategammaCached == today)) {
		return _gamma;
	}

	// create a new convertible bond with delta stock price
	Real So = _stock.getPrice();
	Real deltao = delta(today);
	Real Soplus = So * (1 + 0.01);
	asset *stockDelta = new asset(Soplus, _stock.GetVolatility());
	
	// instantiate a new convertiblebond
	convertiblebond *cbDelta = 
		new convertiblebond(*stockDelta,
		_bond, 
		_conversionRatio, 
		_n, 
		_callPrice, 
		_putPrice);

	Real deltaplus = cbDelta->delta(today);

	_gamma = (deltaplus - deltao)/(Soplus - So);
	_gammaCached = true;
	_dategammaCached = today;

	delete cbDelta;
	delete stockDelta;

	return _gamma;
}

ostream& 
operator << (ostream &os, convertiblebond &cb) {
	os << "convertible bond - price: " << cb.fairvalue() << endl;
	os << " issue: " << cb._issue.toString() << endl;
	os << " maturity: " << cb._maturity.toString() << endl;
	os << " face: " << cb._faceamount << endl;
	os << " stock price: " << cb._stock.getPrice() << endl;
	os << " stock vol: " << cb._stock.GetVolatility() << endl;
	os << " conversion ratio: " << cb._conversionRatio << endl;
	os << " callprice: ";
	if (cb._callPrice == CB_DEFAULT_CALLPRICE) {
		os << "n/a" << endl;
	} else {
		os << cb._callPrice << endl;
	}
	os << " putprice: ";
	if (cb._putPrice == CB_DEFAULT_PUTPRICE) {
		os << "n/a" << endl;
	} else {
		os << cb._putPrice << endl;
	}
	os << " nsteps: " << cb._n << endl;
	os << " delta: " << cb.delta() << endl;
	os << " paritydelta: " << cb.parityDelta() << endl;
	os << " interestratedelta: " << cb.interestRateDelta() << endl;
	os << " gamma: " << cb.gamma() << endl;
	os << " paritygamma: " << cb.parityGamma() << endl;
	if (cb._btCached) {
		os << " underlying binomial tree: " << endl;
		os << cb._bt << endl;
	}

	return os;
}