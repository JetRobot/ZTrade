#include "..\PartJ\convertiblebond.h"
#include "..\common\utils.h"
using namespace std;

bool mainconvertiblebond(void) {
	bool result = true;
	Date today;
	Date todayPlusTwentyYears;

	// using atmel price from 
	// http://www.math.nyu.edu/fellows_fin_math/laud/fall2005/project/CVT_DESC.gif
	// volatility as of 20-dec-2005 - see
	// http://www.ivolatility.com/options.j?ticker=atml
	today.setDateToToday();
	todayPlusTwentyYears = today.plus(20 * 365, Days);
	Real atmelPrice = 2.23;
	Real atmelVol = 0.44;
	Real atmelFace = 1000;
	Real riskfreeRate = 0.05;
	Real atmelSpread = 0.02;
	Real atmelConversion = 55.932;
	Real atmelCallPrice = 50.0;
	Real atmelPutPrice = 20.0;
	Natural nSteps = 10;

	asset *Atmel = new asset(atmelPrice, atmelVol);
	riskybond *AtmelBond = new riskybond(today, 
		todayPlusTwentyYears, 
		atmelFace, 
		ACT_365, 
		yieldCurve(riskfreeRate),
		creditCurve(riskfreeRate, atmelSpread));

	// test: ATMELNOCALLPUT
	//  check values in this test and next test verified using
	//  CBcalculator.xls
	convertiblebond *AtmelConvertible1 = 
		new convertiblebond(*Atmel, *AtmelBond, atmelConversion, nSteps);
	cout << "atmel convertible - no call put: " << AtmelConvertible1 << endl;
	Real p = AtmelConvertible1->fairvalue();
	Real pExp = 42.7113;
	if (!realsEqual(p, pExp, 0.0001)) {
		cout << "convertible, non-callable, non-puttable price: " << p << endl;
		cout << "expecting: " << pExp << endl;
		return false;
	}

	// test: ATMELCALLONLY
	convertiblebond *AtmelConvertible2 =
		new convertiblebond(*Atmel, *AtmelBond, atmelConversion, 
		nSteps, atmelCallPrice);
	cout << "atmel convertible - call only: " << AtmelConvertible2 << endl;
	p = AtmelConvertible2->fairvalue();
	pExp = 25.7722;
	if (!realsEqual(p, pExp, 0.0001)) {
		cout << "convertible, callable price: " << p << endl;
		cout << "expecting: " << pExp << endl;
		return false;
	}

	cout << "callable @ 50" << endl
		<< "So,convertibleprice,paritydelta,irdelta,paritygamma" << endl;
	for (Natural i = 0; i <= 100; i++) {
		Real p = 1 + i*.1;
		Atmel->setPrice(p);
		convertiblebond *cb =
			new convertiblebond(*Atmel, *AtmelBond, atmelConversion, 100, atmelCallPrice);
		cout << p << ","
			<< cb->fairvalue() << ","
			<< cb->parityDelta() << ","
			<< cb->rho() << ","
			<< cb->parityGamma() << endl;
		delete cb;
	}

	cout << "not callable" << endl
		<< "So,convertibleprice,paritydelta,irdelta,paritygamma" << endl;
	for (Natural i = 0; i <= 100; i++) {
		Real p = 1 + i*.1;
		Atmel->setPrice(p);
		convertiblebond *cb =
			new convertiblebond(*Atmel, *AtmelBond, atmelConversion, 100);
		cout << p << ","
			<< cb->fairvalue() << ","
			<< cb->parityDelta() << ","
			<< cb->rho() << ","
			<< cb->parityGamma() << endl;
		delete cb;
	}


	// test: HULLEX21.1
	Date todayPlusNineMonths = today.plus(9, Months);
	Real XYZFace = 100;
	Real XYZPrice = 50;
	Real XYZVol = 0.3;
	Real XYZConversion = 2;
	Real XYZCallPrice = 113;
	Real XYZSpread = 0;
	nSteps = 3;
	asset *XYZ = new asset(XYZPrice, XYZVol);
	riskybond *XYZBond = new riskybond(today, 
		todayPlusNineMonths, 
		XYZFace, 
		ACT_365, 
		yieldCurve(riskfreeRate), 
		creditCurve(riskfreeRate, XYZSpread));
	convertiblebond *XYZConvertible = 
		new convertiblebond(*XYZ, *XYZBond, XYZConversion, nSteps, XYZCallPrice);
	cout << "XYZ convertible - call only: " << XYZConvertible << endl;
	
	p = XYZConvertible->fairvalue();
	// value in Hull is 106.93 but we are not modelling the possibility of
	//  default so the price is slightly higher
	pExp = 107.578; 
	if (!realsEqual(p, pExp, 0.001)) {
		cout << "XYZ convertible, callable price: " << p << endl;
		cout << "expecting: " << pExp << endl;
		return false;
	}

	delete Atmel;
	delete AtmelBond;
	delete XYZ;
	delete XYZBond;
	delete AtmelConvertible1;
	delete AtmelConvertible2;
	delete XYZConvertible;
	return result;
}