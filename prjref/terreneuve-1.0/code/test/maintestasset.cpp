#include"..\PartC\asset.h"
#include "../common/filereader.h"

#include<iostream>
#include<valarray>
#include<time.h>
using namespace std;


/**
@author Yann
*/

bool mainasset(void) {
	string datadir = FileReader::getdatadirasstring();
	string yieldCurveData = datadir + "yctest.csv";
	bool result=true;
	Real test=0.0;
	// the underlying curve
	valarray<yieldPoint> *rates = 
		FileReader::buildYieldPointArray(yieldCurveData);

	yieldCurve testcurve=yieldCurve(*rates,"test");

	Date d= Date();
	d.setDateToToday();
	d=d.plusYears (1);
	flowSchedule flow1=flowSchedule(d,0.05); //5% after 1Y
	d.setDateToToday();
	d=d.plusYears (2);
	flowSchedule flow2=flowSchedule(d,0.05); //5% after 2Y
	valarray<flowSchedule> divSched(2);
	divSched [0]=flow1;
	divSched [1]=flow2;
	asset a=asset(100,testcurve,false,0,divSched);
	d.setDateToToday();
	d=d.plusYears (1);
	d=d.plusDays (-1);
	
	test=a.forwardPrice(d);
	result&=(test==100);
	cout<<"Div Sched - fwd 1yr minus 1day of payment: "<<test<<endl;
	
	d=d.plusDays (+1);
	test=a.forwardPrice(d);
	result&=(test>95.2387)&&(test<95.2389);
	cout<<"Div Sched - fwd 1yr of payment: "<<test<<endl;
	
	d.setDateToToday();
	d=d.plusYears (2);
	test=a.forwardPrice(d);
	result&=(test>90.6942)&&(test<90.6944);
	cout<<"Div Sched - fwd 2yrs of payment: "<<test<<endl;

	a.setDivAsRate(0.05);
	d.setDateToToday();
	d=d.plusYears (1);
	test=a.forwardPrice(d);
	result&=(test>94.7273)&&(test<94.7275);
	cout<<"Fixed div Rate - fwd 1yr of payment: "<<test<<endl;
	
	// For output file
	valarray<flowSchedule> testFlows(10);
	d.setDateToToday();
	Real rate;
	for (Natural i=0;i<10;i++){
		d=d.plusYears (1);
		if (i % 2 == 0)
			rate =0.05;
		else
			rate =0.10;
		testFlows[i]=flowSchedule(d,rate);
	}

	asset b= asset(100,testcurve,false,0,testFlows,EUR);
	asset c= asset(100,testcurve,true,0.075);
	string filename = FileReader::getdatadirasstring() + "test_Asset.txt"; 
	ofstream myfile(filename.c_str()); 
	myfile << "Test Forward Price - Div Sched 0.05 even years, 0.10 odd years, for 10 years\n";
	d.setDateToToday();
	for (Natural i=1;i<=24;i++){
		d=d.plusMonths (6);
		myfile<<d.toString()<<" , "<<b.forwardPrice(d)<<"\n";
	}	
	myfile << "\nTest Forward Price - Fixed Rate at 0.075, for 10 years\n";
	d.setDateToToday();
	for (Natural i=1;i<=24;i++){
		d=d.plusMonths (6);
		myfile<<d.toString()<<" , "<<c.forwardPrice(d)<<"\n";
	}	
	myfile.close();

	return result;
}