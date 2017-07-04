#include <fstream>
#include "filereader.h"
#include "csvparser.h"
using namespace std;

char FileReader::_datadir[128] = "";

/** test for file existence
	@param filename - file to test
	@returns true if file exists, otherwise false
	*/
bool FileReader::fileexists(const char *filename) {
  bool flag = false;
  fstream fin;

  fin.open(filename, ios::in);
  flag = fin.is_open();
  fin.close();

  return flag;
}

/** test for file existence - string version
	@param filename - file to test
	@returns true if file exists, otherwise false
	*/
bool FileReader::fileexists(string filename) {
	return FileReader::fileexists(filename.c_str());
}

bool FileReader::setdatadir(const char *command) {
	/** since there is no nice platform independent way to check
		for directory existence and I don't want to put in platform
		dependent defines, here we look for a file in the data
		directory "sample.csv", if we can find it then we have found
		the data directory */
	bool result = false;
	string mycommand(command);
	string filename("sample.csv");
	string dir("");
	string datadir("");
	string fullfilename("");

	/* possible values for command:
	   program.exe -> datadir: ..\data
	   somepath\program.exe -> datadir: somepath\..\data */

	string::size_type pos = mycommand.rfind("\\", mycommand.length());

	/* if \ found in command, extract the dir including the \, if not found
	   we'll just continue with dir = "" */
	if (pos != string::npos) {
		dir = mycommand.substr(0, pos + 1);
	}
	
	datadir = dir + "..\\data\\";
	fullfilename = datadir + filename;
	if (FileReader::fileexists(fullfilename.c_str())) {
		strcpy(FileReader::_datadir, datadir.c_str());
		result = true;
	} else {
		datadir = dir + "data\\";
		fullfilename = datadir + filename;
		if (FileReader::fileexists(fullfilename.c_str())) {
			strcpy(FileReader::_datadir, datadir.c_str());
			result = true;
		}
	}

	return result;
}

/** Read data from a file to create an array of yield points which can be 
	used to construct a yield curve object.  
	@param filename - source of data
	@returns valarray of yieldPoints or null pointer if file not found
	*/
valarray<yieldPoint> *
FileReader::buildYieldPointArray(string filename) {
  string sLine;
  double yieldRate, yieldMaturity;
  TypeOfRate yieldType;
  
  Natural ypArrayIdx = 0;
  Natural ypArrayMaxIdx = YC_MAX_NUMBER_POINTS - 1;
  yieldPoint *ypArray[YC_MAX_NUMBER_POINTS];
  valarray<yieldPoint> *ypValArray;

  CSVParser parser;

  if (!FileReader::fileexists(filename)) {
	cout << "could not find file " << filename << endl;
	return NULL;
  }
    
  ifstream infile(filename.c_str());
  while (!infile.eof() && (ypArrayIdx < ypArrayMaxIdx)) {
	if (ypArrayIdx >= ypArrayMaxIdx) {
		cout << "File has more than max entries: " << (ypArrayMaxIdx + 1)
			 << " - truncating" << endl;
		break;
	}

    getline(infile, sLine); // Get a line
    if (sLine == "")
      continue;

    parser << sLine; // Feed the line to the parser

    // Now extract the columns from the line
    parser >> yieldRate >> yieldMaturity >> yieldType;

	ypArray[ypArrayIdx++] = 
		new yieldPoint(yieldRate, yieldMaturity, yieldType);
  }
  infile.close();

  ypValArray = new valarray<yieldPoint>(ypArrayIdx);
  for (Natural i = 0; i < ypArrayIdx; i++) {
	  (*ypValArray)[i] = *(ypArray[i]);
  }

  return ypValArray;
}

/** Read data from a file to create an array of credit spread points which 
	can be used to construct a credit curve object.  
	@param filename - source of data
	@returns valarray of CreditSpreadPoints or null pointer if file not found
	*/
valarray<CreditSpreadPoint> *
FileReader::buildCreditSpreadPointArray(string filename) {
  string sLine;
  double spreadRate, spreadMaturity;
  CreditSpreadType spreadType;
  
  Natural ArrayIdx = 0;
  Natural ArrayMaxIdx = CC_MAX_NUM_SPREADS - 1;
  CreditSpreadPoint *theArray[CC_MAX_NUM_SPREADS];
  valarray<CreditSpreadPoint> *theValarray;

  CSVParser parser;

  if (!FileReader::fileexists(filename)) {
	cout << "could not find file " << filename << endl;
	return NULL;
  }
    
  ifstream infile(filename.c_str());
  while (!infile.eof() && (ArrayIdx < ArrayMaxIdx)) {
	if (ArrayIdx >= ArrayMaxIdx) {
		cout << "File has more than max entries: " << (ArrayMaxIdx + 1)
			 << " - truncating" << endl;
		break;
	}

    getline(infile, sLine); // Get a line
    if (sLine == "")
      continue;

    parser << sLine; // Feed the line to the parser

    // Now extract the columns from the line
    parser >> spreadRate >> spreadMaturity >> spreadType;

	theArray[ArrayIdx++] = 
		new CreditSpreadPoint(spreadRate, spreadMaturity, spreadType);
  }
  infile.close();

  theValarray = new valarray<CreditSpreadPoint>(ArrayIdx);
  for (Natural i = 0; i < ArrayIdx; i++) {
	  (*theValarray)[i] = *(theArray[i]);
  }

  return theValarray;
}

/** Read data from a file to create an array of strikes and maturities which 
	can be used to construct a vol surface object.  
	@param filename - source of data
	@returns structure containing strikes, maturities, etc.
	*/
volsurfaceparams *
FileReader::buildVolSurfaceParams(string filename) {
	volsurfaceparams *p = new volsurfaceparams();

	CSVParser parser;
	string sLine, dummy;
	Natural numStrikes, numMaturities;

	if (!FileReader::fileexists(filename)) {
		cout << "could not find file " << filename << endl;
		return NULL;
	}

	ifstream infile(filename.c_str());

	// file format is
	// maturities,<num of maturities>
	// strikes,<num of strikes>
	// maturities/strikes,<strike>,type,<strike>,type,...
	// <date>            ,<price>,<c|p>,<price>,<c|p>,...

	getline(infile, sLine); // Get a line
    parser << sLine; // Feed the line to the parser
    parser >> dummy >> numMaturities;
	// cout << "maturities = " << numMaturities << endl;

	getline(infile, sLine);
	parser << sLine;
	parser >> dummy >> numStrikes;
	// cout << "strikes = " << numStrikes << endl;

	p->strikes.resize(numStrikes);
	p->maturities.resize(numMaturities);
	p->callputprices.resize(numMaturities);
	p->iscallputprices.resize(numMaturities);
	for (Natural i = 0; i < numMaturities; i++) {
		p->callputprices[i].resize(numStrikes);
		p->iscallputprices[i].resize(numStrikes);
	}
		
	getline(infile, sLine);
	parser << sLine;
	parser >> dummy; // skip past first column
	for (i = 0; i < numStrikes; i++) {
		parser >> p->strikes[i] >> dummy; // every other column is "type"
		// cout << "dummy: " << dummy << ", strike: " << p->strikes[i] << endl;
	}

	Natural maturityIdx = 0;
	string callorput;

	while (!infile.eof()) {
		getline(infile, sLine);
		if (sLine == "")
			continue;
		parser << sLine;
		parser >> p->maturities[maturityIdx];
		// cout << "maturity: " << p->maturities[maturityIdx].toString() << endl;
		
		for (i = 0; i < numStrikes; i++) {
			parser >> p->callputprices[maturityIdx][i];
			parser >> callorput;
			// cout << "price: " << p->callputprices[maturityIdx][i] << ", " << callorput << endl;
			p->iscallputprices[maturityIdx][i] = 
				(callorput.compare("c") == 0);
		}
		maturityIdx++;
	}

	return p;
}