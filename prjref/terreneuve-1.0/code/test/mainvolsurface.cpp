#include"..\PartB\yieldCurve.h"
#include"..\PartE\volsurface.h"
#include "..\common\filereader.h"

#include<iostream>
#include <fstream>

#include<valarray>
//#include<time.h>
using namespace std;

bool mainvolsurface(void) {
//	valarray<yieldPoint> *rates = 
//		FileReader::buildYieldPointArray(FileReader::getdatadirasstring() + 
//		"yctest.csv");
	yieldCurve testcurve=yieldCurve(0.05);

	volsurfaceparams *params =
		FileReader::buildVolSurfaceParams(FileReader::getdatadirasstring() +
		"voltest2.csv");

	Date t0(16,July,2005);
	Real stockprice = 2994.0;

	volsurface testvolsurf(stockprice, t0, testcurve, *params);
	testvolsurf.setvolsurface();


	Date datetest1(16,September,2005);
	Date datetest2(17,March,2006);
	Date datetest3(15,December,2006);
	Date datetest4(21,December,2007);
	Date datetest5(18,December,2009);
	Date datetest6(21,December,2012);

	Real striketest1=2395.95;
	Real striketest2=2545.69;
	Real striketest3=2695.44;
	Real striketest4=2770.31;
	Real striketest5=2845.19;
	Real striketest6=2920.06;
	Real striketest7=2994.93;
	Real striketest8=3069.81;
	Real striketest9=3144.68;
	Real striketest10=3294.43;
	


	Real voltest1=testvolsurf.volatility(striketest1, datetest1);
	Real voltest2=testvolsurf.volatility(striketest2, datetest1);
	Real voltest3=testvolsurf.volatility(striketest3, datetest1);
	Real voltest4=testvolsurf.volatility(striketest4, datetest1);
	Real voltest5=testvolsurf.volatility(striketest5, datetest1);
	Real voltest6=testvolsurf.volatility(striketest6, datetest2);
	Real voltest7=testvolsurf.volatility(striketest7, datetest2);
	Real voltest8=testvolsurf.volatility(striketest8, datetest2);
	Real voltest9=testvolsurf.volatility(striketest9, datetest2);
	Real voltest10=testvolsurf.volatility(striketest10, datetest2);
	Real voltest11=testvolsurf.volatility(striketest1, datetest4);
	Real voltest12=testvolsurf.volatility(striketest2, datetest4);
	Real voltest13=testvolsurf.volatility(striketest3, datetest4);
	Real voltest14=testvolsurf.volatility(striketest4, datetest4);
	Real voltest15=testvolsurf.volatility(striketest5, datetest4);
	Real voltest16=testvolsurf.volatility(striketest6, datetest6);
	Real voltest17=testvolsurf.volatility(striketest7, datetest6);
	Real voltest18=testvolsurf.volatility(striketest8, datetest6);
	Real voltest19=testvolsurf.volatility(striketest9, datetest6);
	Real voltest20=testvolsurf.volatility(striketest10, datetest6);

	cout << "volatility for K=" << striketest1 << "," << datetest1.toString() << " : " << voltest1 << endl;
	cout << "volatility for K=" << striketest2 << "," << datetest1.toString() << " : " << voltest2 << endl;
	cout << "volatility for K=" << striketest3 << "," << datetest1.toString() << " : " << voltest3 << endl;
	cout << "volatility for K=" << striketest4 << "," << datetest1.toString() << " : " << voltest4 << endl;
	cout << "volatility for K=" << striketest5 << "," << datetest1.toString() << " : " << voltest5 << endl;
	cout << "volatility for K=" << striketest6 << "," << datetest2.toString() << " : " << voltest6 << endl;
	cout << "volatility for K=" << striketest7 << "," << datetest2.toString() << " : " << voltest7 << endl;
	cout << "volatility for K=" << striketest8 << "," << datetest2.toString() << " : " << voltest8 << endl;
	cout << "volatility for K=" << striketest9 << "," << datetest2.toString() << " : " << voltest9 << endl;
	cout << "volatility for K=" << striketest10 << "," << datetest2.toString() << " : " << voltest10 << endl;
	cout << "volatility for K=" << striketest1 << "," << datetest4.toString() << " : " << voltest11 << endl;
	cout << "volatility for K=" << striketest2 << "," << datetest4.toString() << " : " << voltest12 << endl;
	cout << "volatility for K=" << striketest3 << "," << datetest4.toString() << " : " << voltest13 << endl;
	cout << "volatility for K=" << striketest4 << "," << datetest4.toString() << " : " << voltest14 << endl;
	cout << "volatility for K=" << striketest5 << "," << datetest4.toString() << " : " << voltest15 << endl;
	cout << "volatility for K=" << striketest6 << "," << datetest6.toString() << " : " << voltest16 << endl;
	cout << "volatility for K=" << striketest7 << "," << datetest6.toString() << " : " << voltest17 << endl;
	cout << "volatility for K=" << striketest8 << "," << datetest6.toString() << " : " << voltest18 << endl;
	cout << "volatility for K=" << striketest9 << "," << datetest6.toString() << " : " << voltest19 << endl;
	cout << "volatility for K=" << striketest10 << "," << datetest6.toString() << " : " << voltest20 << endl;

	return false;
}