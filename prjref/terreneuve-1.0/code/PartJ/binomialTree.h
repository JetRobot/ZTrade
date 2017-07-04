#pragma once
#ifndef binomialtree_h
#define binomialtree_h

#include "../common/types.h"
#include "../PartA/MonteCarlo1/PayOff.h"
#include "../PartB/yieldCurve.h"
#include "../PartC/asset.h"
#include "../common/utils.h"
#include <iostream>
#include <valarray>
using namespace std;

#define BT_DEFAULT_SO 100
#define BT_DEFAULT_RATE 0.05
#define BT_DEFAULT_SIGMA 0.30
#define BT_DEFAULT_MATURITY 1
#define BT_DEFAULT_STEPS 10

class binomialTree {
public:
	friend ostream& operator << (ostream &os, const binomialTree& bt);
	friend ostream& operator << (ostream &os, const binomialTree* bt) {
		return os << *bt;
	};

	/** default constructor */
	binomialTree(void);

	/** Constructor
	*/
	binomialTree(Real So, Real r, Real sigma, Real T, Natural n);
	binomialTree(Real So, Real r, Real sigma, Real T, Natural n, Real u, Real d);
	binomialTree(const asset& theAsset, yieldCurve& yc, Real T, Natural n);
	binomialTree(const binomialTree &rhs);
	binomialTree &operator=(const binomialTree &rhs);
	virtual ~binomialTree(void);

	const valarray<Real>* getStockProcess(Natural step);

	const valarray<Real>* getClaimProcess(Natural step);

	Real getPrice();

	void runEngineConvertibleBond(PayOff thePayoff, 
		Real ConversionRatio,
		Real CallPrice,
		Real PutPrice);

	void runEngineCall(PayOff thePayoff);
	// void runEnginePut(PayOff thePayoff, Real Strike);
	// void runEngineAmericanCall(PayOff thePayoff, Real Strike);
	// void runEngineAmericanPut(PayOff thePayoff, Real Strike);

protected:
	Real getSo() const {return _So;};
	Real getRate(Natural timestep) const {
		return (timestep < _n ? _discountFactor[timestep] : 0);
	};
	Real getSigma() const {return _sigma;};
	Real getMaturity() const {return _maturity;};
	Natural getSteps() const {return _n;};
	void constructStockProcess();
	void setClaimVariables(Real constantRate);
	void setClaimVariables(yieldCurve& yc);
private:
	Real _So;
	Real _sigma;
	Real _maturity;
	Natural _n;
	Real _dt;
	Real _u;
	Real _d;

	valarray<valarray <Real> > _stockProcess;

	valarray<valarray <Real> > _claimProcess;

	valarray<Real> _discountFactor;
	
	/** risk neutral probability of an up move */
	valarray<Real> _q;

	void
	copyObj(const binomialTree &rhs);
};

#endif