#include "main.h"
using namespace std;

Natural main(Natural argc, char **argv)
{
	cout<<"**************************************"<<endl;
	cout<<"**************************************"<<endl;
	cout<<"*****                            *****"<<endl;
	cout<<"*****  Terreneuve-Devel project  *****"<<endl;
	cout<<"*****                            *****"<<endl;
	cout<<"**************************************"<<endl;
	cout<<"**************************************\n"<<endl;

	cout<<"Welcome to Terreneuve Project user interface.\n"<<endl;
	
	bool replay=true,haveDataBeenImported;
	marketData data;
	importData import;
	Natural choice;

	while(replay){
		choice =0;
		cout<<"**************************************"<<endl;
		cout<<"************  Main  Menu  ************"<<endl;
		cout<<"**************************************\n"<<endl;
		cout<<"\nWhat would you like to do? Press:"<<endl;
		cout<<"1 - To import data"<<endl;
		cout<<"2 - To create some products"<<endl;
		cout<<"3 - To create and save a portfolio"<<endl;
		cout<<"4 - To visualize the tests we ran"<<endl;
		cout<<"5 - To view the credits"<<endl;
		cout<<"6 - To quit"<<endl;
		cin>>choice;
		switch(choice){
			case 1:
				import=importData(*argv); // Launches the import Menu - will have access to yc, vs and cs to create products
				haveDataBeenImported=true;
				data=import.getData();
				break;
			case 2:
				if(haveDataBeenImported)
					productsCreationMenu(data);
				else
					cout<<"Without any data you cannot use products, at least import the default and do not use it later..."<<endl;
				break;
			case 3:
				cout<<"Not yet available - promise will be soon"<<endl;
				break;
			case 4:
				maintests(argc,argv);
				break;
			case 5:
				credits();
				break;
			default:
				replay=false;
		}
	}
	return 1;
} 