#include ".\binomialTree.h"

binomialTree::binomialTree(void) :
 _So(BT_DEFAULT_SO),
 _sigma(BT_DEFAULT_SIGMA),
 _maturity(BT_DEFAULT_MATURITY),
 _n(BT_DEFAULT_STEPS),
 _stockProcess(BT_DEFAULT_STEPS + 1),
 _claimProcess(BT_DEFAULT_STEPS + 1),
 _discountFactor(BT_DEFAULT_STEPS),
 _q(BT_DEFAULT_STEPS) {
	_dt = _maturity/_n;
	_u = exp(_sigma * sqrt(_dt));
	_d = 1/_u;

	setClaimVariables(BT_DEFAULT_RATE);
	constructStockProcess();
}

binomialTree::binomialTree(Real So, 
						   Real r, 
						   Real sigma, 
						   Real T, 
						   Natural n) :
_So(So), _sigma(sigma), _maturity(T), _n(n), 
_stockProcess(n + 1),
_claimProcess(n + 1), 
_discountFactor(n), 
_q(n) {
	_dt = T/n;
	_u = exp(sigma * sqrt(_dt));
	_d = 1/_u;

	setClaimVariables(r);
	constructStockProcess();
}

binomialTree::binomialTree(const asset& theStock, 
						   yieldCurve& yc, 
						   Real T, 
						   Natural n) :
_So(theStock.getPrice()), _sigma(theStock.GetVolatility()), 
_maturity(T), _n(n), 
_stockProcess(n + 1),
_claimProcess(n + 1),
_discountFactor(n),
_q(n) {
	_dt = T/n;
	_u = exp(_sigma * sqrt(_dt));
	_d = 1/_u;

	setClaimVariables(yc);
	constructStockProcess();
}

binomialTree::binomialTree(Real So, 
						   Real r, 
						   Real sigma, 
						   Real T, 
						   Natural n, 
						   Real u, 
						   Real d) :
_So(So), _sigma(sigma), _maturity(T), _n(n), _u(u), _d(d), 
_stockProcess(n + 1), 
_claimProcess(n + 1),
_discountFactor(n), 
_q(n) {
	_dt = T/n;
	setClaimVariables(r);
	constructStockProcess();
}

void 
binomialTree::setClaimVariables(Real constantRate) {
	Real df = exp(-1 * constantRate * _dt);
	Real q = (exp(constantRate * _dt) - _d)/(_u - _d);
	for (Natural timestep = 0; timestep < _n; timestep++) {
		_discountFactor[timestep] = df;
		_q[timestep] = q;
	}
}

void
binomialTree::setClaimVariables(yieldCurve& yc) {
	for (Natural timestep = 0; timestep < _n; timestep++) {
		_discountFactor[timestep] = yc.forwardDiscountFactor(timestep * _dt, _dt);
		_q[timestep] = ((1/_discountFactor[timestep]) - _d)/(_u - _d);
	}
}

binomialTree::binomialTree(const binomialTree &rhs) {
	copyObj(rhs);
}

binomialTree &
binomialTree::operator=(const binomialTree &rhs) {
	copyObj(rhs);
	return *this;
}

void
binomialTree::copyObj(const binomialTree &rhs) {
	_So = rhs._So;
	_sigma = rhs._sigma;
	_maturity = rhs._maturity;
	_n = rhs._n;
	_dt = rhs._dt;
	_u = rhs._u;
	_d = rhs._d;
	_discountFactor = rhs._discountFactor;
	_stockProcess = rhs._stockProcess;
	_claimProcess = rhs._claimProcess;
	_discountFactor = rhs._discountFactor;
	_q = rhs._q;
}

binomialTree::~binomialTree(void) { 

}

void
binomialTree::constructStockProcess(void) {
	// set root node to initial stock price, dimension claim process but set
	// all values to zero
	_stockProcess[0].resize(1, _So);
	_claimProcess[0].resize(1, 0);

	// iterate forward in time, applying the appropriate multipliers
	for (Natural i = 1; i <= _n; i++) {
		_stockProcess[i].resize(i + 1);
		_claimProcess[i].resize(i + 1, 0);

		_stockProcess[i][0] = _stockProcess[i - 1][0] * _d;
		for (Natural j = 1; j <= i; j++) {
			_stockProcess[i][j] = _stockProcess[i - 1][j - 1] * _u;
		}
	}
}

const valarray<Real>* 
binomialTree::getStockProcess(Natural step) {
	valarray<Real> *result = 
		(step <= _n ? &_stockProcess[step] : NULL);

	return result;
}

const valarray<Real>* 
binomialTree::getClaimProcess(Natural step) {
	valarray<Real> *result = 
		(step <= _n ? &_claimProcess[step] : NULL);

	return result;
}

Real
binomialTree::getPrice() {
	return (_n > 0 ? _claimProcess[0][0] : 0);
}

ostream& 
operator << (ostream &os, const binomialTree &bt) {
	os << "binomialtree parameters - " << endl 
		<< " So = " << bt.getSo()
		<< ", sigma = " << bt.getSigma()
		<< ", maturity = " << bt.getMaturity() << endl
		<< " steps = " << bt.getSteps() 
		<< ", u = " << bt._u
		<< ", d = " << bt._d << endl
		<< " discountFactors: " << valarrayRealToString(bt._discountFactor) << endl
		<< " q (up move prob): " << valarrayRealToString(bt._q) << endl << endl;

	os << "stock process" << endl;
	for (Natural timestep = 0; timestep <= bt._n; timestep++) {
		os << bt._stockProcess[timestep][0];
		for (Natural branch = 1; branch <= timestep; branch++) {
			os << "," << bt._stockProcess[timestep][branch];
		}
		os << endl;
	}

	os << "claim process (price at the top)" << endl;
	for (Natural timestep = 0; timestep <= bt._n; timestep++) {
		os << bt._claimProcess[timestep][0];
		for (Natural branch = 1; branch <= timestep; branch++) {
			os << "," << bt._claimProcess[timestep][branch];
		}
		os << endl;
	}

	return os;
}

void
binomialTree::runEngineCall(PayOff thePayoff) {
	// check payoff at leaf nodes
	for (Natural branch = 0; branch <= _n; branch++) {
		_claimProcess[_n][branch] =
			thePayoff.Call(_stockProcess[_n][branch]);
	}

	// now work backwards discounting expected values
	for (Integer timestep = _n - 1; timestep >= 0; timestep--) {
		for (Integer branch = 0; branch <= timestep; branch++) {
			_claimProcess[timestep][branch] = 
				_discountFactor[timestep] * 
				((_q[timestep] * _claimProcess[timestep + 1][branch + 1]) +
				((1 - _q[timestep]) * _claimProcess[timestep + 1][branch]));
		}
	}
}

void
binomialTree::runEngineConvertibleBond(PayOff thePayoff, 
									   Real ConversionRatio,
									   Real CallPrice,
									   Real PutPrice) {
    // check payoff at leaf nodes - don't apply call/putprice
	for (Natural branch = 0; branch <= _n; branch++) {
		_claimProcess[_n][branch] = 
			thePayoff.Convertible(
				_stockProcess[_n][branch], ConversionRatio, 100);
	}

	// now work back through the tree
	for (Integer timestep = _n - 1; timestep >= 0; timestep--) {
		for (Integer branch = 0; branch <= timestep; branch++) {

			// first calculate bond price as discounted expected value
			_claimProcess[timestep][branch] = 
				_discountFactor[timestep] * 
				((_q[timestep] * _claimProcess[timestep + 1][branch + 1]) +
				((1 - _q[timestep]) * _claimProcess[timestep + 1][branch]));

			// now check other possibilities unique to convertibles:
			//  conversion to stock, call, put
			_claimProcess[timestep][branch] = 
				thePayoff.Convertible(_stockProcess[timestep][branch], 
					ConversionRatio, 
					_claimProcess[timestep][branch], 
					CallPrice, 
					PutPrice);
		} // for each branch
	} // for times from n -1 to 0
}