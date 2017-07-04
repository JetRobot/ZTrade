#pragma once
#include "date.h"

/**
@author Simon
*/

class UsDate :
	public Date
{
public:
	bool isBusinessDay();
};
