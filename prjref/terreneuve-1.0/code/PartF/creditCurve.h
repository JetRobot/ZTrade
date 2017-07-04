#pragma once
#ifndef creditCurve_h
#define creditCurve_h

#include "../common/types.h"
#include "../common/date.h"
#include "../PartB/yieldCurve.h"
#include <string.h>
#include <math.h>
#include <valarray>

using namespace std;

#define CC_MAX_NUM_SPREADS 30

// default arguments to use when constructing creditcurves
#define CC_DEFAULT_RECOVERY_RATE	0.40
#define CC_DEFAULT_FREQUENCY		Annual
#define CC_DEFAULT_CURRENCY			USD
#define CC_DEFAULT_NAME				"creditCurve"

enum CreditSpreadType {
	// this is an absolute rate, spread must be calculated
	Absolute,
	// this is the additional rate above the base rate
	Relative
};

/** used to encapsulate a spread at a given maturity */
class CreditSpreadPoint { 
  public:
    /** Default Constructor */
    CreditSpreadPoint(void);

    /** Constructor
      @param s: Real spread between risky asset and risk free rate
      @param T: Real maturity of the spread
	  @param t: Absolute or relative spread
      */
    CreditSpreadPoint(Real r, Real T, CreditSpreadType t);
  
    /** Destructor */
    ~CreditSpreadPoint(void);

	static char *TypeAsString(CreditSpreadType t);

    /** Associated rate */
    Real getRate() { return _rate; }

	Real getMaturity() { return _maturity; }

	CreditSpreadType getSpreadType() { return _spreadtype; }

    void setRate(Real r) { _rate = r; };

    void setMaturity(Real m) { _maturity = m; };

	void setType(CreditSpreadType t) { _spreadtype = t; };
private:
    Real  _rate;
    Real  _maturity;
	CreditSpreadType _spreadtype;

};

class creditCurve : public yieldCurve {
public:
	friend ostream& operator << (ostream &os, const creditCurve& c);
	friend ostream& operator << (ostream &os, const creditCurve* c) {
		return os << *c;
	}

	/** default constructor */
	creditCurve(void);

	/** Constructor
	@param yp - as with yield curve, array of yield points
	@param cp - array of credit spreads for different maturities
	@param recoveryRate - amount of debt collected in case of default
	@param currency - that the debt is denominated in
	@param frequency - annual, semi-annual, etc.
	@param name - a string identifying the curve
	*/
	creditCurve(
		valarray<yieldPoint> &yp, 
		valarray<CreditSpreadPoint> &cp,
		char *name = CC_DEFAULT_NAME,
		Real recoveryRate = CC_DEFAULT_RECOVERY_RATE,
		Currency currency = CC_DEFAULT_CURRENCY,
		Frequency frequency = CC_DEFAULT_FREQUENCY);

	creditCurve(
		yieldCurve &yc, 
		valarray<CreditSpreadPoint> &cp, 
		char *name = CC_DEFAULT_NAME,
		Real recoveryRate = CC_DEFAULT_RECOVERY_RATE,
		Currency currency = CC_DEFAULT_CURRENCY,
		Frequency frequency = CC_DEFAULT_FREQUENCY);

	creditCurve(
		Real flatRate,
		Real flatSpread,
		char *name = CC_DEFAULT_NAME,
		Real recoveryRate = CC_DEFAULT_RECOVERY_RATE,
		Currency currency = CC_DEFAULT_CURRENCY,
		Frequency frequency = CC_DEFAULT_FREQUENCY);

	creditCurve(
		yieldCurve &yc,
		Real flatSpread,
		char *name = CC_DEFAULT_NAME,
		Real recoveryRate = CC_DEFAULT_RECOVERY_RATE,
		Currency currency = CC_DEFAULT_CURRENCY,
		Frequency frequency = CC_DEFAULT_FREQUENCY);

	creditCurve(const creditCurve &rhs);

	creditCurve &
	operator=(const creditCurve &rhs);

	virtual ~creditCurve(void);

	yieldCurve *
	createSpreadCurve(
		yieldCurve &underlying,
		valarray<CreditSpreadPoint> &spreads);

	yieldCurve *
	combineUnderlyingAndSpreads(
		yieldCurve &underlying, 
		yieldCurve &spreadcurve);

	void 
	assignFlatSpread(Real r);

	void
	resampleSpread();

	virtual Real creditSpread(Real maturity) const {
		return (_combined->spotRate(maturity) - 
			    _underlying->spotRate(maturity));
	}

	virtual Real creditSpread(Date maturityDate) const {
		return (_combined->spotRate(maturityDate) - 
			    _underlying->spotRate(maturityDate));
	}
	
	virtual Real timeOfCurrentSpread(Real maturity) const;
	virtual Natural indexOfCurrentSpread(Real maturity) const;
	virtual Real timeOfPreviousSpread(Real maturity) const;
	virtual Natural indexOfPreviousSpread(Real maturity) const;
	virtual Real survivalProbability(Real maturity) const;
	virtual Real cumulativeDefaultProbability(Real maturity) const;
	virtual Real swapFees(Real maturity) const;

	/** returns conditional default probability at a given maturity.
		
		@param maturity - time at which to evaluate probability
		@returns conditional default probability - the same probability
		 will be returned for all times between two spreads
	*/
	virtual Real defaultProbability(Real maturity) const;

	/** returns hazard rate at a given maturity - this is an alias for
	    defaultProbability i.e. probability of default at time t conditional 
		on no earlier default.

		@param maturity - time at which to evaluate probability
		@returns conditional default probability
	*/
	virtual Real hazardRate(Real maturity) const {
		return defaultProbability(maturity);
	}
	
	virtual Real spotRate(Real maturity) const {
		return _combined->spotRate(maturity);
	}

	virtual Real spotRate(Date maturityDate) const {
		return _combined->spotRate(maturityDate);
	}

	/**
	Calculates the discountFactor
	@param maturity : just after ZCBrates are computed, it it very easy [done at trhe constructor level]
	*/     
	virtual Real 
	discountFactor(Real maturity, interestComposition composition=Continuous) {
		return _combined->discountFactor(maturity, composition);
	}

	/** Calculates the risky discount factor incorporating the hazard
	    rate.

		@param maturity - maturity to calculate risky discount factor for
	*/
	virtual Real
	riskyDiscountFactor(Real maturity, interestComposition composition=Continuous);
	
	/**
	Calculates the discountFactor
	@param maturity : just after ZCBrates are computed, it it very easy [done at trhe constructor level]
	*/     
	virtual Real 
	discountFactor(Date maturityDate, interestComposition composition=Continuous) {
		return _combined->discountFactor(maturityDate, composition);
	}

	/**
	Calculates the fwd rate
	@param forwardStart start of the rate
	@param maturityAfterForward maturity after the start
	*/          
	virtual Real 
	forwardRate(Real forwardStart, Real effectiveLengthOfTheContractAfterStart, 
	interestComposition composition=Continuous) {
		return _combined->forwardRate(forwardStart, 
			effectiveLengthOfTheContractAfterStart,
			composition);
	}

	/**
	Calculates the fwd rate
	@param forwardStart start of the rate
	@param maturityAfterForward maturity after the start
	*/          
	virtual Real 
	forwardRate(Date forwardStart, Date forwardEnd, 
	interestComposition composition=Continuous) {
		return _combined->forwardRate(forwardStart, forwardEnd, composition);
	}

	virtual valarray<Real> getMaturitiesInTheZCBCurve() const
		{ return _combined->getMaturitiesInTheZCBCurve(); }

	virtual char *getName() { return _combined->getName(); }

	Real getRecoveryRate(void) const {return _recoveryRate;}
	Currency getCurrency(void) const {return _currency;}
	Frequency getFrequency(void) const {return _frequency;}

protected:
	yieldCurve *_underlying;
	yieldCurve *_combined;

	yieldCurve *getUnderlying(void) const {return _underlying;}
	yieldCurve *getCombined(void) const	{return _combined;	}
	valarray<CreditSpreadPoint>	getSpreads(void) const {return _spreads;}
	valarray<cachedval> getSurvivalProbability(void) const {return _survivalProbability;}
	valarray<cachedval> getDefaultProbability(void) const {return _defaultProbability;}
	valarray<cachedval> getSwapFees(void) const {return _swapFees;}


private:
	valarray<CreditSpreadPoint> _spreads;

	// allow const functions to update the cache status
	mutable valarray<cachedval> _survivalProbability;
	mutable valarray<cachedval> _defaultProbability;
	mutable valarray<cachedval> _swapFees;
	
	Real _recoveryRate;
	Currency _currency;
	Frequency _frequency;

	void
	copyObj(const creditCurve &rhs);
};

#endif