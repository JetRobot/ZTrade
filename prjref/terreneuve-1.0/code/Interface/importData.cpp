#include ".\importData.h"

importData::importData(void)
{
}

importData::importData(char *argv)
{
	FileReader::setdatadir (argv); // memorize the local path if the user wants the predefined data
	runInterface();
}

void importData::importYieldCurve(string path){
	string yieldCurveData = _datadir + "/"+path;

	valarray<yieldPoint> *rates = 
		FileReader::buildYieldPointArray(yieldCurveData);

	_yc =yieldCurve(*rates,"test yc");
	cout<<"- Yield curve imported"<<endl;
}

void importData::importVolSurface(string path,Real spot)
{
	Date t0=Date();
	t0.setDateToToday ();
	string volSurfaceData=_datadir+"/"+path;
	volsurfaceparams *params =
		FileReader::buildVolSurfaceParams(volSurfaceData);

	_vs= volsurface(spot, t0, _yc, *params);
	_vs.setvolsurface();
	cout<<"- Volatility surface imported"<<endl;
}

void importData::importCreditCurve(string path){
	string creditSpreadsData= _datadir+"/"+path;
	valarray<CreditSpreadPoint> *creditSpreadArray =
		FileReader::buildCreditSpreadPointArray(creditSpreadsData);
	_cc=creditCurve(_yc,*creditSpreadArray,"test cc");
	cout<<"- Credit spreads imported"<<endl;
}


bool importData::runInterface(){
	cout<<"\n**************************************"<<endl;
	cout<<"********* Data import Module *********"<<endl;
	cout<<"**************************************\n"<<endl;
	cout<<"As it is a financial products module, you first need to import market data."<<endl;
	cout<<"Would you like to import your own data or use the stale one provided ? "<<endl;
	cout<<"[The yield points come from the US swap market from Oct 5th, 2005]"<<endl;
	cout<<"[The options are on -- (spot 2994) from July 16th, 2005]"<<endl;
	cout<<"[The credit spreads are on -- from -- ]"<<endl;
	cout<<"Type 1 to input your data files location, 2 to see the files format,\n Else another key to get default inputs:"<<endl;
	string userWantsToImport;
	cin>>userWantsToImport;
	Natural importNAt=atoi(userWantsToImport.c_str());
	if(importNAt==2)
		importNAt=displayFileFormatsMenu();
	cout<<((importNAt==1) ? "\nYou will now choose your data:" : "\nThe program will get its own data:" )<<endl;
	if(importNAt!=1){
		_datadir=FileReader::getdatadirasstring();
		importYieldCurve();
		importVolSurface();
		importCreditCurve();
		cout<<"--> Default data imported\n"<<endl;
		setMarketData();
		return true;
	}
	else{
		return runUserDefinedInterface();
	}
}
void importData::setMarketData(){
	_marketData.yieldcurve=_yc;
	_marketData.vols=_vs;
	_marketData.creditcurve=_cc;
}

bool importData::runUserDefinedInterface(){
	bool failure=true;
	bool input;
	bool yc,vs,cs;
	string dataDir;
	cout<<"We need a spot for the call/put prices to calibrate the volatility surface, please input one"<<endl;
	Real spot;
	cin>>spot;

	while (failure){
		cout<<"\nNOTE:\n- Your own data should be in the SAME directory "<<endl;
		cout<<"- The yield curve name shoud be "<<YCNAME<<endl;
		cout<<"- The volatility suface name shoud be "<<VSNAME<<endl;
		cout<<"- The credit spreads name shoud be "<<CSNAME<<endl;
		cout<<"Please input the path where your data is located, ALL 3 files"<<endl;
		cin>>dataDir;
		cout<<"\n"<<endl;
		yc=FileReader::fileexists(dataDir+"/"+YCNAME);
		vs=FileReader::fileexists(dataDir+"/"+VSNAME);
		cs=FileReader::fileexists(dataDir+"/"+CSNAME);
		failure=!(yc&&vs&&cs); //if all are found, then failure will be false
		if(failure){
			if(!yc)
				cout<<"- "<<YCNAME<<" not found"<<endl;
			if(!vs)
				cout<<"- "<<VSNAME<<" not found"<<endl;
			if(!cs)
				cout<<"- "<<CSNAME<<" not found"<<endl;
			cout<<"Some data are not located where you speficied - do you want to re-input ?\nType 1 if yes, else another key "<<endl;
			cin>>input;
			if(!input){
				cout<<"Default import launched:"<<endl;
				_datadir=FileReader::getdatadirasstring();
				importYieldCurve();
				importVolSurface();
				importCreditCurve();
				cout<<"--> Default data imported\n"<<endl;
				setMarketData();
				return true;
			}
		}
	}
	_datadir=dataDir;
	importYieldCurve(YCNAME);
	importVolSurface(VSNAME,spot);
	importCreditCurve(CSNAME);
	cout<<"--> Personal data imported\n"<<endl;
	setMarketData();
	return true;
}


Natural importData::displayFileFormatsMenu(){
	cout<<"\nHELP ON FILE FORMATS\n"<<endl;
	cout<<"YIELD CURVE\nA csv file as follows, with on each line a rate,a maturity in years,and a rate type (Cash or Swap)."<<endl;
	cout<<"For example a possible first line could be:\n"<<endl;
	cout<<"0.041,0.25,Cash     <-- Meaning the Cash rate of maturity 3 months is 4.1 percent\n\n"<<endl;

	cout<<"VOLATILITIES\nAs csv file as follows:"<<endl;
	cout<<"- The first 2 lines specify the number of maturities quoted and the number of strikes at these:\n"<<endl;
	cout<<"maturities            ,    1\nstrikes               ,    2\n"<<endl;
	cout<<"- Then a matrix of which the first column has the maturities in DD-MM-YYYY, "<<endl;
	cout<<"and then alternatively prices at the given strike and the option type (c/p). For example:\n"<<endl;
	cout<<"maturities / strikes  , 2395.95  , type, 2545.69  , type"<<endl;
	cout<<"17-3-2006             , 693.1431 ,  c  , 561.4132 ,  c\n"<<endl;
	cout<<"<-- Meaning that for maturity 17-3-2006, the 2395.95 call is quoted 693.1431, and the 2545.69 is 561.4132\n\n"<<endl;

	cout<<"CREDIT SPREADS\nA csv file as follows, with on each line a spread,a maturity in years,and a spread type (Abs or Rel)."<<endl;
	cout<<"For example a possible first line could be:\n"<<endl;
	cout<<"0.0014,2,Rel     <-- Meaning the realtive credit spread rate of maturity 2 years is 14 bps\n\n"<<endl;

	cout<<"Now that you know, would you like to import your own data or use the stale one provided ? "<<endl;
	cout<<"Type 1 to input your data files location, else another key to get the default inputs:"<<endl;
	Natural userWantsToImport;
	cin>>userWantsToImport;
	return userWantsToImport;
}

