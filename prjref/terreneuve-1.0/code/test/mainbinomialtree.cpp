#include "..\PartA\BlackScholes\BlackScholes.h"
#include "../Interface/main.h"
#include "..\PartJ\binomialTree.h"
#include "..\common\utils.h"
using namespace std;

bool mainbinomialtree(void) {
	bool result = true;
	const valarray<Real> *stockValues;
	
	Real So = 50, K = 50, r = 0.05, sigma = .30, T = 1;
	Natural n = 10;
	binomialTree *bt = new binomialTree(So, r, sigma, T, n);
	binomialTree *bt2 = new binomialTree(So, r, sigma, T, n + 1);
	bt->runEngineCall(PayOff(K));
	bt2->runEngineCall(PayOff(K));

	cout << "bintree: " << bt << endl;
	
	stockValues = bt->getStockProcess(n);
	Real highest = So * exp(n * sigma * sqrt(T/n));
	Real lowest = So * exp(-1 * (Real)n * sigma * sqrt(T/n));
	if ((stockValues == NULL) || 
		!realsEqual((*stockValues)[0], lowest) || 
		!realsEqual((*stockValues)[n], highest)) {
		cout << "low value: " << (*stockValues)[0]
			<< " expecting: " << lowest
			<< " high value: " << (*stockValues)[n] 
			<< " expecting: " << highest << endl;
		result = false;
	}

	BlackScholes *BS = new BlackScholes(So, sigma, true, r, K, T, Call);

	LongNatural nPaths = 100000, nDates = 1;
	volsurface* myvol = new volsurface(sigma);
	yieldCurve* mycurve = new yieldCurve(r);
	Real mcPrice = mainmc(T, K, So, myvol, mycurve, nPaths, nDates, 1);

	cout << "binomial tree price: " << bt->getPrice() << endl;
	cout << "avg of " << n << " and " << (n + 1) << " tree prices: " <<
		(.5 * (bt->getPrice() + bt2->getPrice())) << endl;
	cout << "black-scholes price: " << BS->getPrice() << endl;
	cout << "monte carlo price: " << mcPrice << endl;

	delete BS;
	// delete myvol;
	// delete mycurve;
	delete bt;
	return result;
}