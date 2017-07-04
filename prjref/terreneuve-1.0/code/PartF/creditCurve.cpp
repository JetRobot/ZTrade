#include ".\creditCurve.h"
#include "..\common\utils.h"
#include <cmath>

CreditSpreadPoint::CreditSpreadPoint(void) : 
	_rate(0), _maturity(0), _spreadtype(Relative) { }
CreditSpreadPoint::CreditSpreadPoint(Real r, Real T, CreditSpreadType t) : 
	_rate(r), _maturity(T), _spreadtype(t) { }
CreditSpreadPoint::~CreditSpreadPoint(void) { }

char *CreditSpreadPoint::TypeAsString(CreditSpreadType t)
{
	switch (t) {
		case Absolute:
			return "Abs";
		case Relative:
			return "Rel";
		default:
			return "No spread type has been defined";
	}
}


creditCurve::creditCurve(void) :
 yieldCurve(),
 _spreads(CC_MAX_NUM_SPREADS),
 _survivalProbability(CC_MAX_NUM_SPREADS),
 _defaultProbability(CC_MAX_NUM_SPREADS),
 _swapFees(CC_MAX_NUM_SPREADS) {
}

creditCurve::creditCurve(yieldCurve &yc, 
						 valarray<CreditSpreadPoint> &cp, 
						 char *name,
						 Real recoveryRate,
						 Currency currency,
						 Frequency frequency) :
_spreads(CC_MAX_NUM_SPREADS),
_survivalProbability(CC_MAX_NUM_SPREADS),
_defaultProbability(CC_MAX_NUM_SPREADS),
_swapFees(CC_MAX_NUM_SPREADS),
_recoveryRate(recoveryRate),
_currency(currency),
_frequency(frequency) {

	// make a local copy of the underlying yieldcurve - its ours now!
	_underlying = new yieldCurve(yc);	
	yieldCurve *spreadCurve = createSpreadCurve(*_underlying, cp);
	_combined = combineUnderlyingAndSpreads(*_underlying, *spreadCurve);
	resampleSpread();

	delete spreadCurve;
}

creditCurve::creditCurve(Real flatRate,
						 Real flatSpread,
						 char *name,
						 Real recoveryRate,
						 Currency currency,
						 Frequency frequency) :
_spreads(CC_MAX_NUM_SPREADS),
_survivalProbability(CC_MAX_NUM_SPREADS),
_defaultProbability(CC_MAX_NUM_SPREADS),
_swapFees(CC_MAX_NUM_SPREADS),
_recoveryRate(recoveryRate),
_currency(currency),
_frequency(frequency) {

	_underlying = new yieldCurve(flatRate);
	yieldCurve *spreadCurve = new yieldCurve(flatSpread);
	_combined = combineUnderlyingAndSpreads(*_underlying, *spreadCurve);
	assignFlatSpread(flatSpread);
	
	delete spreadCurve;
}

creditCurve::creditCurve(yieldCurve &yc,
						 Real flatSpread,
						 char *name,
						 Real recoveryRate,
						 Currency currency,
						 Frequency frequency) :
_spreads(CC_MAX_NUM_SPREADS),
_survivalProbability(CC_MAX_NUM_SPREADS),
_defaultProbability(CC_MAX_NUM_SPREADS),
_swapFees(CC_MAX_NUM_SPREADS),
_recoveryRate(recoveryRate),
_currency(currency),
_frequency(frequency) {

	// make a local copy of the underlying yieldcurve - its ours now!
	_underlying = new yieldCurve(yc);	
	yieldCurve *spreadCurve = new yieldCurve(flatSpread);
	_combined = combineUnderlyingAndSpreads(*_underlying, *spreadCurve);
	assignFlatSpread(flatSpread);
	
	delete spreadCurve;
}

creditCurve::creditCurve(valarray<yieldPoint> &yp, 
						 valarray<CreditSpreadPoint> &cp, 
						 char *name,
						 Real recoveryRate, 
						 Currency currency, 
						 Frequency frequency) : 
_spreads(CC_MAX_NUM_SPREADS),
_survivalProbability(CC_MAX_NUM_SPREADS),
_defaultProbability(CC_MAX_NUM_SPREADS),
_swapFees(CC_MAX_NUM_SPREADS),
_recoveryRate(recoveryRate), 
_currency(currency), 
_frequency(frequency) {

	_underlying = new yieldCurve(yp, name);
	yieldCurve *spreadCurve = 
		createSpreadCurve(*_underlying, cp);
	_combined = 
		combineUnderlyingAndSpreads(*_underlying, *spreadCurve);
	resampleSpread();

	// cout << _underlying->getName() << ": " << _underlying << endl;
	// cout << spreadCurve->getName() << ": " << spreadCurve << endl;
	// cout << _combined->getName() << ": " << _combined << endl;
	// cout << "end." << endl;

	delete spreadCurve;
}

creditCurve::creditCurve(const creditCurve &rhs) {
	copyObj(rhs);
}

creditCurve &
creditCurve::operator=(const creditCurve &rhs) {
	copyObj(rhs);
	return *this;
}

void
creditCurve::copyObj(const creditCurve &rhs) {
	_underlying = new yieldCurve(*(rhs.getUnderlying()));
	_combined = new yieldCurve(*(rhs.getCombined()));
	_spreads = rhs.getSpreads();
	_survivalProbability = rhs.getSurvivalProbability();
	_defaultProbability = rhs.getDefaultProbability();
	_swapFees = rhs.getSwapFees();
	_recoveryRate = rhs.getRecoveryRate();
	_frequency = rhs.getFrequency();
	_currency = rhs.getCurrency();
}

yieldCurve *
creditCurve::createSpreadCurve(yieldCurve &underlying,
							   valarray<CreditSpreadPoint> &spreads) {
	// convert all credit spreads to relative, and use this to create a
	//  temporary yieldcurve object (so we can use the interpolation
	//  functionality)
	valarray<yieldPoint> _tempspreads(spreads.size());
	for (Natural i = 0; i < spreads.size(); i++) {
		if (spreads[i].getSpreadType() == Absolute) {
			_tempspreads[i].setRate(
				spreads[i].getRate() -
				underlying.spotRate(spreads[i].getMaturity())
				);
		} else {
			_tempspreads[i].setRate(spreads[i].getRate());
		}		
		_tempspreads[i].setMaturity(spreads[i].getMaturity());
		_tempspreads[i].setType(Cash);
	}

	yieldCurve *result = new yieldCurve(_tempspreads, "tempspreads");
	
	// cout << result->getName() << ": " << result << endl;
	// cout << "end." << endl;

	return result;
}

yieldCurve *
creditCurve::combineUnderlyingAndSpreads(yieldCurve &underlying, 
										 yieldCurve &spreadcurve) {
	Natural numTermValues = 0;
	valarray<Real> underlyingMaturities =
		underlying.getMaturitiesInTheZCBCurve();

	valarray<Real> spreadMaturities =
		spreadcurve.getMaturitiesInTheZCBCurve();

	// create a curve combining the underlying points and the credit spreads

	// 1. first get a merged unique list of maturities from the yieldcurve and
	//  the spreads
	valarray<Real> termValues = 
		mergeunique(underlyingMaturities, spreadMaturities);
	numTermValues = termValues.size();

	// 2. then iterate through all known term values adding the spotrate 
	//  of the underlying to the credit spread "spotrate" to create
	//  the combined yieldcurve
	valarray<yieldPoint> combinedyp(numTermValues);	
	for (Natural i = 0; i < numTermValues; i++) {
		combinedyp[i].setRate(
			underlying.spotRate(termValues[i]) +
			spreadcurve.spotRate(termValues[i]));
		combinedyp[i].setMaturity(termValues[i]);
		combinedyp[i].setType(Cash);
	}

	return new yieldCurve(combinedyp, "combined");
}

void 
creditCurve::assignFlatSpread(Real r) {
     Natural size = _spreads.size();
     for(Natural i=0;i<size;i++){
         _spreads[i].setRate(r);       
		 _spreads[i].setMaturity(i);
		 _spreads[i].setType(Relative);

		 _survivalProbability[i].isCached = 
			 _defaultProbability[i].isCached =
			 _swapFees[i].isCached = false;
	 }
}

void
creditCurve::resampleSpread() {
	Natural size = _spreads.size();
	for (Natural i = 0; i < size; i++) {
		_spreads[i].setRate(_combined->spotRate(i) - 
			_underlying->spotRate(i));
		_spreads[i].setMaturity(i);
		_spreads[i].setType(Relative);

		 _survivalProbability[i].isCached = 
			 _defaultProbability[i].isCached =
			 _swapFees[i].isCached = false;
	}
}


creditCurve::~creditCurve(void) { 
	delete _underlying;
	delete _combined;
}

Real 
creditCurve::timeOfCurrentSpread(Real maturity) const {
	return _spreads[indexOfCurrentSpread(maturity)].getMaturity();
}

Natural
creditCurve::indexOfCurrentSpread(Real maturity) const {
	Natural i = 0;
	Natural result = 0;

	// linear search through the array...  performance optimization
	while (i < _spreads.size() &&  _spreads[i].getMaturity() < maturity) {
		i++;
	}

	// if index is past the end of _spreads, take last point, 
	//  otherwise take the point we have (first point equal to or 
	//  after specified maturity)
	result = (i == _spreads.size() ? i - 1 : i);

	return result;
}

Natural
creditCurve::indexOfPreviousSpread(Real maturity) const {
	Natural i = 0;

	while (i < _spreads.size() &&  _spreads[i].getMaturity() < maturity) {
		i++;
	}
	// now go back one index
	i = (i - 1 < 0 ? 0 : i - 1);

	return i;
}

Real 
creditCurve::timeOfPreviousSpread(Real maturity) const {
    return _spreads[indexOfPreviousSpread(maturity)].getMaturity();
}

Real
creditCurve::cumulativeDefaultProbability(Real maturity) const {
	return (1 - survivalProbability(maturity));
}

Real 
creditCurve::survivalProbability(Real maturity) const {
	Real result = 0;
	// cout << "survivalProbability(" << maturity << ")" << endl;
	if (maturity == 0) {
		// no time has passed so survivalProbability is 100%
		result = 1;
	} else {
		Natural i = indexOfCurrentSpread(maturity);
		// if this value is not cached, calculate and cache it
		if (!_survivalProbability[i].isCached) {
			Real T = timeOfCurrentSpread(maturity);
			Real Tminus = timeOfPreviousSpread(maturity);
			Real Sminus = survivalProbability(Tminus);
			Real qminus = defaultProbability(T);

			_survivalProbability[i].value = Sminus * (1 - qminus);
			_survivalProbability[i].isCached = true;
		}
		result = _survivalProbability[i].value;
	}

	return result;
}

Real 
creditCurve::swapFees(Real maturity) const {
	// cout << "swapFees(" << maturity << ")" << endl;
	Real result = 0;

	if (maturity == 0) {
		result = 0;
	} else {
		Natural i = indexOfCurrentSpread(maturity);

		// if this value is not cached, calculate and cache it
		if (!_swapFees[i].isCached) {
			Real T = timeOfCurrentSpread(maturity);
			Real Tminus = timeOfPreviousSpread(maturity);
			Real Fminus = swapFees(Tminus);
			Real Sminus = survivalProbability(Tminus);
			Real qminus = defaultProbability(T);
			Real DF = _underlying->discountFactor(T);
			Real CS = creditSpread(T);
			Real CSminus = creditSpread(Tminus);

			_swapFees[i].isCached = true;
			_swapFees[i].value = 
				Fminus * CS/CSminus + DF * CS * Sminus * (1 - qminus);
		}
		result = _swapFees[i].value;
	}

	return result;
}

/** returns conditional default probability at a given maturity.
	
	@param maturity - time at which to evaluate probability
	@returns conditional default probability - the same probability
		will be returned for all times between two spreads
*/
Real 
creditCurve::defaultProbability(Real maturity) const {
	// cout << "defaultProbability(" << maturity << ")" << endl;
	Real result = 0;

	if (maturity == 0) {
		result = 0;
	} else {
		Natural i = indexOfCurrentSpread(maturity);

		// if this value is not cached, calculate and cache it
		if (!_defaultProbability[i].isCached) {
			Real T = timeOfCurrentSpread(maturity);
			Real Tminus = timeOfPreviousSpread(maturity);
			Real Sminus = survivalProbability(Tminus);
			Real Fminus = swapFees(Tminus);
			Real DF = _underlying->discountFactor(T);

			Real Tdefault = T - (T - Tminus)/2;
			Real DFd = _underlying->discountFactor(Tdefault);
			Real CS = creditSpread(T);
			Real CSminus = creditSpread(Tminus);

			// cout << "DFd: " << DFd << endl;
			Real num = Fminus * (CS/CSminus - 1) + DF * CS * Sminus;
			Real denom = Sminus * (DFd * (1 - _recoveryRate) + DF * CS);

			_defaultProbability[i].isCached = true;
			_defaultProbability[i].value = num/denom;
		}

		result = _defaultProbability[i].value;
	}

	return result;
}

Real
creditCurve::riskyDiscountFactor(Real maturity, 
								 interestComposition composition) {
	/** risky discount = risk free discount * survival probability 
		
		In the class notes we have RF = DF * (1 - Q(T))
		Q(T) is cumulative default probability so it is the complement
		of S(T), the cumulative survival probability.
		
		In discrete time we have the identity: (S(n) - S(n+1)) / S(n) = q(n) 
		where q(n) is the default probability for period n conditional
		on no earlier default.  q(n) is a discrete time version of 
		hazard rate.  In the limit (as dt -> 0) this leads to the expression
		S(t) = exp(-(integral from 0 to t)*h(t)*dt)

		The risky discount factor is a "discounted" discount factor - 
		the discounting applied is the survival probability.  
		In continuous time we can use the expression above but since
		we have calculated everything to this point in discrete time
		and we have an explicit expression for the survival probability
		we use this as the discount factor rather than the continuous
		time expression above.
		*/
	Real DF = _underlying->discountFactor(maturity, composition);
	Real RDF = DF * survivalProbability(maturity);

	return RDF;
}

ostream& 
operator << (ostream &os, const creditCurve &c) {
	os << c._combined;
	return os;
}