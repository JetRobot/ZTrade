#include "../PartA/BlackScholes/OptionStrategy.h"
#include "../PartK/VarianceSwap.h"

#include<iostream>
#include<valarray>
using namespace std;


bool mainvarianceswap(void) {

	//Create basket of options
	Real spot=100,strike1=90,strike2=110,vol1=0.2,vol2=0.22,vol3=0.21,mat=1.,rate=0.02;
	OptionStrategy* Strategy=new OptionStrategy();
	Strategy->addLongButterflySpread(spot,vol1,true,vol2,true,vol3,true,rate,strike1,strike2,mat,10);

	//Compute value of VIX index
	Real Kmin=200,Kmax=3500,step=10,Mat=1./12,SpotVix=1200,Fwd,ratevix=0.043,volvix=0.33;
	Real strike,result;
	Fwd=SpotVix*exp(ratevix/12);
	OptionStrategy* optionsVix=new OptionStrategy();
	unsigned int i=0;
	valarray<BlackScholes> bs;
	bs.resize(Natural((Kmax-Kmin)/step+1));
	while (Kmin+i*step<=Kmax) {
		strike=Kmin+i*step;
		if (strike<=Fwd)
			bs[i]=BlackScholes(SpotVix,volvix,true,ratevix,strike,Mat,Put);
		else
			bs[i]=BlackScholes(SpotVix,volvix,true,ratevix,strike,Mat,Call);
		optionsVix->addOneBlackScholesObject(&bs[i],1);
		i++;
	}
	VarianceSwap* vix=new VarianceSwap(optionsVix,Mat,Fwd);
	result=100*vix->getPrice();
	cout<<"The price of vix index is : "<<result<<" for a forward price of "<<Fwd<<" and vol of "<<volvix<<" and rate of "<<ratevix<<endl;

	//Create Variance Swap
	VarianceSwap myvarswap=VarianceSwap(Strategy,1.,100);
	Real price=myvarswap.getPrice();
	cout<<"The price of the variance swap is " << myvarswap.getPrice() <<endl;
	if (price>0.015 && price <0.02)	return true;
	else return false;
}