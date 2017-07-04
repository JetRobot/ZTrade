#pragma once
#ifndef importData_h
#define importData_h


#include <iostream>
#include <string>
#include "..\PartF\creditCurve.h"
#include "..\common\filereader.h"
#include "..\common\date.h"
#include"..\PartB\yieldCurve.h"
#include"..\PartE\volsurface.h"

/**
@author Yann
*/

static const string YCNAME ="yieldcurve.csv";
static const string VSNAME ="volsurface.csv";
static const string CSNAME ="creditspreads.csv";

struct marketData {
	yieldCurve yieldcurve;
	creditCurve creditcurve;
	volsurface vols;
};

class importData{
private:
	string _datadir;
	yieldCurve _yc;
	creditCurve _cc;
	volsurface _vs;
	marketData _marketData;
	void setMarketData();
public:
	/**constructor with a reference to local files - need for default data import*/
	importData(void);
	/**constructor with a reference to local files - need for default data import*/
	importData(char *argv);
	/**default user interface for data import*/
	bool runInterface();
	/**Help output on the file format needed to import*/
	Natural displayFileFormatsMenu(); 
	/**User interface to input the data files*/
	bool runUserDefinedInterface();

	/**Import the yield curve from the file*/
	void importYieldCurve(string path="yctest.csv");
	/**Import the call/put points from the file*/
	void importVolSurface(string path="voltest2.csv",Real spot=2994.0);
	/**Import the credit spreads from the file*/
	void importCreditCurve(string path="ccspread.csv");

	marketData getData() {return _marketData;}
	yieldCurve getYieldCurve()			{return _yc;}
	creditCurve getCreditCurve()		{return _cc;}
	volsurface getVolatilitySurface()	{return _vs;}
};

#endif