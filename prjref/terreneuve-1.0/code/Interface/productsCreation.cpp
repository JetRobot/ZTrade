#include ".\productscreation.h"


bool productsCreationMenu(marketData data)
{
	cout<<"\n**************************************"<<endl;
	cout<<"****** Products Creation Module ******"<<endl;
	cout<<"**************************************\n"<<endl;

	bool replay=true;
	while (replay){
		cout<<"\nThe available products are (type their number to use them, or another key to get back) :"<<endl;
		cout<<"1 - Black Scholes Call/Put"<<endl;
		cout<<"2 - Strategies combining Calls and Puts"<<endl;
		cout<<"3 - Exotics options on single underlying"<<endl;
		cout<<"4 - Treasury/Risky bond"<<endl;
		cout<<"5 - Vanilla interest rate swap"<<endl;
		cout<<"6 - Rainbow Options"<<endl;
		cout<<"7 - Convertible bond"<<endl;
		Natural choice=0;
		cin>>choice;
		switch(choice){
			case 1:
				inputBSOption(data);
				break;
			case 2:
				inputOptionStrategy(data);
				break;
			case 3:
				inputExoticOptionOnSingleAsset(data);
				break;
			case 4:
				inputBond(data);
				break;
			case 5:
				inputVanillaSwap(data);
				break;
			case 6:
				inputRainbowOption(data);
				break;
			case 7:
				inputConvertibleBond(data);
				break;
			default:
				replay=false;
				break;
		}
	}
}


BlackScholes * inputBSOption(marketData data)
{
	bool useData;
	Natural useMarketData;
	cout<<"\n**************************************"<<endl;
	cout<<"*** Input one Black Scholes option ***"<<endl;
	cout<<"**************************************\n"<<endl;
	cout<<"\nWould you like to use the market data?\nType 1 if yes, else any other key"<<endl;
	cin>>useMarketData;
	useData=(useMarketData==1);
	TypeOptionBS type;
	Real spot,rate,vol,K,T;
	bool isnotCorP=true;
	char callOrPut;
	while(isnotCorP){
		cout<<"\nDo you want to add a call or a put ? Type c or C for a call, and p or P"<<endl;
		cin>>callOrPut;
		isnotCorP=!(callOrPut=='c'||callOrPut=='C'||callOrPut=='p'||callOrPut=='P');
		if(isnotCorP)
			cout<<"Your entry did not seem valid"<<endl;
	}
	if(callOrPut=='c'||callOrPut=='C')
		type=Call;
	else // as we are sure he entered c, C, p or P
		type=Put;
	cout<<"What is the spot level of your underlying?"<<endl;
	cin>>spot;
	cout<<"What is the strike of your option?"<<endl;
	cin>>K;
	cout<<"What is the maturity in years of your option?"<<endl;
	cin>>T;
	if(useData){
		Date d;
		d.setDateToToday ();
		rate=data.yieldcurve.spotRate(T);
		vol=data.vols.volatility(K,d.plusDays((Integer)(365*T)));
	}
	else{
		cout<<"What is the rate to maturity you want to use? (in absolute value)"<<endl;
		cin>>rate;
		cout<<"What is the volatility to maturity you want to use? (in absolute value)"<<endl;
		cin>>vol;
	}
	BlackScholes* res=new BlackScholes(spot,vol,true,rate,K,T,type);
	cout<<"You have succesfully created a "<<K<<" "<<outputCallPut(callOrPut)<<" with maturity "<<T<<", vol "<<vol<<", rate "<<rate<<"; while the spot is at "<<spot<<endl;
	cout<<"Its characteristics are as follows:"<<endl;
	cout<<"- Price: "<<res->getPrice()<<endl;
	cout<<"- Delta: "<<res->getDelta()<<endl;
	cout<<"- Gamma: "<<res->getGamma()<<endl;
	cout<<"- Vega:  "<<res->getVega()<<endl;
	cout<<"- Theta: "<<res->getTheta()<<endl;
	cout<<"- Rho:   "<<res->getRho()<<"\n"<<endl;

	return res;
}

string outputCallPut(char c){
	string res;
	if(c=='c' || c=='C')
		res="Call";
	else if(c=='p' || c=='P')
			res="Put";
	else
		res = "";
	return res;
}

OptionStrategy inputOptionStrategy(marketData data)
{
	OptionStrategy strategy;
	cout<<"\n**************************************"<<endl;
	cout<<"****** Input an Option Strategy ******"<<endl;
	cout<<"**************************************\n"<<endl;
	cout<<"\nAn option strategy is a set of Black Sholes options, you will create them separately"<<endl;
	bool replay=true,addOneMoreOption,addBS;
	Natural addOption,addABS,replayInput;
	Real amount;
	Natural count =1;
	while(replay){
		addOneMoreOption=true;
		while(addOneMoreOption){
			cout<<"Add the option number "<<count<<":"<<endl;
			cout<<"Do you want add a single Call/Put or somehting else? Type 1 for Call/Put or another key for something else"<<endl;
			cin>>addABS;
			addBS=(addABS==1);
			if(addBS){
				BlackScholes* toAdd=inputBSOption(data);
				cout<<"How many of these would you like to add?"<<endl;
				cin>>amount;
				strategy.addOneBlackScholesObject(toAdd,amount);
			}
			else
				inputSpecificOptionStrategy(data,strategy);
			cout<<"\n**************************************"<<endl;
			cout<<"****** Input an Option Strategy ******"<<endl;
			cout<<"**************************************\n"<<endl;
			cout<<"\nWould you like to add one more? Type 1 for yes, else another key"<<endl;
			cin>>addOption;
			addOneMoreOption=(addOption==1);
			count++;
		}
		cout<<"The creation process is now over. Given what you added, the charateristics of the strategy are:"<<endl;
		cout<<"- Price: "<<strategy.returnPrice()<<endl;
		cout<<"- Delta: "<<strategy.getGlobalDelta()<<endl;
		cout<<"- Gamma: "<<strategy.getGlobalGamma()<<endl;
		cout<<"- Vega:  "<<strategy.getGlobalVega()<<endl;
		cout<<"- Theta: "<<strategy.getGlobalTheta()<<endl;
		cout<<"- Rho:   "<<strategy.getGlobalRho()<<"\n"<<endl;
		cout<<"Your strategy is now made of:\n"<<strategy<<endl;
		cout<<"Are you satisfied with your risk or would you like to add more options (adding negative is deleting)? Type 1 to add more or another key to stop here"<<endl;
		cin>>replayInput;
		replay=(replayInput==1);
	}

	return strategy;
}

void inputSpecificOptionStrategy(marketData data,OptionStrategy& strategy)
{
	bool incorrectChoice=true,useMarketData;
	Natural choice=0,useData;
	cout<<"\n**************************************"<<endl;
	cout<<" Add a specific option for a strategy "<<endl;
	cout<<"**************************************\n"<<endl;
	cout<<"\nWould you like to use the market data?\nType 1 if yes, else any other key"<<endl;
	cin>>useData;
	useMarketData=(useData==1);
	while(incorrectChoice){
		cout<<"What sort of option do you want to add? Press:"<<endl;
		cout<<"1 - for a ButterflySpread"<<endl;
		cout<<"2 - for a CallSpread"<<endl;
		cout<<"3 - for a PutSpread"<<endl;
		cout<<"4 - for a RatioCallSpread"<<endl;
		cout<<"5 - for a Straddle"<<endl;
		cout<<"6 - for a Strangle"<<endl;
		cin>>choice;
		incorrectChoice=((choice<=0)||(choice>6));
		if(incorrectChoice)
			cout<<"Your entry did not seem valid"<<endl;
	}
	switch (choice){
		case 1:
			inputButterflySpread(data,strategy,useMarketData);
			break;
		case 2:
			inputCallSpread(data,strategy,useMarketData);
			break;
		case 3:
			inputPutSpread(data,strategy,useMarketData);
			break;
		case 4:
			inputRatioCallSpread(data,strategy,useMarketData);
			break;
		case 5:
			inputStraddle(data,strategy,useMarketData);
			break;
		case 6:
			inputStrangle(data,strategy,useMarketData);
			break;
	}
}

void inputButterflySpread(marketData data,OptionStrategy& strategy,bool useMarketData){
	cout<<"\n**************************************"<<endl;
	cout<<" Add a Butterfly Spread to a strategy "<<endl;
	cout<<"**************************************\n"<<endl;
	Real spot,K1,K2,K3,T,rate,vol1,vol2,vol3,amount;
	Natural symetric;
	cout<<"Do you want a symetric butterfly? Type 1 for yes, else another key"<<endl;
	cin>>symetric;
	cout<<"What is the spot level of your underlying?"<<endl;
	cin>>spot;
	cout<<"What is the lowest strike of your option?"<<endl;
	cin>>K1;
	cout<<"What is the highest strike of your option?"<<endl;
	cin>>K2;
	if(symetric==1)
		K3=(K1+K2)/2.0;
	else{
		cout<<"What is the mid strike of the option ?"<<endl;
		cin>>K3;
		while(K3>K2 ||K3<K1){
			cout<<"The mid strike should be between K1 and K2 - can you please re enter it?"<<endl;
			cin>>K3;
		}
	}
	cout<<"What is the maturity in years of your option?"<<endl;
	cin>>T;
	if(useMarketData){
		Date d;
		d.setDateToToday ();
		rate=data.yieldcurve.spotRate(T);
		vol1=data.vols.volatility(K1,d.plusDays((Integer)(365*T)));
		vol2=data.vols.volatility(K2,d.plusDays((Integer)(365*T)));
		vol3=data.vols.volatility(K3,d.plusDays((Integer)(365*T)));
	}
	else{
		cout<<"What is the rate to maturity you want to use? (in absolute value)"<<endl;
		cin>>rate;
		cout<<"At strike K_low, what is the volatility to maturity you want to use? (in absolute value)"<<endl;
		cin>>vol1;
		cout<<"At strike K_high, what is the volatility to maturity you want to use? (in absolute value)"<<endl;
		cin>>vol2;
		cout<<"At strike K_mid, what is the volatility to maturity you want to use? (in absolute value)"<<endl;
		cin>>vol3;
	}
	cout<<"How many of these would you like to add?"<<endl;
	cin>>amount;
	strategy.addLongButterflySpread(spot,vol1,true,vol2,true,vol3,true,rate,K1,K2,K3,T,amount);
}

void inputCallSpread(marketData data,OptionStrategy& strategy,bool useMarketData){
	cout<<"\n**************************************"<<endl;
	cout<<"*** Add a CallSpread to a strategy ***"<<endl;
	cout<<"**************************************\n"<<endl;
	Real spot,K1,K2,T,rate,vol1,vol2,amount;
	cout<<"What is the spot level of your underlying?"<<endl;
	cin>>spot;
	cout<<"What is the lowest strike of your option?"<<endl;
	cin>>K1;
	cout<<"What is the highest strike of your option?"<<endl;
	cin>>K2;
	cout<<"What is the maturity in years of your option?"<<endl;
	cin>>T;
	if(useMarketData){
		Date d;
		d.setDateToToday ();
		rate=data.yieldcurve.spotRate(T);
		vol1=data.vols.volatility(K1,d.plusDays((Integer)(365*T)));
		vol2=data.vols.volatility(K2,d.plusDays((Integer)(365*T)));
	}
	else{
		cout<<"What is the rate to maturity you want to use? (in absolute value)"<<endl;
		cin>>rate;
		cout<<"At strike K_low, what is the volatility to maturity you want to use? (in absolute value)"<<endl;
		cin>>vol1;
		cout<<"At strike K_high, what is the volatility to maturity you want to use? (in absolute value)"<<endl;
		cin>>vol2;
	}
	cout<<"How many of these would you like to add?"<<endl;
	cin>>amount;
	strategy.addLongCallSpread(spot,vol1,true,vol2,true,rate,K1,K2,T,amount);
}

void inputPutSpread(marketData data,OptionStrategy& strategy,bool useMarketData){
	cout<<"\n**************************************"<<endl;
	cout<<"*** Add a Put Spread to a strategy ***"<<endl;
	cout<<"**************************************\n"<<endl;
	Real spot,K1,K2,T,rate,vol1,vol2,amount;
	cout<<"What is the spot level of your underlying?"<<endl;
	cin>>spot;
	cout<<"What is the lowest strike of your option?"<<endl;
	cin>>K1;
	cout<<"What is the highest strike of your option?"<<endl;
	cin>>K2;
	cout<<"What is the maturity in years of your option?"<<endl;
	cin>>T;
	if(useMarketData){
		Date d;
		d.setDateToToday ();
		rate=data.yieldcurve.spotRate(T);
		vol1=data.vols.volatility(K1,d.plusDays((Integer)(365*T)));
		vol2=data.vols.volatility(K2,d.plusDays((Integer)(365*T)));
	}
	else{
		cout<<"What is the rate to maturity you want to use? (in absolute value)"<<endl;
		cin>>rate;
		cout<<"At strike K_low, what is the volatility to maturity you want to use? (in absolute value)"<<endl;
		cin>>vol1;
		cout<<"At strike K_high, what is the volatility to maturity you want to use? (in absolute value)"<<endl;
		cin>>vol2;
	}
	cout<<"How many of these would you like to add?"<<endl;
	cin>>amount;
	strategy.addLongPutSpread(spot,vol1,true,vol2,true,rate,K1,K2,T,amount);
}

void inputRatioCallSpread(marketData data,OptionStrategy& strategy,bool useMarketData){
	cout<<"\n**************************************"<<endl;
	cout<<" Add a Ratio CallSpread to a strategy "<<endl;
	cout<<"**************************************\n"<<endl;
	Real spot,K1,K2,T,rate,vol1,vol2,amount;
	cout<<"What is the spot level of your underlying?"<<endl;
	cin>>spot;
	cout<<"What is the lowest strike of your option?"<<endl;
	cin>>K1;
	cout<<"What is the highest strike of your option?"<<endl;
	cin>>K2;
	cout<<"What is the maturity in years of your option?"<<endl;
	cin>>T;
	if(useMarketData){
		Date d;
		d.setDateToToday ();
		rate=data.yieldcurve.spotRate(T);
		vol1=data.vols.volatility(K1,d.plusDays((Integer)(365*T)));
		vol2=data.vols.volatility(K2,d.plusDays((Integer)(365*T)));
	}
	else{
		cout<<"What is the rate to maturity you want to use? (in absolute value)"<<endl;
		cin>>rate;
		cout<<"At strike K_low, what is the volatility to maturity you want to use? (in absolute value)"<<endl;
		cin>>vol1;
		cout<<"At strike K_high, what is the volatility to maturity you want to use? (in absolute value)"<<endl;
		cin>>vol2;
	}
	cout<<"How many of these would you like to add?"<<endl;
	cin>>amount;
	strategy.addLongRatioCallSpread(spot,vol1,true,vol2,true,rate,K1,K2,T,amount);
}

void inputStraddle(marketData data,OptionStrategy& strategy,bool useMarketData){
	cout<<"\n**************************************"<<endl;
	cout<<"**** Add a Straddle to a strategy ****"<<endl;
	cout<<"**************************************\n"<<endl;
	Real spot,K1,T,rate,vol1,amount;
	cout<<"What is the spot level of your underlying?"<<endl;
	cin>>spot;
	cout<<"What is the strike of your option?"<<endl;
	cin>>K1;
	cout<<"What is the maturity in years of your option?"<<endl;
	cin>>T;
	if(useMarketData){
		Date d;
		d.setDateToToday ();
		rate=data.yieldcurve.spotRate(T);
		vol1=data.vols.volatility(K1,d.plusDays((Integer)(365*T)));
	}
	else{
		cout<<"What is the rate to maturity you want to use? (in absolute value)"<<endl;
		cin>>rate;
		cout<<"What is the volatility to maturity you want to use? (in absolute value)"<<endl;
		cin>>vol1;
	}
	cout<<"How many of these would you like to add?"<<endl;
	cin>>amount;
	strategy.addLongStraddle(spot,vol1,true,rate,K1,T,amount);
}

void inputStrangle(marketData data,OptionStrategy& strategy,bool useMarketData){
	cout<<"\n**************************************"<<endl;
	cout<<"**** Add a Strangle to a strategy ****"<<endl;
	cout<<"**************************************\n"<<endl;
	Real spot,K1,K2,T,rate,vol1,vol2,amount;
	cout<<"What is the spot level of your underlying?"<<endl;
	cin>>spot;
	cout<<"What is the lowest strike of your option?"<<endl;
	cin>>K1;
	cout<<"What is the highest strike of your option?"<<endl;
	cin>>K2;
	cout<<"What is the maturity in years of your option?"<<endl;
	cin>>T;
	if(useMarketData){
		Date d;
		d.setDateToToday ();
		rate=data.yieldcurve.spotRate(T);
		vol1=data.vols.volatility(K1,d.plusDays((Integer)(365*T)));
		vol2=data.vols.volatility(K2,d.plusDays((Integer)(365*T)));
	}
	else{
		cout<<"What is the rate to maturity you want to use? (in absolute value)"<<endl;
		cin>>rate;
		cout<<"At strike K_low, what is the volatility to maturity you want to use? (in absolute value)"<<endl;
		cin>>vol1;
		cout<<"At strike K_high, what is the volatility to maturity you want to use? (in absolute value)"<<endl;
		cin>>vol2;
	}
	cout<<"How many of these would you like to add?"<<endl;
	cin>>amount;
	strategy.addLongStrangle(spot,vol1,true,vol2,true,rate,K1,K2,T,amount);
}


Exotics* inputExoticOptionOnSingleAsset(marketData& data){
	bool import,flatData;
	Real spot,K,T,vol,rate,K2;
	LongNatural asianDates;
	yieldCurve* yc;
	volsurface* vs;

	cout<<"\n**************************************"<<endl;
	cout<<"**** Input an Exotic option (M-C) ****"<<endl;
	cout<<"**************************************\n"<<endl;

	cout<<"\nWhat is the spot level of your underlying?"<<endl;
	cin>>spot;
	cout<<"What is the maturity in years of your option?"<<endl;
	cin>>T;

	cout<<"These options are often path dependant: would you like to re-import some data?\nType 1 to re-import, else any other key"<<endl;
	cin>>import;
	if(import){
		importData tempImport;
		tempImport.runUserDefinedInterface();
		data=tempImport.getData ();
	}
	else{
		cout<<"The data was not changed, would you like to input flat volatility and yield curves?\nType 1 if yes, else the program will use the data imported earlier."<<endl;
		cin>>flatData;
		if(flatData){
			cout<<"What is the flat rate you want to use? (in absolute value)"<<endl;
			cin>>rate;
			cout<<"What is the flat volatility you want to use? (in absolute value)"<<endl;
			cin>>vol;
		}
	}
	Natural choice;
	bool incorrectChoice=true;
	exoticsType type;
	while(incorrectChoice){
		cout<<"\nWhat sort of option do you want to add? Press:"<<endl;
		cout<<"1 - for an AsianCall (computation can be long)"<<endl;
		cout<<"2 - for a AsianPut (computation can be long)"<<endl;
		cout<<"3 - for a RevLookbackCall"<<endl;
		cout<<"4 - for a RevLookbackPut"<<endl;
		cout<<"5 - for a FlooredCliquet"<<endl;
		cout<<"6 - for a CappedCliquet"<<endl;
		cout<<"7 - for a CollaredCliquet"<<endl;
		cout<<"8 - for a BarrierCall"<<endl;
		cout<<"9 - for a BarrierPut"<<endl;
		cin>>choice;
		incorrectChoice=((choice<=0)||(choice>9));
		if(incorrectChoice)
			cout<<"Your entry did not seem valid"<<endl;
	}
	type=choiceToType(choice);
	if (type==CollaredCliquet){ //collared are capped and floored
		cout<<"What is the first strike?"<<endl;
		cin>>K;
		cout<<"What is the second strike?"<<endl;
		cin>>K2;
	}
	else{
		cout<<"What is the strike of the option?"<<endl;
		cin>>K;	
	}
	cout<<"The Monte-Carlo Price on several dates can take several minutes...\nHow many dates should be looked at in the payoff?"<<endl;
	cin>>asianDates;
	asianDates=max(asianDates,(LongNatural)1);

	Exotics * res;
	if(flatData){
		yc=new yieldCurve(rate);
		vs=new volsurface(vol);
		res = new Exotics(type,yc,vs,spot,K,T,asianDates,K2);
	}
	else{
		yc=new yieldCurve(data.yieldcurve);
		vs=new volsurface(data.vols);
		res = new Exotics(type,yc,vs,spot,K,T,asianDates,K2);
	}
	
	cout<<"Its price is: ... being computed ...\n"<<res->getPrice()<<endl;
	bool replay=true;
	while (replay){
		cout<<"Would you like to see its greeks? Given the time it took to get a price, up to you!\nPress the corresponding number to see it, else another key"<<endl;
		cout<<"1 - Delta "<<endl;
		cout<<"2 - Vega"<<endl;
		cout<<"3 - Theta"<<endl;
		cout<<"4 - Rho"<<"\n"<<endl;
		cin>>choice;
		switch (choice){
			case 1:
				cout<<"Delta is "<<res->getDelta()<<endl;
				break;
			case 2:
				cout<<"Vega is "<<res->getVega()<<endl;
				break;
			case 3:
				cout<<"Theta is "<<res->getTheta()<<endl;
				break;
			case 4:
				cout<<"Rho is "<<res->getRho()<<endl;
				break;
			default:
				replay=false;
				break;
		}
	}
	return res;
}

convertiblebond * inputConvertibleBond(marketData& data) 
{
	cout<<"*****************************"<<endl;
	cout<<"*** Input one convertible ***"<<endl;
	cout<<"*****************************\n"<<endl;

	cout<<"First create an underlying risky bond"<<endl;

	// maturity in years
	Real theMaturity;
	cout << "\nWhat is the maturity of the bond in years? e.g. 20"<<endl;
	cin >> theMaturity;

	// face amount
	Real theFace;
	cout << "\nWhat is the  face amount? e.g. 1000"<<endl;
	cin >> theFace;

	// yield curve
	yieldCurve yc(data.yieldcurve);
	Natural yield;
	bool testyield=true;
	while (testyield){
		cout << "Would you like to use market data for the yield curve? "<<endl;
		cout << " 1- yes"<<endl;
		cout << " 2- no"<<endl;
		cin >> yield;
		if (yield==1)
			testyield=false;
		else if (yield==2){
			Real rate;
			cout << "What is the flat rate (in percentage) you want to use? e.g. 5"<<endl;
			cin >> rate;
			yc = yieldCurve(rate/100);
			testyield=false;
		}
	}

	// credit curve
	creditCurve cc(data.creditcurve);
	Natural credit;
	bool testcredit=true;
	while (testcredit){
		cout << "Would you like to use market data for the credit curve? "<<endl;
		cout << " 1- yes"<<endl;
		cout << " 2- no"<<endl;
		cin >> credit;
		if (credit==1)
			testcredit=false;
		else if (credit==2){
			Real spread;
			cout << "What is the credit spread (in percentage) you want to use? e.g. 2"<<endl;
			cin >> spread;
			spread/=100;
			cc = creditCurve(yc, spread);
			testcredit=false;
		}
	}

	// asset current price
	Real thePrice;
	cout << "What is the spot level of your underlying? e.g. 2.23" <<endl;
	cin >> thePrice;

	// asset volatility
	Real theVol;
	cout << "What is the volatility of the underlying - e.g. 0.44" << endl;
	cin >> theVol;

	// conversion ratio
	Real theConversionRatio;
	cout << "What is the conversion ratio for the face amount - e.g. 55.932" << endl;
	cin >> theConversionRatio;

	// call price
	Real theCallPrice;
	cout << "What is the call price for the bond - input 9999 if not callable" << endl;
	cin >> theCallPrice;

	// put price
	Real thePutPrice;
	cout << "What is the put price for the bond - input 0 if not puttable" << endl;
	cin >> thePutPrice;

	// number of steps
	Natural theSteps;
	cout << "How many steps to use in binomial tree - e.g. 10" << endl;
	cin >> theSteps;

	Date today;
	today.setDateToToday();
	Date todayPlusMaturity = today.plusDays((Integer)(365 * theMaturity));

	asset *theStock =
		new asset(thePrice, theVol);
	riskybond *theRiskyBond =
		new riskybond(today, todayPlusMaturity, theFace, ACT_365, yc, cc);
	convertiblebond *theConvertible =
		new convertiblebond(*theStock, *theRiskyBond, theConversionRatio, theSteps, theCallPrice, thePutPrice);

	cout << "characteristics of this convertible:" << endl
		<< theConvertible << endl;

	return theConvertible;
}

bond * inputBond(marketData& data)
{
	Natural typeofbond;
	cout<<"**********************"<<endl;
	cout<<"*** Input one bond ***"<<endl;
	cout<<"***********************\n"<<endl;
	bool test=true;
	while(test){
		cout << " 1- To create a treasury bond"<<endl;
		cout << " 2- To create a risky bond"<<endl;
		cin>>typeofbond;
		if((typeofbond==1)||(typeofbond==2))
			test = false;
	}

	Real dateofirstcoupon;
	cout << "\nWhat is the time of the first coupon in years? "<<endl;
	cin >> dateofirstcoupon;


	Real dateofmaturity;
	cout << "\nWhat is the maturity of the bond in years? "<<endl;
	cin >> dateofmaturity;

	Real coupon;
	cout << "\nWhat is the yearly coupon (in percentage of the faceamount)? "<<endl;
	cin >> coupon;
	coupon /= 100;

	Natural freq;
	bool testfreq=true;
	Frequency fr;
	while(testfreq){
		cout << "\nWhat is the frequency of coupon deliveries? "<<endl;
		cout << " 0- No frequency"<<endl;
		cout << " 1- Once (ZeroCoupon)"<<endl;
		cout << " 2- Annual"<<endl;
		cout << " 3- Semiannual"<<endl;
		cout << " 4- Every Four Months"<<endl;
		cout << " 5- Quaterly"<<endl;
		cout << " 6- Bimonthly"<<endl;
		cout << " 7- Monthly"<<endl;
		cin >> freq;

		switch (freq) {
			case 0:
				fr = NoFrequency;
				testfreq = false;
				break;
			case 1:
				fr = Once;
				testfreq = false;
				break;
			case 2:
				fr = Annual;
				testfreq = false;
				break;
			case 3:
				fr = Semiannual;
				testfreq = false;
				break;
			case 4:
				fr = EveryFourthMonth;
				testfreq = false;
				break;
			case 5:
				fr = Quarterly;
				testfreq = false;
				break;
			case 6:
				fr = Bimonthly;
				testfreq = false;
				break;
			case 7:
				fr = Monthly;
				testfreq = false;
				break;
			default:
				break;
		}
	}
	
	Real faceamount;
	cout << "\nWhat is the  face amount? "<<endl;
	cin >> faceamount;
	

	Natural dcount;
	bool testdcount=true;
	DayCountConvention daycount;
	while(testdcount){
		cout << "\nWhat is the Day Count Convention? "<<endl;
		cout << " 0- ACT/365"<<endl;
		cout << " 1- ACT/360"<<endl;
		cout << " 2- Day30/365"<<endl;
		cout << " 3- Day30/360"<<endl;
		cin >> dcount;

		switch (dcount) {
			case 0:
				daycount = ACT_365;
				testdcount = false;
				break;
			case 1:
				daycount = ACT_360;
				testdcount = false;
				break;
			case 2:
				daycount = Day30_365;
				testdcount = false;
				break;
			case 3:
				daycount = Day30_360;
				testdcount = false;
				break;
			default:
				break;
		}
	}

	Date today, maturity, firstcoupondate;
	today.setDateToToday ();
	firstcoupondate=today.plusDays((Integer)(365*dateofirstcoupon)); 
	maturity=today.plusDays((Integer)(365*dateofmaturity)); 

	bond* res;

	yieldCurve yc(data.yieldcurve);
	Natural yield;
	bool testyield=true;
	while (testyield){
		cout << "Would you like to use market data for the yield curve? "<<endl;
		cout << " 1- yes"<<endl;
		cout << " 2- no"<<endl;
		cin >> yield;
		if (yield==1)
			testyield=false;
		else if (yield==2){
			Real rate;
			cout << "What is the flat rate (in percentage) you want to use? "<<endl;
			cin >> rate;
			yc = yieldCurve(rate/100);
			testyield=false;
		}
	}
	
	creditCurve cc(data.creditcurve);
	if(typeofbond==2){		
		Natural credit;
		bool testcredit=true;
		while (testcredit){
			cout << "Would you like to use market data for the credit curve? "<<endl;
			cout << " 1- yes"<<endl;
			cout << " 2- no"<<endl;
			cin >> credit;
			if (credit==1)
				testcredit=false;
			else if (credit==2){
				Real spread;
				cout << "What is the credit spread (in percentage) you want to use? "<<endl;
				cin >> spread;
				spread/=100;
				cc = creditCurve(yc, spread);
				testcredit=false;
			}
		}
	}


	switch (typeofbond){
		case 1:
			res = new treasurybond(today, maturity, firstcoupondate, coupon, fr, faceamount, daycount, yc);
			break;
		case 2:
			res = new riskybond(today, maturity, firstcoupondate, coupon, fr, faceamount, daycount, yc, cc);
			break;
		default:
			break;
	}

	cout <<"*** characteristic of your bond ***"<<endl;
	cout << "fairvalue  " << res->fairvalue()  << endl;
	cout << "yield to maturity " << res->yieldToMaturity()  << endl;
	cout << "convexity  " << res->convexity()  << endl;
	cout << "duration  " << res->duration()  << endl;
	cout << endl;


	return res;

}


exoticsType choiceToType(Natural choice){
	exoticsType type;
	switch (choice){
		case 1:
			type=AsianCall;
			break;
		case 2:
			type=AsianPut;
			break;
		case 3:
			type=RevLookbackCall ;
			break;
		case 4:
			type=RevLookbackPut;
			break;
		case 5:
			type=FlooredCliquet;
			break;
		case 6:
			type=CappedCliquet;
			break;
		case 7:
			type=CollaredCliquet;
			break;
		case 8:
			type=BarrierCall;
			break;
		case 9:
			type=BarrierPut ;
			break;
	}
	return type;
}

VanillaSwap* inputVanillaSwap(marketData data) {
	VanillaSwap* res;
	Date startDate=Date();
	Date endDate=Date();
	Real frequency,notional,amortizing,maturity,fixedRate;
	Natural payOrReceive,inputTodayDate,yield;
	bool payFixed,useTodayStart;

	cout<<"******************************"<<endl;
	cout<<"*** Input one vanilla swap ***"<<endl;
	cout<<"******************************\n"<<endl;

	cout<<"What is the notional of your contract?"<<endl;
	cin>>notional;
	cout<<"What is the amortizing size of your contract (0 if not)?"<<endl;
	cin>>amortizing;
	cout<<"What is the maturity of the contract in years (you can enter a floating number)?"<<endl;
	cin>>maturity;
	cout<<"What is the frequency of the contract (floating number, enter 2 for semi annual for example)?"<<endl;
	cin>>frequency;
	cout<<"What is the fixed rate of your contract?"<<endl;
	cin>>fixedRate;
	cout<<"Enter 1 if you receive the fixed rate or anything else if you pay it."<<endl;
	cin>>payOrReceive;
	payFixed=(payOrReceive!=1);
	cout<<"Enter 1 if you want to use today's date as start date or anything else if you want to enter it."<<endl;
	cin>>inputTodayDate;
	useTodayStart=(inputTodayDate==1);
	if (useTodayStart)
		startDate.setDateToToday();
	else {
		Natural daystart,monthstart,yearstart;
		cout<<"Enter start month as a number between 1 and 12 :"<<endl;
		cin>>monthstart;
		cout<<"Enter start day as a number between 1 and 31 :"<<endl;
		cin>>daystart;
		cout<<"Enter start year as a number between 1900 and 2100 :"<<endl;
		cin>>yearstart;
		startDate=Date(daystart,monthstart,yearstart);
	}

	endDate=startDate.plusDays((Integer)(365.25*maturity));

	yieldCurve* yc=new yieldCurve();
	bool testyield=true;
	while (testyield){
		cout << "Would you like to use market data for the yield curve? "<<endl;
		cout << " 1- yes"<<endl;
		cout << " 2- no"<<endl;
		cin >> yield;
		if (yield==1) {
			testyield=false;
			yc=&(data.yieldcurve);
		}
		else if (yield==2){
			Real rate;
			cout << "What is the flat rate you want to use (enter 0.05 for 5 percent)? "<<endl;
			cin >> rate;
			yc = new yieldCurve(rate);
			testyield=false;
		}
	}


	//Create cashflows
	SwapLeg swapLegReceived=SwapLeg(startDate,frequency,endDate,notional,amortizing,Following);
	SwapLeg swapLegPaid=SwapLeg(startDate,frequency,endDate,notional,amortizing,Following);
	CashFlow cashFlowFloat=CashFlow(swapLegReceived,data.yieldcurve);
	CashFlow cashFlowFixed=CashFlow(swapLegReceived,fixedRate);

	char* name1=new char[MAX_LETTERS];
	char* name2=new char[MAX_LETTERS];
	cout<<"Enter name of counterpart 1 :"<<endl;
	cin>>name1;
	cout<<"Enter name of counterpart 2 :"<<endl;
	cin>>name2;
	if (payFixed)
		res=new VanillaSwap(cashFlowFloat,cashFlowFixed,name1,name2,yc);
	else
		res=new VanillaSwap(cashFlowFixed,cashFlowFloat,name1,name2,yc);

	cout<<"\nThe value of the first leg is : "<<res->getFairValue1()<<endl;
	cout<<"\nThe value of the second leg is : "<<res->getFairValue2()<<endl;
	cout<<"\nThe price of your swap is then : "<<res->returnPrice()<<endl;
	cout<<"\nThe sensibility of your swap to interest rate is : "<<res->getRho()<<endl;
	cout<<"\nThe sensibility of your swap to time is : "<<res->getTheta()<<endl;
	cout<<"\n"<<endl;
	return res;
}


rainbowType chooseRainbowType(){
	rainbowType type;
	Natural choice;
	bool testInput=true;
	while(testInput){
		cout<<"The available Rainbow OptionTypes are as follows, which one do you want?"<<endl;
		cout<<"1 - SpreadOptionMax"<<endl;
		cout<<"2 - AssetsBasketMax"<<endl;
		cout<<"3 - BestOf2AssetsCash"<<endl;
		cout<<"4 - WorstOf2AssetsCash"<<endl;
		cout<<"5 - BetterOf2Assets"<<endl;
		cout<<"6 - WorseOf2Assets"<<endl;
		cout<<"7 - Max2AssetsCall"<<endl;
		cout<<"8 - Min2AssetsCall"<<endl;
		cout<<"9 - Max2AssetsPut"<<endl;
		cout<<"10 - Min2AssetsPut"<<endl;
		cin>>choice;
		testInput=(choice<=0)||(choice>10);		
	}
	switch (choice){
		case 1:
			type=SpreadOptionMax;
			break;
		case 2:
			type=AssetsBasketMax;
			break;
		case 3:
			type=BestOf2AssetsCash;
			break;
		case 4:
			type=WorstOf2AssetsCash;
			break;
		case 5:
			type=BetterOf2Assets;
			break;
		case 6:
			type=WorseOf2Assets;
			break;
		case 7:
			type=Max2AssetsCall;
			break;
		case 8:
			type=Min2AssetsCall;
			break;
		case 9:
			type=Max2AssetsPut;
			break;
		case 10:
			type=Min2AssetsPut;
			break;
		default:
			type=BestOf2AssetsCash;
			break;
	}
	return type;
}

priceType choosePricingType(){
	priceType type;
	Natural choice;
	bool testInput=true;
	while(testInput){
		cout<<"Which pricing method do you want? Note that obviously MC is slow, but for Spread/Asset/WOCash it is the only way"<<endl;
		cout<<"1 - Monte Carlo"<<endl;
		cout<<"2 - Closed Form"<<endl;	
		cin>>choice;
		testInput=testInput=(choice!=1)&&(choice!=2);	
	}
	switch (choice){
		case 1:
			type=MonteCarlo;
			break;
		case 2:
			type=ClosedForm;
			break;
		default:
			type=ClosedForm;
			break;			
	}
	return type;
}

RainbowOption* inputRainbowOption(marketData data){
	cout<<"\n*********************************"<<endl;
	cout<<"*** Input one Rainbow option ***"<<endl;
	cout<<"********************************\n"<<endl;
	
	cout<<"What type of option would you like to add? \n[Only 2 assets type available in the menu -- see code]"<<endl;
	rainbowType type=chooseRainbowType();
	
	Date d;
	d.setDateToToday ();
	Real exp=0.0;
	while(exp<=0.0){
		cout<<"What is the maturity of your option in years?"<<endl;
		cin>>exp;
	}
	exp=max(exp,0.01);
	
	bool testData=true;
	Natural choice;
	cout<<"Do you want to use the market data (both vol curves will be indexed on SP at 2994...? Type 1 for Yes, else something else."<<endl;
	cin>>choice;
	
	yieldCurve yc=yieldCurve(data.yieldcurve);
	volsurface vs1=volsurface(data.vols);
	volsurface vs2=volsurface(data.vols);
	if(choice!=1){
		Real rate,vol1,vol2;
		cout<<"What flat rate do you want to use? In absolute value"<<endl;
		cin>>rate;
		yc=yieldCurve(max(rate,0));
		cout<<"What 1st flat vol do you want to use? In absolute value"<<endl;
		cin>>vol1;
		vs1=volsurface(max(vol1,0));	
		cout<<"What 2nd flat vol do you want to use? In absolute value"<<endl;
		cin>>vol2;
		vs1=volsurface(max(vol2,0));
	}
	valarray<volsurface> vols(2);
	vols[0]=vs1;
	vols[1]=vs2;
//	RainbowOption(rainbowType type,Date start,Real exp,Real Strike,yieldCurve yc,valarray<volsurface> vols,Real Spot1,Real Spot2,Real Mult=RO_DEFAULT_MULTIPLIER,Real Correl12=0,Real weight1=0.5,Real weight2=0.5,bool outputMsgs=false);
	Real Strike,spot1,spot2,weight1,weight2;
	Real rho=-2,mult=1;
	cout<<"What is the strike of your option?"<<endl;
	cin>>Strike;
	cout<<"What is the spot of your 1st underlying?"<<endl;
	cin>>spot1;
	cout<<"What is the spot of your 2nd underlying?"<<endl;
	cin>>spot2;
	while (rho>1 ||rho<-1){
		cout<<"What is the correlation between the assets?"<<endl;
		cin>>rho;
	}
	cout<<"What is the weight you want to assign to your 1st underlying?"<<endl;
	cin>>weight1;
	cout<<"What is the weight you want to assign to your 2nd underlying?"<<endl;
	cin>>weight2;	

	RainbowOption * rb=new RainbowOption(type,d,exp,Strike,yc,vols,spot1,spot2,mult,rho,weight1,weight2,false);
	
	priceType pType=choosePricingType();


	cout<<"Its price is: "<<rb->getPrice(pType)<<endl;

	cout<<"Would you like to see its greeks? Type 1 if yes, else something else.\n[Note: If you chose Monte Carlo, it will take a few seconds]"<<endl;
	cin>>choice;
	if(choice==1){
		cout<<"Delta 1: "<<rb->getPartialDelta(1,pType)<<endl;
		cout<<"Delta 2: "<<rb->getPartialDelta(2,pType)<<endl;
		cout<<"Gamma 1: "<<rb->getPartialGamma(1,pType)<<endl;
		cout<<"Gamma 2: "<<rb->getPartialGamma(2,pType)<<endl;
		cout<<"Vega 1: "<<rb->getPartialVega(1,pType)<<endl;
		cout<<"Vega 2: "<<rb->getPartialVega(2,pType)<<endl;
		cout<<"Correl risk : "<<rb->getCorrelRisk(pType)<<endl;
		cout<<"Rho : "<<rb->getRho(pType)<<endl;
	}
	cout<<"\n"<<endl;

	return rb;
}
