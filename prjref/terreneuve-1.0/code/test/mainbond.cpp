#include"..\PartB\yieldCurve.h"
#include"..\PartF\creditCurve.h"
#include"..\PartH\bond.h"

#include "..\common\filereader.h"

#include<iostream>
#include<valarray>

using namespace std;

bool mainbond(void) {
	yieldCurve testcurve=yieldCurve(0.05);
	creditCurve testcreditcurve = creditCurve(0.05, 0.02);

	Date issue(15,November,2005);
	Date maturity(15,November,2015);
	Date firstcoupondate(15,May,2006);
	Real coupon = 0.045;
	Frequency freq2 = Semiannual;
	Real faceamount = 100.0;
	DayCountConvention daycount = ACT_365;

	treasurybond testTbond(issue, maturity, firstcoupondate, coupon, freq2, faceamount, daycount, testcurve);
	riskybond testRbond(issue, maturity, firstcoupondate, coupon, freq2, faceamount, daycount, testcurve, testcreditcurve);
	
	cout << "characteristics of our Treasury and risky bonds"<<endl;
	cout << "date of issue 15 November 2005, firstcoupondate 15 May 2006, maturity 15 November 2015"<<endl;
	cout <<	"coupon = 4.5%, faceamount 100"<<endl;
	cout << "frequency Semiannual, daycount convention  ACT/365"<<endl;
	cout << "flat yield curve at 5%"<<endl;
	cout << endl;

	cout <<"*** for the treasury bond ***"<<endl;
	cout << "fairvalue  " << testTbond.fairvalue()  << endl;
	cout << "yield to maturity " << testTbond.yieldToMaturity()  << endl;
	cout << "convexity  " << testTbond.convexity()  << endl;
	cout << "duration  " << testTbond.duration()  << endl;
	cout << endl;

	cout <<"*** for the risky bond ***"<<endl;
	cout << "fairvalue  " << testRbond.fairvalue()  << endl;
	cout << "yield to maturity " << testRbond.yieldToMaturity()  << endl;
	cout << "convexity  " << testRbond.convexity()  << endl;
	cout << "duration  " << testRbond.duration()  << endl;
	
//	Integer i;
//	treasurybond testTb=testTbond;
//	for(i=0;i<10;i++){
//		Real shift = 0.01+0.03*i;
//		testTb = testTbond.shiftedbond(shift);
//		cout << "fairvalue  "  <<testTb.fairvalue(issue)  << endl;
//	}
 	return true;
}