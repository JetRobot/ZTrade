#include "../PartA/BlackScholes/OptionStrategy.h"
#include "../PartA/BlackScholes/BlackScholes.h"
#include <iostream>

bool mainoption(void) {
	//Test Black Scholes 
	double spot=100,strike=110,vol=0.2,mat=1.,rate=0.02;
	LongNatural nPaths=1000000,nDates=1;
	BlackScholes BS=BlackScholes(spot,vol,true,rate,strike,mat,Call);
	cout<<"B-S price"<<endl;
	cout<<BS.getPrice()<<endl;
	cout<<"B-S delta"<<endl;
	cout<<BS.getDelta()<<endl;
	cout<<"B-S gamma"<<endl;
	cout<<BS.getGamma()<<endl;
	cout<<"B-S vega"<<endl;
	cout<<BS.getVega()<<endl;
	cout<<"B-S theta"<<endl;
	cout<<BS.getTheta()<<endl;
	cout<<"B-S rho"<<endl;
	cout<<BS.getRho()<<endl;
	cout<<"B-S vol"<<endl;
	cout<<BS.getVolatility()<<endl;
}

bool mainoptionstrategy(void) {
	//Test Option-Strategy
	double spot=100,strike1=90,strike2=110,vol1=0.2,vol2=0.22,vol3=0.21,mat=1.,rate=0.02;
	OptionStrategy Strategy=OptionStrategy();
	Strategy.addLongButterflySpread(spot,vol1,true,vol2,true,vol3,true,rate,strike1,strike2,mat,10);
	cout<<"Price of Butterfly"<<endl;
	cout<<Strategy.returnPrice()<<endl;
	cout<<"Delta of Butterfly"<<endl;
	cout<<Strategy.getGlobalDelta()<<endl;
	return false;
}
