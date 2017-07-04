#include"..\PartB\yieldCurve.h"
#include "../common/filereader.h"

#include<iostream>
#include<valarray>
#include<time.h>
using namespace std;

/**
@author Yann
*/

bool mainyieldcurve(void) {
	string datadir = FileReader::getdatadirasstring();
	string yieldCurveData = datadir + "yctest.csv";
	Real shift=0.001,calculation=0.,matRotation=7;
	bool fctnResult=true;

	// the underlying curve
	valarray<yieldPoint> *rates = 
		FileReader::buildYieldPointArray(yieldCurveData);

	yieldCurve testcurve=yieldCurve(*rates,"test");

	valarray<Real> maturities = testcurve.getMaturitiesInTheZCBCurve();
	valarray<Real> ZCB(maturities.size());

	yieldCurve ycShifted=testcurve.shiftZCBRateCurve(shift);
	valarray<Real> ZCBShift(maturities.size());
	yieldCurve ycRotated=testcurve.rotateZCBRateCurve(shift,matRotation);
	valarray<Real> ZCBRotate(maturities.size());
	
	cout<<fctnResult<<endl;

	for (Natural i = 0; i < ZCB.size(); i++) {
		ZCB[i] = testcurve.spotRate(maturities[i]);
		ZCBShift[i]=ycShifted.spotRate(maturities[i]);
		fctnResult&=(ZCBShift[i]==ZCB[i]+shift);
	}

	for(i=0;i<rates->size();i++)
		cout << "Mkt Rate: " << (*rates)[i].getRate()*100 << " (mat: " << (*rates)[i].getMaturity()
			<< " ; type: " << (*rates)[i].getType() << ")"<<endl;

	cout << "ZCB rates:\n" << testcurve << endl;

	string filename = FileReader::getdatadirasstring() + "test_YC_PCA.txt"; 
	ofstream myfile(filename.c_str()); 
	myfile << "Original curve\n"<<testcurve<<"\n"; 
	myfile << "Shifted curve\n"<<ycShifted<<"\n"; 
	myfile << "Rotated curve\n"<<ycRotated<<"\n"; 
	myfile << "DF\n";
	for(Real T=0.25;T<=30;T+=0.25)
		myfile << testcurve.discountFactor(T,Continuous)<<"\n";
	myfile << "6m Fwd\n";
	for(Real T=0.25;T<=29.5;T+=0.25)
		myfile << testcurve.forwardRate(0.5,T,Continuous)<<"\n";
	myfile.close();


	calculation=testcurve.spotRate (3.72);
	cout<<"\n Test Spot rate at 3.72 years "<<calculation<<endl;
	fctnResult&=(calculation>=0.0463701)&&(calculation<=0.0463702);

	calculation=testcurve.spotRate (14);
	cout<<"\n Test Spot rate at 14 years "<<calculation<<endl;
	fctnResult&=(calculation>=0.0494384)&&(calculation<=0.0494385);

	calculation =testcurve.discountFactor (5.6,Continuous);
	cout<<"\n Test DF at 5.6 years cont "<<calculation <<endl;
	fctnResult&=(calculation>=0.7688138)&&(calculation<=0.7688142);

	calculation =testcurve.discountFactor (5.6,Discrete);
	cout<<"\n Test DF at 5.6 years disc "<<calculation <<endl;
	fctnResult&=(calculation>=0.7734288)&&(calculation<=0.7734292);
	
	calculation =testcurve.discountFactor (1.5);
	cout<<"\n Test DF at 1.5 years "<<calculation<<endl;
	fctnResult&=(calculation>=0.934078)&&(calculation<=0.934079);

	Date d=Date();
	d.setDateToToday ();
	calculation =testcurve.discountFactor(d.plusMonths(18));
	cout<<"\n Test from today to 18 months : "<<calculation<<endl;
	fctnResult&=(calculation>=0.930458)&&(calculation<=0.930459);

	calculation=testcurve.discountFactor (Date(18,(Month)6,2007));
	cout<<"\n Test DF in Date (18/6/2007)"<<calculation <<endl;
	fctnResult&=(calculation>=0.930458)&&(calculation<=0.930459);

	calculation=testcurve.discountFactor (4);
	cout<<"\n Test DF at 4 years "<<calculation<<endl;
	fctnResult&=(calculation>=0.830441)&&(calculation<=0.830442);

	calculation=testcurve.forwardRate(1.5,2.5,Continuous);
	cout<<"\n Test FWD at (1.5,2.5) <=> 4 years maturity cont "<<calculation<<endl;
	fctnResult&=(calculation>=0.0470409)&&(calculation<=0.0470410);

	calculation=testcurve.forwardRate(1.5,2.5,Discrete);
	cout<<"\n Test FWD at (1.5,2.5) <=> 4 years maturity disc "<<calculation<<endl;
	fctnResult&=(calculation>=0.0470413)&&(calculation<=0.0470414);

	return fctnResult;
}