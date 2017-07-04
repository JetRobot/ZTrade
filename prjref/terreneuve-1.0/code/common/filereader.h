#pragma once
#ifndef filereader_h
#define filereader_h

#include <string>
#include "..\PartB\yieldCurve.h"
#include "..\PartF\creditCurve.h"
#include "..\PartE\volsurface.h"

using namespace std;

class FileReader {
public:
	static bool fileexists(const char *filename);
	static bool fileexists(string filename);
	static bool setdatadir(const char* command);
	static char *getdatadir(void) { return &_datadir[0]; };
	static string getdatadirasstring(void) { return string(_datadir); };
	static valarray<yieldPoint> *buildYieldPointArray(string filename);
	static valarray<CreditSpreadPoint> *buildCreditSpreadPointArray(string filename);
	static volsurfaceparams *buildVolSurfaceParams(string filename);
protected:
	static char _datadir[128];
};

#endif