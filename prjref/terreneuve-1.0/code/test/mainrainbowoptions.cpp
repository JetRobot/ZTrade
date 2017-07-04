#include "../Interface/main.h"
#include "../PartI/rainbowoption.h"
#include"../PartB/YieldCurve.h"

#include<iostream>
#include<valarray>
using namespace std;

/**
@author Yann
*/

bool mainrainbowoptions(void){
	cout<<"Test Rainbows with S1=100, S2=120, K=110, vols at 0.20, rates at 0.10 for 1 Year - correl at 0.5"<<endl;
	Date start=Date();
	start.setDateToToday();
	Real exp=1.0;//1Y option
	Real Strike=110;
	yieldCurve yc=yieldCurve(0.1);
	volsurface v1=volsurface(0.2);
	volsurface v2=volsurface(0.2);
	valarray<volsurface> vols(2);
	vols[0]=v1;
	vols[1]=v2;
	Real Spot1=100;
	Real Spot2=120;
	Real Mult=1;
	Real Correl12=0.5;
	Real weight1=1;
	Real weight2=1;
	bool outputmsg=false;

	Real tempPrice=0.0;

	/* 	Types 	(MonteCarlo,	ClosedForm	)
		SpreadOptionMax,	AssetsBasketMax,	BestOf2AssetsCash,	WorstOf2AssetsCash,	BetterOf2Assets,
		WorseOf2Assets,		Max2AssetsCall,		Min2AssetsCall,		Max2AssetsPut,		Min2AssetsPut
	*/
	RainbowOption rb1=RainbowOption(SpreadOptionMax,start,exp,Strike,yc,vols,Spot1,Spot2,Mult,Correl12,weight1,weight2,outputmsg);
	RainbowOption rb2;// used for specific producs to adjust the strike

//	 no closed form
	cout<<"No closed form solution for these"<<endl;
	//this one will be used for spread to have a reasonnable strike for the spread
	rb2=RainbowOption(SpreadOptionMax,start,exp,0.0,yc,vols,Spot1,Spot2,Mult,Correl12,weight1,weight2,outputmsg);

	tempPrice=rb2.getPrice();
	cout<<"Price2SpreadOptionMaxByMC - (Strike = 0.0) : "<<tempPrice<<endl;
	cout<<"delta 1: "<<rb2.getPartialDelta(1)<<endl;
	cout<<"delta 2: "<<rb2.getPartialDelta(2)<<endl;
	cout<<"gamma 1: "<<rb2.getPartialGamma(1)<<endl;
	cout<<"gamma 2: "<<rb2.getPartialGamma(2)<<endl;
	cout<<"vega 1: "<<rb2.getPartialVega(1)<<endl;
	cout<<"vega 2: "<<rb2.getPartialVega(2)<<endl;
	cout<<"correl risk : "<<rb2.getCorrelRisk()<<endl;
	cout<<"Rho : "<<rb2.getRho()<<endl;

	rb1.setRainbowType(AssetsBasketMax);
	tempPrice=rb1.getPrice();
	cout<<"Price2AssetsBasketMaxByMC : "<<tempPrice<<endl;
	cout<<"delta 1: "<<rb1.getPartialDelta(1)<<endl;
	cout<<"delta 2: "<<rb1.getPartialDelta(2)<<endl;
	cout<<"gamma 1: "<<rb1.getPartialGamma(1)<<endl;
	cout<<"gamma 2: "<<rb1.getPartialGamma(2)<<endl;
	cout<<"vega 1: "<<rb1.getPartialVega(1)<<endl;
	cout<<"vega 2: "<<rb1.getPartialVega(2)<<endl;
	cout<<"correl risk : "<<rb1.getCorrelRisk()<<endl;
	cout<<"Rho : "<<rb1.getRho()<<endl;
	
	//did not find closed form, should be one
	cout<<"\nWorstOf2AssetsCash"<<endl;
	rb1.setRainbowType(WorstOf2AssetsCash);
	tempPrice=rb1.getPrice();
	cout<<"PriceWorstOf2AssetsCashByMC : "<<tempPrice<<endl;
	cout<<"delta 1: "<<rb1.getPartialDelta(1)<<endl;
	cout<<"delta 2: "<<rb1.getPartialDelta(2)<<endl;
	cout<<"gamma 1: "<<rb1.getPartialGamma(1)<<endl;
	cout<<"gamma 2: "<<rb1.getPartialGamma(2)<<endl;
	cout<<"vega 1: "<<rb1.getPartialVega(1)<<endl;
	cout<<"vega 2: "<<rb1.getPartialVega(2)<<endl;
	cout<<"correl risk : "<<rb1.getCorrelRisk()<<endl;
	cout<<"Rho : "<<rb1.getRho()<<endl;

	//comparisons
	cout<<"\nCompare Best Of 2 Assets plus cash"<<endl;
	rb1.setRainbowType(BestOf2AssetsCash);
	tempPrice=rb1.getPrice(MonteCarlo);
	cout<<"PriceBestOf2AssetsCashByMC 1: "<<tempPrice<<endl;
	tempPrice=rb1.getPrice(ClosedForm);
	cout<<"PriceByClosedForm_BestOf2_plusCash : "<<tempPrice<<endl;
	cout<<"delta 1: "<<rb1.getPartialDelta(1)<<endl;
	cout<<"delta 2: "<<rb1.getPartialDelta(2)<<endl;
	cout<<"gamma 1: "<<rb1.getPartialGamma(1)<<endl;
	cout<<"gamma 2: "<<rb1.getPartialGamma(2)<<endl;
	cout<<"vega 1: "<<rb1.getPartialVega(1)<<endl;
	cout<<"vega 2: "<<rb1.getPartialVega(2)<<endl;
	cout<<"correl risk : "<<rb1.getCorrelRisk()<<endl;
	cout<<"Rho : "<<rb1.getRho()<<endl;

	cout<<"\nCompare Max Of 2 Assets Call"<<endl;
	rb1.setRainbowType(Max2AssetsCall);
	tempPrice=rb1.getPrice(MonteCarlo);
	cout<<"PriceMax2AssetsByMCCall : "<<tempPrice<<endl;
	tempPrice=rb1.getPrice(ClosedForm);
	cout<<"PriceByClosedForm_MaxOf2_call : "<<tempPrice<<endl;
	cout<<"delta 1: "<<rb1.getPartialDelta(1)<<endl;
	cout<<"delta 2: "<<rb1.getPartialDelta(2)<<endl;
	cout<<"gamma 1: "<<rb1.getPartialGamma(1)<<endl;
	cout<<"gamma 2: "<<rb1.getPartialGamma(2)<<endl;
	cout<<"vega 1: "<<rb1.getPartialVega(1)<<endl;
	cout<<"vega 2: "<<rb1.getPartialVega(2)<<endl;
	cout<<"correl risk : "<<rb1.getCorrelRisk()<<endl;
	cout<<"Rho : "<<rb1.getRho()<<endl;

	cout<<"\nCompare Min Of 2 Assets Call"<<endl;
	rb1.setRainbowType(Min2AssetsCall);
	tempPrice=rb1.getPrice(MonteCarlo);
	cout<<"PriceMin2AssetsByMCCall : "<<tempPrice<<endl;
	tempPrice=rb1.getPrice(ClosedForm);
	cout<<"PriceByClosedForm_MinOf2_call : "<<tempPrice<<endl;
	cout<<"delta 1: "<<rb1.getPartialDelta(1)<<endl;
	cout<<"delta 2: "<<rb1.getPartialDelta(2)<<endl;
	cout<<"gamma 1: "<<rb1.getPartialGamma(1)<<endl;
	cout<<"gamma 2: "<<rb1.getPartialGamma(2)<<endl;
	cout<<"vega 1: "<<rb1.getPartialVega(1)<<endl;
	cout<<"vega 2: "<<rb1.getPartialVega(2)<<endl;
	cout<<"correl risk : "<<rb1.getCorrelRisk()<<endl;
	cout<<"Rho : "<<rb1.getRho()<<endl;

	cout<<"\nCompare Max Of 2 Assets Put"<<endl;
	rb1.setRainbowType(Max2AssetsPut);
	tempPrice=rb1.getPrice(MonteCarlo);
	cout<<"PriceMax2AssetsByMCPut : "<<tempPrice<<endl;
	tempPrice=rb1.getPrice(ClosedForm);
	cout<<"PriceByClosedForm_MaxOf2_put : "<<tempPrice<<endl;
	cout<<"delta 1: "<<rb1.getPartialDelta(1)<<endl;
	cout<<"delta 2: "<<rb1.getPartialDelta(2)<<endl;
	cout<<"gamma 1: "<<rb1.getPartialGamma(1)<<endl;
	cout<<"gamma 2: "<<rb1.getPartialGamma(2)<<endl;
	cout<<"vega 1: "<<rb1.getPartialVega(1)<<endl;
	cout<<"vega 2: "<<rb1.getPartialVega(2)<<endl;
	cout<<"correl risk : "<<rb1.getCorrelRisk()<<endl;
	cout<<"Rho : "<<rb1.getRho()<<endl;

	cout<<"\nCompare Min Of 2 Assets Put"<<endl;
	rb1.setRainbowType(Min2AssetsPut);
	tempPrice=rb1.getPrice(MonteCarlo);
	cout<<"PriceMin2AssetsByMCPut : "<<tempPrice<<endl;
	tempPrice=rb1.getPrice(ClosedForm);
	cout<<"PriceByClosedForm_MinOf2_put : "<<tempPrice<<endl;
	cout<<"delta 1: "<<rb1.getPartialDelta(1)<<endl;
	cout<<"delta 2: "<<rb1.getPartialDelta(2)<<endl;
	cout<<"gamma 1: "<<rb1.getPartialGamma(1)<<endl;
	cout<<"gamma 2: "<<rb1.getPartialGamma(2)<<endl;
	cout<<"vega 1: "<<rb1.getPartialVega(1)<<endl;
	cout<<"vega 2: "<<rb1.getPartialVega(2)<<endl;
	cout<<"correl risk : "<<rb1.getCorrelRisk()<<endl;
	cout<<"Rho : "<<rb1.getRho()<<endl;
	
	cout<<"\nCompare Better of 2"<<endl;
	rb1.setRainbowType(BetterOf2Assets);
	tempPrice=rb1.getPrice(MonteCarlo);
	cout<<"PriceByMc_BetterOf2Assets : "<<tempPrice<<endl;
	tempPrice=rb1.getPrice(ClosedForm);
	cout<<"PriceByClosedForm_BetterOf2 : "<<tempPrice<<endl;
	cout<<"delta 1: "<<rb1.getPartialDelta(1)<<endl;
	cout<<"delta 2: "<<rb1.getPartialDelta(2)<<endl;
	cout<<"gamma 1: "<<rb1.getPartialGamma(1)<<endl;
	cout<<"gamma 2: "<<rb1.getPartialGamma(2)<<endl;
	cout<<"vega 1: "<<rb1.getPartialVega(1)<<endl;
	cout<<"vega 2: "<<rb1.getPartialVega(2)<<endl;
	cout<<"correl risk : "<<rb1.getCorrelRisk()<<endl;
	cout<<"Rho : "<<rb1.getRho()<<endl;

	cout<<"\nCompare Worse of 2"<<endl;
	rb1.setRainbowType(WorseOf2Assets);
	tempPrice=rb1.getPrice(MonteCarlo);
	cout<<"PriceByMc_WorseOf2Assets : "<<tempPrice<<endl;
	tempPrice=rb1.getPrice(ClosedForm);
	cout<<"PriceByClosedForm_WorseOf2 : "<<tempPrice<<endl;
	cout<<"delta 1: "<<rb1.getPartialDelta(1)<<endl;
	cout<<"delta 2: "<<rb1.getPartialDelta(2)<<endl;
	cout<<"gamma 1: "<<rb1.getPartialGamma(1)<<endl;
	cout<<"gamma 2: "<<rb1.getPartialGamma(2)<<endl;
	cout<<"vega 1: "<<rb1.getPartialVega(1)<<endl;
	cout<<"vega 2: "<<rb1.getPartialVega(2)<<endl;
	cout<<"vega 1: "<<rb1.getPartialVega(1,MonteCarlo)<<endl;
	cout<<"vega 2: "<<rb1.getPartialVega(2,MonteCarlo)<<endl;
	cout<<"correl risk : "<<rb1.getCorrelRisk()<<endl;
	cout<<"Rho : "<<rb1.getRho()<<endl;




	// to generate set of prices for the differences
	// 5 hours to run ...

//	
//	Real S1,S2,K,vol1,vol2,correl;
//	Real price,d1,d2,g1,g2,v1,v2,rho,corr,theta;
//	yieldCurve yctest=yieldCurve(0.05);	
//	volsurface vs1,vs2;
//	valarray<volsurface> vols(2);
//	Date start=Date();
//	start.setDateToToday();
//	Real exp=2.0;//1Y option
//
//	RainbowOption rb2;
//	ofstream myfile;
//	myfile.open ("c:/resRainbow.txt");
//	myfile<<"2Y Rainbow Options with 0.05 rate to maturity, one spot at 100 - Results\n";
//	myfile<<"S1,S2,K,correl,vol1,vol2,Type,Method,Price,delta1,delta2,gamma1,gamma2,Vega1,Vega2,Rho,Correl,Theta\n";
//
//	for(S1=80;S1<=120;S1+=20){
//		for(S2=100;S2<=100;S2+=10){//symetry so only one oscillation should be enough
//			for(K=80;K<130;K+=20){
//				for(correl=-1;correl<=1;correl+=0.5){
//					for(vol1=0.1;vol1<0.4;vol1+=0.1){
//						for(vol2=0.2;vol2<=0.2;vol2+=0.1){//symetry
//							vs1=volsurface(vol1);
//							vs2=volsurface(vol2);
//							vols[0]=vs1;
//							vols[1]=vs2;
////
//							rb2=RainbowOption(BestOf2AssetsCash,start,exp,K,yctest,vols,S1,S2,1,correl,1,1,false);
//
//							price=rb2.getPrice(MonteCarlo);
//							d1=rb2.getPartialDelta(1,MonteCarlo);
//							d2=rb2.getPartialDelta(2,MonteCarlo);
//							g1=rb2.getPartialGamma(1,MonteCarlo);
//							g2=rb2.getPartialGamma(2,MonteCarlo);
//							v1=rb2.getPartialVega(1,MonteCarlo);
//							v2=rb2.getPartialVega(2,MonteCarlo);
//							corr=rb2.getCorrelRisk(MonteCarlo);
//							rho=rb2.getRho(MonteCarlo);
//							theta=rb2.getTheta(MonteCarlo);
//
//							myfile <<S1<<","<<S2<<","<<K<<","<<correl<<","<<vol1<<","<<vol2<<","<<"BOCash"<<","<<"MC"<<","<<price<<","<<d1<<","<<d2<<","<<g1<<","<<g2<<","<<v1<<","<<v2<<","<<rho<<","<<corr<<","<<theta<<"\n";
//
//							price=rb2.getPrice(ClosedForm);
//							d1=rb2.getPartialDelta(1,ClosedForm);
//							d2=rb2.getPartialDelta(2,ClosedForm);
//							g1=rb2.getPartialGamma(1,ClosedForm);
//							g2=rb2.getPartialGamma(2,ClosedForm);
//							v1=rb2.getPartialVega(1,ClosedForm);
//							v2=rb2.getPartialVega(2,ClosedForm);
//							corr=rb2.getCorrelRisk(ClosedForm);
//							rho=rb2.getRho(ClosedForm);
//							theta=rb2.getTheta(ClosedForm);
//
//							myfile <<S1<<","<<S2<<","<<K<<","<<correl<<","<<vol1<<","<<vol2<<","<<"BOCash"<<","<<"CF"<<","<<price<<","<<d1<<","<<d2<<","<<g1<<","<<g2<<","<<v1<<","<<v2<<","<<rho<<","<<corr<<","<<theta<<"\n";
//
//							rb2.setRainbowType(Max2AssetsCall);
//
//							price=rb2.getPrice(MonteCarlo);
//							d1=rb2.getPartialDelta(1,MonteCarlo);
//							d2=rb2.getPartialDelta(2,MonteCarlo);
//							g1=rb2.getPartialGamma(1,MonteCarlo);
//							g2=rb2.getPartialGamma(2,MonteCarlo);
//							v1=rb2.getPartialVega(1,MonteCarlo);
//							v2=rb2.getPartialVega(2,MonteCarlo);
//							corr=rb2.getCorrelRisk(MonteCarlo);
//							rho=rb2.getRho(MonteCarlo);
//							theta=rb2.getTheta(MonteCarlo);
//
//							myfile <<S1<<","<<S2<<","<<K<<","<<correl<<","<<vol1<<","<<vol2<<","<<"MaxCall"<<","<<"MC"<<","<<price<<","<<d1<<","<<d2<<","<<g1<<","<<g2<<","<<v1<<","<<v2<<","<<rho<<","<<corr<<","<<theta<<"\n";
//
//							price=rb2.getPrice(ClosedForm);
//							d1=rb2.getPartialDelta(1,ClosedForm);
//							d2=rb2.getPartialDelta(2,ClosedForm);
//							g1=rb2.getPartialGamma(1,ClosedForm);
//							g2=rb2.getPartialGamma(2,ClosedForm);
//							v1=rb2.getPartialVega(1,ClosedForm);
//							v2=rb2.getPartialVega(2,ClosedForm);
//							corr=rb2.getCorrelRisk(ClosedForm);
//							rho=rb2.getRho(ClosedForm);
//							theta=rb2.getTheta(ClosedForm);
//
//							myfile <<S1<<","<<S2<<","<<K<<","<<correl<<","<<vol1<<","<<vol2<<","<<"MaxCall"<<","<<"CF"<<","<<price<<","<<d1<<","<<d2<<","<<g1<<","<<g2<<","<<v1<<","<<v2<<","<<rho<<","<<corr<<","<<theta<<"\n";
////
//							rb2.setRainbowType(Min2AssetsPut);
//
//							price=rb2.getPrice(MonteCarlo);
//							d1=rb2.getPartialDelta(1,MonteCarlo);
//							d2=rb2.getPartialDelta(2,MonteCarlo);
//							g1=rb2.getPartialGamma(1,MonteCarlo);
//							g2=rb2.getPartialGamma(2,MonteCarlo);
//							v1=rb2.getPartialVega(1,MonteCarlo);
//							v2=rb2.getPartialVega(2,MonteCarlo);
//							corr=rb2.getCorrelRisk(MonteCarlo);
//							rho=rb2.getRho(MonteCarlo);
//							theta=rb2.getTheta(MonteCarlo);
//
//							myfile <<S1<<","<<S2<<","<<K<<","<<correl<<","<<vol1<<","<<vol2<<","<<"MinPut"<<","<<"MC"<<","<<price<<","<<d1<<","<<d2<<","<<g1<<","<<g2<<","<<v1<<","<<v2<<","<<rho<<","<<corr<<","<<theta<<"\n";
//
//							price=rb2.getPrice(ClosedForm);
//							d1=rb2.getPartialDelta(1,ClosedForm);
//							d2=rb2.getPartialDelta(2,ClosedForm);
//							g1=rb2.getPartialGamma(1,ClosedForm);
//							g2=rb2.getPartialGamma(2,ClosedForm);
//							v1=rb2.getPartialVega(1,ClosedForm);
//							v2=rb2.getPartialVega(2,ClosedForm);
//							corr=rb2.getCorrelRisk(ClosedForm);
//							rho=rb2.getRho(ClosedForm);
//							theta=rb2.getTheta(ClosedForm);
//
//							myfile <<S1<<","<<S2<<","<<K<<","<<correl<<","<<vol1<<","<<vol2<<","<<"MinPut"<<","<<"CF"<<","<<price<<","<<d1<<","<<d2<<","<<g1<<","<<g2<<","<<v1<<","<<v2<<","<<rho<<","<<corr<<","<<theta<<"\n";
//
//						}
//					}
//				}
//			}
//		}
//	}
//	myfile.close();


	// output greeks and price vs strike - All with closed form, else it is a pain
	//ofstream myfile;
	//myfile.open ("c:/resRainbowPerStrike.txt");

	//myfile<<"Test Rainbows with S1=100, S2=100, vols at 0.20, rates at 0.10 for 1 Year"<<"\n";
	//Date start=Date();
	//start.setDateToToday();
	//Real exp=1.0;//1Y option
	//Real Strike;;
	//yieldCurve yc=yieldCurve(0.1);
	//volsurface v1=volsurface(0.2);
	//volsurface v2=volsurface(0.2);
	//valarray<volsurface> vols(2);
	//vols[0]=v1;
	//vols[1]=v2;
	//Real Spot1=100;
	//Real Spot2=100;
	//Real Mult=1;
	//Real Correl12=0.5;
	//Real weight1=1;
	//Real weight2=1;
	//bool outputmsg=false;

	//Real tempPrice=0.0;

	///* 	Types 	(MonteCarlo,	ClosedForm	)
	//	BestOf2AssetsCash,	WorstOf2AssetsCash,	BetterOf2Assets,
	//	Max2AssetsCall,		Min2AssetsCall,		Max2AssetsPut,		Min2AssetsPut
	//*/
	//RainbowOption rb1;
	//myfile<<"Strike,Price,Delta,Gamma,Vega,Correl,Rho\n";
	//myfile<<"BOCash";
	//for(Strike=50;Strike<=150;Strike+=5){
	//	rb1=RainbowOption(BestOf2AssetsCash,start,exp,Strike,yc,vols,Spot1,Spot2,Mult,Correl12,weight1,weight2,outputmsg);
	//	myfile<<Strike<<","<<rb1.getPrice();
	//	myfile<<","<<rb1.getPartialDelta(1);
	//	myfile<<","<<rb1.getPartialGamma(1);
	//	myfile<<","<<rb1.getPartialVega(1);
	//	myfile<<","<<rb1.getCorrelRisk();
	//	myfile<<","<<rb1.getRho();
	//	myfile<<"\n";
	//}
	//myfile<<"Better";
	//for(Strike=50;Strike<=150;Strike+=5){
	//	rb1=RainbowOption(BetterOf2Assets,start,exp,Strike,yc,vols,Spot1,Spot2,Mult,Correl12,weight1,weight2,outputmsg);
	//	myfile<<Strike<<","<<rb1.getPrice();
	//	myfile<<","<<rb1.getPartialDelta(1);
	//	myfile<<","<<rb1.getPartialGamma(1);
	//	myfile<<","<<rb1.getPartialVega(1);
	//	myfile<<","<<rb1.getCorrelRisk();
	//	myfile<<","<<rb1.getRho();
	//	myfile<<"\n";
	//}
	//myfile<<"Worse";
	//for(Strike=50;Strike<=150;Strike+=5){
	//	rb1=RainbowOption(WorseOf2Assets,start,exp,Strike,yc,vols,Spot1,Spot2,Mult,Correl12,weight1,weight2,outputmsg);
	//	myfile<<Strike<<","<<rb1.getPrice();
	//	myfile<<","<<rb1.getPartialDelta(1);
	//	myfile<<","<<rb1.getPartialGamma(1);
	//	myfile<<","<<rb1.getPartialVega(1);
	//	myfile<<","<<rb1.getCorrelRisk();
	//	myfile<<","<<rb1.getRho();
	//	myfile<<"\n";
	//}
	//myfile<<"MaxCall";
	//for(Strike=50;Strike<=150;Strike+=5){
	//	rb1=RainbowOption(Max2AssetsCall,start,exp,Strike,yc,vols,Spot1,Spot2,Mult,Correl12,weight1,weight2,outputmsg);
	//	myfile<<Strike<<","<<rb1.getPrice();
	//	myfile<<","<<rb1.getPartialDelta(1);
	//	myfile<<","<<rb1.getPartialGamma(1);
	//	myfile<<","<<rb1.getPartialVega(1);
	//	myfile<<","<<rb1.getCorrelRisk();
	//	myfile<<","<<rb1.getRho();
	//	myfile<<"\n";
	//}
	//myfile<<"MinCall";
	//for(Strike=50;Strike<=150;Strike+=5){
	//	rb1=RainbowOption(Min2AssetsCall,start,exp,Strike,yc,vols,Spot1,Spot2,Mult,Correl12,weight1,weight2,outputmsg);
	//	myfile<<Strike<<","<<rb1.getPrice();
	//	myfile<<","<<rb1.getPartialDelta(1);
	//	myfile<<","<<rb1.getPartialGamma(1);
	//	myfile<<","<<rb1.getPartialVega(1);
	//	myfile<<","<<rb1.getCorrelRisk();
	//	myfile<<","<<rb1.getRho();
	//	myfile<<"\n";
	//}
	//myfile<<"MaxPut";
	//for(Strike=50;Strike<=150;Strike+=5){
	//	rb1=RainbowOption(Max2AssetsPut,start,exp,Strike,yc,vols,Spot1,Spot2,Mult,Correl12,weight1,weight2,outputmsg);
	//	myfile<<Strike<<","<<rb1.getPrice();
	//	myfile<<","<<rb1.getPartialDelta(1);
	//	myfile<<","<<rb1.getPartialGamma(1);
	//	myfile<<","<<rb1.getPartialVega(1);
	//	myfile<<","<<rb1.getCorrelRisk();
	//	myfile<<","<<rb1.getRho();
	//	myfile<<"\n";
	//}
	//myfile<<"MinPut";
	//for(Strike=50;Strike<=150;Strike+=5){
	//	rb1=RainbowOption(Min2AssetsPut,start,exp,Strike,yc,vols,Spot1,Spot2,Mult,Correl12,weight1,weight2,outputmsg);
	//	myfile<<Strike<<","<<rb1.getPrice();
	//	myfile<<","<<rb1.getPartialDelta(1);
	//	myfile<<","<<rb1.getPartialGamma(1);
	//	myfile<<","<<rb1.getPartialVega(1);
	//	myfile<<","<<rb1.getCorrelRisk();
	//	myfile<<","<<rb1.getRho();
	//	myfile<<"\n";
	//}
	//myfile.close();

// for Jason
	//Date start=Date();
	//start.setDateToToday();
	//Real exp=1.0;//1Y option
	//Real Strike=100;
	//yieldCurve yc=yieldCurve(0.0457296);
	//volsurface v1=volsurface(0.2);
	//volsurface v2=volsurface(0.2);
	//valarray<volsurface> vols(2);
	//vols[0]=v1;
	//vols[1]=v2;
	//Real Spot1=100;
	//Real Spot2=105;
	//Real Mult=1;
	//Real Correl12=0.4;
	//Real weight1=0.5;
	//Real weight2=0.5;
	//bool outputmsg=false;

	//Real tempPrice=0.0;

	//RainbowOption rb1=RainbowOption(SpreadOptionMax,start,exp,Strike,yc,vols,Spot1,Spot2,Mult,Correl12,weight1,weight2,outputmsg);

	////did not find closed form, should be one
	//cout<<"\nWorstOf2AssetsCash<<endl;
	//rb1.setRainbowType(WorstOf2AssetsCash);
	//tempPrice=rb1.getPrice();
	//cout<<"PriceWorstOf2AssetsCashByMC : "<<tempPrice<<endl;

	////comparisons
	//cout<<"\nCompare Best Of 2 Assets plus cash"<<endl;
	//rb1.setRainbowType(BestOf2AssetsCash);
	//tempPrice=rb1.getPrice(MonteCarlo);
	//cout<<"PriceBestOf2AssetsCashByMC 1: "<<tempPrice<<endl;

	//cout<<"\nCompare Max Of 2 Assets Call"<<endl;
	//rb1.setRainbowType(Max2AssetsCall);
	//tempPrice=rb1.getPrice(MonteCarlo);
	//cout<<"PriceMax2AssetsByMCCall : "<<tempPrice<<endl;

	//cout<<"\nCompare Min Of 2 Assets Call"<<endl;
	//rb1.setRainbowType(Min2AssetsCall);
	//tempPrice=rb1.getPrice(MonteCarlo);
	//cout<<"PriceMin2AssetsByMCCall : "<<tempPrice<<endl;

	//cout<<"\nCompare Max Of 2 Assets Put"<<endl;
	//rb1.setRainbowType(Max2AssetsPut);
	//tempPrice=rb1.getPrice(MonteCarlo);
	//cout<<"PriceMax2AssetsByMCPut : "<<tempPrice<<endl;

	//cout<<"\nCompare Min Of 2 Assets Put"<<endl;
	//rb1.setRainbowType(Min2AssetsPut);
	//tempPrice=rb1.getPrice(MonteCarlo);
	//cout<<"PriceMin2AssetsByMCPut : "<<tempPrice<<endl;
	//
	//cout<<"\nCompare Better of 2"<<endl;
	//rb1.setRainbowType(BetterOf2Assets);
	//tempPrice=rb1.getPrice(MonteCarlo);
	//cout<<"PriceByMc_BetterOf2Assets : "<<tempPrice<<endl;

	//cout<<"\nCompare Worse of 2"<<endl;
	//rb1.setRainbowType(WorseOf2Assets);
	//tempPrice=rb1.getPrice(MonteCarlo);
	//cout<<"PriceByMc_WorseOf2Assets : "<<tempPrice<<endl;

	return false;
}
