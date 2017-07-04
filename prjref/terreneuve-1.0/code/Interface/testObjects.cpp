#include <iostream>
#include <string>
#include "testObjects.h"
#include "../common/filereader.h"
using namespace std;

Natural maintests(Natural argc, char **argv)
{
	const ShortNatural TESTNAME_LEN=30;

	/* set the global datadir variable */
	if (FileReader::setdatadir(argv[0]) == false) {
		cout << "could not set data dir, some tests may fail" << endl;
	}

	cout<<"\n**************************************"<<endl;
	cout<<"******  Objects Testing Module  ******"<<endl;
	cout<<"**************************************\n"<<endl;
	
	bool testpass = true;
	bool playagain=true;
	string choice;
	Natural choiceNAt=0;

	struct tests {
		char testname[TESTNAME_LEN];
		bool (*testfunction)();
	};

 	while (playagain) {
		choiceNAt=0;
		playagain=false;
		testpass=true;

		tests testarray[] = {
			{"filereader", mainfilereader},
			{"interpolator", maininterpolator},
			{"matrix", mainmatrix},
			{"date", maindate},
			{"yieldcurve", mainyieldcurve},
			{"volsurface", mainvolsurface},
			{"creditcurve", maincreditcurve},
			{"blackscholes", mainoption},
			{"optionstrategy", mainoptionstrategy},
			{"montecarlo", mainmontecarlo},
			{"asset", mainasset},
			{"rainbowoptions", mainrainbowoptions},
			{"varianceswaps",mainvarianceswap},
			{"bond", mainbond},
			{"binomialtree", mainbinomialtree},
			{"convertiblebond", mainconvertiblebond},
			{"IRVanillaSwap", mainIRVanillaSwap}
		};
		Natural maxtest = sizeof(testarray)/sizeof(tests);

		for (ShortNatural i = 0; i < maxtest; i++) {
			cout << "  Press " << i + 1<< " for " << testarray[i].testname << endl;
		}
		cout<<"* Press "<<maxtest+1<<" for all tests (might be long)"<<endl;
		cout <<"* Press something else than a number between 1 and "<<maxtest+1<<" to end program"<<endl;

		cin >> choice;
		choiceNAt=atoi(choice.c_str());
		if(choiceNAt<=maxtest && choiceNAt>0)	{
			testpass &= testarray[choiceNAt-1].testfunction();
			if (testpass) {
				cout << "\n\n***** all tests passed" << endl;
			} else {
				cout << "\n\n***** some tests failed" << endl;
			}
			Natural test=0;
			cout << "\nType 1 + enter key to test again or any other key to end the test module" << endl;
			cin >> test;
			if (test==1) playagain=true;
		}
		else {
			if(choiceNAt==maxtest+1){
				for (ShortNatural i = 0; i < maxtest; i++) {
					testpass &= testarray[i].testfunction();
				}
				if (testpass) {
					cout << "\n\n***** all tests passed" << endl;
				} else {
					cout << "\n\n***** some tests failed" << endl;
				}
				Natural test=0;
				cout << "\nType 1 + enter key to test again or any other key to end the test module" << endl;
				cin >> test;
				if (test==1) playagain=true;
			}
		}
	}

	if (testpass) {
		return 1;
	} else {
		return 0;
	}
} 