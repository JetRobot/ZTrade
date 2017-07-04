#include "..\PartF\creditCurve.h"
#include "..\common\filereader.h"

#include<iostream>
#include<valarray>
#include<time.h>
using namespace std;

bool maincreditcurve(void) {
	string datadir = FileReader::getdatadirasstring();
	string yieldCurveData = datadir + "yctest.csv";
	string creditSpreadData = datadir + "ccspread.csv";
	string creditSpreadMixedData = datadir + "ccspreadmix.csv";
	string creditSpreadZeroData = datadir + "cc0spread.csv";
	string aigSpreadData = datadir + "cdsw_aig.csv";

	bool result = true;

	// the underlying curve
	valarray<yieldPoint> *yieldCurveArray = 
		FileReader::buildYieldPointArray(yieldCurveData);

	// zero (relative) spreads
	valarray<CreditSpreadPoint> *creditSpreadZeroArray =
		FileReader::buildCreditSpreadPointArray(creditSpreadZeroData);

	// absolute "spreads"	
	valarray<CreditSpreadPoint> *creditSpreadArray =
		FileReader::buildCreditSpreadPointArray(creditSpreadData);

	// mixed absolute and relative spreads (equivalent to absolute
	//  when combined with the underlying yield curve)
	valarray<CreditSpreadPoint> *creditSpreadMixedArray =
		FileReader::buildCreditSpreadPointArray(creditSpreadMixedData);

	// AIG credit swap spreads from bloomberg
	valarray<CreditSpreadPoint> *aigSpreadArray =
		FileReader::buildCreditSpreadPointArray(aigSpreadData);

	if ((yieldCurveArray == NULL) || 
		(creditSpreadArray == NULL) ||
		(creditSpreadZeroArray == NULL) ||
		(creditSpreadMixedArray == NULL)) {
		cout << "error reading in credit curve test files" << endl;
		return false;
	}

	// test: NOSPREAD
	// construct a credit curve with no spread, verify that
	//  it returns the same values as the underlying yieldcurve
	creditCurve zeroSpreadCurve(*yieldCurveArray, 
		*creditSpreadZeroArray, 
		"zero-spread");

	// underlying yield curve which we can use for comparisons
	yieldCurve underlyingCurve(*yieldCurveArray, "test");

	if (zeroSpreadCurve != underlyingCurve) {
		cout << "error: zero spread spot rates don't match the underlying" << endl;
		return false;
	}

	// test: MIXEDSPREAD
	// now construct a curve with spreads, one with absolute and
	//  one with a mix of absolute and relative, but both are
	//  equivalent (given the same underlying yieldCurve)
	creditCurve absCurve(*yieldCurveArray, 
		*creditSpreadArray, 
		"abs-spreads");

	creditCurve mixedCurve(*yieldCurveArray, 
		*creditSpreadMixedArray, 
		"mixed-spreads");
	
	cout << "zeroSpreadCurve:\n" << zeroSpreadCurve << endl;
	cout << "absCurve:\n" << absCurve << endl;
	cout << "mixedCurve:\n" << mixedCurve << endl;
	if (absCurve != mixedCurve) {
		cout << "error: absolute spread spot rates don't match mixed curve" << endl;
		return false;
	}

	// test: CTORTEST
	creditCurve mixedCurve2(underlyingCurve, 
		*creditSpreadMixedArray,
		"mixed-spreads2");
	cout << "mixedCurve2:\n" << mixedCurve2 << endl;
	if (mixedCurve != mixedCurve2) {
		cout << "error: constructing with yieldpoints gives different result"
			<< " than constructing with yieldcurve" << endl;
		return false;
	}

	Real flatRate = 0.04;  Real flatSpread = 0.01;
	creditCurve flatCreditCurve(flatRate, 
		flatSpread);
	yieldCurve flatYieldCurve(flatRate + flatSpread);
	
	cout << "flatCreditCurve:\n" << flatCreditCurve << endl;
	cout << "flatYieldCurve:\n" << flatYieldCurve << endl;
	if (flatYieldCurve != flatCreditCurve) {
		cout << "error: flat 0.04 + 0.01 creditcurve gives different result"
			<< " than flat 0.05 yieldcurve" << endl;
		return false;
	}

	// test: COPYCTORTEST
	creditCurve *pcreditCurve = new creditCurve(*yieldCurveArray, 
		*creditSpreadArray, 
		"abs-spreads");
	creditCurve copyCurve(*pcreditCurve);
	creditCurve equalCurve = copyCurve;
	delete pcreditCurve;
	cout << "copycurve: " << copyCurve << endl;
	cout << "equalcurve: " << equalCurve << endl;

	// test: HULLS21.3TEST
	flatRate = 0.05;  flatSpread = 0.0126;
	creditCurve flatCreditCurve2(flatRate, flatSpread);
	for (Natural i = 0; i <= 5; i++) {
		cout << "prob of default at time " << i << "," 
			<< flatCreditCurve2.defaultProbability(i) << ","
			<< "survival prob," 
			<< flatCreditCurve2.survivalProbability(i) << endl;
	}

	// test: AIGTEST
	creditCurve aigCurve(flatYieldCurve, *aigSpreadArray, "aigcurve");
	cout << "aigCurve: " << aigCurve << endl;
	cout << "\n\n*** table of various values for aig curve" << endl;
	cout << "time,creditspread,hazardrate,survivalprob,cumdefaultprob,riskydiscount" << endl;
	for (Natural i = 0; i <= 10; i++) {
		cout << i << ","
			<< aigCurve.creditSpread(i) << ","
			<< aigCurve.hazardRate(i) << ","
			<< aigCurve.survivalProbability(i) << ","
			<< aigCurve.cumulativeDefaultProbability(i) << ","
			<< aigCurve.riskyDiscountFactor(i) << endl;
	}

	return result;
}