#include "../Interface/main.h"
#include "../PartD/VanillaSwap.h"
#include <iostream>

using namespace std;

/** @author Yann*/

bool mainIRVanillaSwap(void) {
	cout<<"Let us create a 5 Years Annual Vanilla Swap - Fixed rate 0.0471"<<endl;
	Real priceSwapAn5Y=0.0;
	string datadir = FileReader::getdatadirasstring();
	string yieldCurveData = datadir + "yctest.csv";
	valarray<yieldPoint> *yieldCurveArray = 
		FileReader::buildYieldPointArray(yieldCurveData);
	yieldCurve* underlyingCurve=new yieldCurve(*yieldCurveArray, "test");

	Date d=Date();
	d.setDateToToday();
	
	valarray<Date> dates(5);
	valarray<Real> Notionals(5);
	for (int i=0;i<5;i++){
		dates[i]=d.plusYears(i+1);	
		Notionals[i]=1000000;
	}
	
	SwapLeg legsInformation=SwapLeg(dates,Notionals);

	CashFlow fixLeg=CashFlow(legsInformation,0.0471);
	CashFlow varLeg=CashFlow(legsInformation,*underlyingCurve);

	cout<<"Fixed "<<fixLeg.getFairValue(underlyingCurve)<<endl;
	cout<<"Float "<<varLeg.getFairValue(underlyingCurve)<<endl;

	VanillaSwap swap5YAnnual=VanillaSwap(fixLeg,varLeg,"fixed","floating",underlyingCurve);
	priceSwapAn5Y=swap5YAnnual.returnPrice();
	cout<<"Value "<<priceSwapAn5Y<<"\n"<<endl;
	// closed to par as we took a mkt swap annual rate !

	cout<<"Let us create a 4 Years Semi-Annual Vanilla Swap - Fixed rate 0.04641"<<endl;
	Real priceSwapSemiAn4Y=0.0;
	d.setDateToToday();	
	dates.resize(8);
	Notionals.resize(8);
	for (int i=0;i<8;i++){
		dates[i]=d.plusMonths((i+1)*6);	
		Notionals[i]=1000000;
	}
	
	legsInformation=SwapLeg(dates,Notionals);

	fixLeg=CashFlow(legsInformation,0.04641);
	varLeg=CashFlow(legsInformation,*underlyingCurve);

	cout<<"Fixed "<<fixLeg.getFairValue(underlyingCurve)<<endl;
	cout<<"Float "<<varLeg.getFairValue(underlyingCurve)<<endl;

	VanillaSwap swap4YSemiAnnual=VanillaSwap(fixLeg,varLeg,"fixed","floating",underlyingCurve);
	priceSwapSemiAn4Y=swap4YSemiAnnual.returnPrice();
	cout<<"Value "<<priceSwapSemiAn4Y<<endl;
	
	delete underlyingCurve;
	
	bool res=true;
	res&=(priceSwapAn5Y>1,050)&&(priceSwapAn5Y<1,052);
	res&=(priceSwapSemiAn4Y>152)&&(priceSwapSemiAn4Y<153);
	return res;
}