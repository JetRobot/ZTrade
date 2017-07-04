#include <iostream>
#include <fstream>
#include <string>
#include "../Interface/main.h"
#include "../common/csvparser.h"
#include "../common/filereader.h"
#include "../PartB/yieldCurve.h"

using namespace std;

/* Added some modifications to integrate with terreneuve - alokem */

/*
Copyright (c) 2004, Mayukh Bose
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

* Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.  

* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

* Neither the name of Mayukh Bose nor the names of other
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS interRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

bool mainfilereader(void) {
	bool result = true;

	result &= fr_basic();

	return result;
}

bool fr_basic(void)
{
  string sLine;
  string sCol1, sCol3, sCol4;
  double fCol2;
  int iCol5, iCol6;

  // string fullpath = string(globaldatadir) + "sample.csv";
  string fullpath = FileReader::getdatadirasstring() + "sample.csv";


  CSVParser parser;

  if (!FileReader::fileexists(fullpath)) {
	cout << "could not find file " << fullpath << endl;
	return false;
  }
    
  ifstream infile(fullpath.c_str());
  while (!infile.eof()) {
    getline(infile, sLine); // Get a line
    if (sLine == "")
      continue;

    parser << sLine; // Feed the line to the parser

    // Now extract the columns from the line
    parser >> sCol1 >> fCol2 >> sCol3;
    parser >> sCol4 >> iCol5;
    parser >> iCol6;

    cout << "Column1: " << sCol1 << endl
         << "Column2: " << fCol2 << endl
         << "Column3: " << sCol3 << endl
         << "Column4: " << sCol4 << endl
         << "Column5: " << iCol5 << endl
         << "Column6: " << iCol6 << endl
         << endl;
  }
  infile.close();

  // check one value...
  return (iCol6 == 63);
}

