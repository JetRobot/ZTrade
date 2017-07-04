#pragma once
#ifndef convertiblebond_h
#define convertiblebond_h

#include "../common/types.h"
#include "../PartH/bond.h"
#include "../PartC/asset.h"
#include "./binomialTree.h"
#include <iostream>
#include <valarray>
using namespace std;

// bond component defaults
#define CB_DEFAULT_FACEAMOUNT	100
#define CB_DEFAULT_MATURITY		1
#define CB_DEFAULT_DAYCOUNT		ACT_360
#define CB_DEFAULT_RATE			0.05
#define CB_DEFAULT_SPREAD		0.02

// stock component defaults
#define CB_DEFAULT_SO			100
#define CB_DEFAULT_SIGMA		0.30

// binomial tree defaults
#define CB_DEFAULT_STEPS		4

// convertible component defaults
#define CB_DEFAULT_RATIO		10
#define CB_DEFAULT_CALLPRICE	TN_INFINITY
#define CB_DEFAULT_PUTPRICE		0

class convertiblebond : public riskybond {
public:
	friend ostream& operator << (ostream &os, convertiblebond& cb);
	friend ostream& operator << (ostream &os, convertiblebond* cb) {
		return os << *cb;
	};

	/** Constructor
	*/
	convertiblebond(
		asset Stock, 
		riskybond Bond, 
		Real conversionRatio = CB_DEFAULT_RATIO, 
		Natural nSteps = CB_DEFAULT_STEPS,
		Real callPrice = CB_DEFAULT_CALLPRICE, 
		Real putPrice = CB_DEFAULT_PUTPRICE);
	virtual ~convertiblebond(void);
	virtual Real fairvalue(Date today);
	virtual Real fairvalue() {return fairvalue(_issue);};

	inline Real adjustedConversionRatio() const {
		return _conversionRatio / getFaceAmount() * 100;
	};
	Real parity(void) const {return _conversionRatio * _stock.getPrice();};
	Real delta(void) const {return delta(_issue);};
	Real delta(Date today) const;
	Real parityDelta(void) const {return parityDelta(_issue);};
	Real parityDelta(Date today) const {return delta(today)/adjustedConversionRatio();};
	convertiblebond shiftedcbond(Real shift);
	Real rho(Date today) {return interestRateDelta(today);};
	Real rho() {return rho(_issue);};
	Real interestRateDelta(void) const {return interestRateDelta(_issue);};
	Real interestRateDelta(Date today) const;
	Real gamma(void) const {return gamma(_issue);};
	Real gamma(Date today) const;
	Real parityGamma(void) const {return parityGamma(_issue);};
	Real parityGamma(Date today) const {
		return gamma(today)/pow(adjustedConversionRatio(), 2);
	};

	Natural getSteps(void) const {return _n;};
	
protected:
	mutable binomialTree *_bt;
	mutable bool _btCached;
	
private:
	asset _stock;
	riskybond _bond;

	Natural _n;
	Real _callPrice;
	Real _putPrice;
	Real _conversionRatio;


	mutable Real _price;
	mutable bool _priceCached;
	mutable Date _datepriceCached;

	mutable Real _delta;
	mutable bool _deltaCached;
	mutable Date _datedeltaCached;

	mutable Real _interestRateDelta;
	mutable bool _interestRateDeltaCached;
	mutable Date _dateinterestRateDeltaCached;

	mutable Real _gamma;
	mutable bool _gammaCached;
	mutable Date _dategammaCached;

	void
	copyObj(const convertiblebond &rhs);
};

#endif