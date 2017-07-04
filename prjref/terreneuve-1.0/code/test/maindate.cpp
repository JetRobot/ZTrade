#include "../Interface/main.h"
#include "../common/date.h"
#include <iostream>

using namespace std;

bool maindate(void) {
	Date d=Date();
	d.setDateToToday();
	cout<<"Today's date is :"<<endl;
	cout<<d.toString()<<endl;
	if (d.lastDayOfMonth()!=30 || d.lastDayOfMonth()!=31) {
		return false;
	} else return true;
}