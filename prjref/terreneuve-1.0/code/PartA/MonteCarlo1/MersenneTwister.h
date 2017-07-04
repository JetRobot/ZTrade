#pragma once
#ifndef MERSENNETWISTER_H
#define MERSENNETWISTER_H

#include "./RandomGenerator.h"
#include "../../Common/types.h"

#include <valarray>
using namespace std;

class MersenneTwister :	public RandomGenerator
{
public:

	/** Default constructor : if the given seed is 0, a random seed will be chosen
	based on clock() */
	MersenneTwister(LongNatural seed=0);
	MersenneTwister(const valarray<LongNatural>& seeds);
	~MersenneTwister();
	/** returns a sample with weight 1.0 containing a random number	on (0.0, 1.0) */
	inline Real getUniform() {return (Real(GetOneRandomInteger()) + 0.5)/4294967296.0;}
	//! return  a random number on [0,0xffffffff]-interval
	LongNatural GetOneRandomInteger() ;
	/** Return maximum number of random numbers */
    VeryLongNatural Max();
	LongNatural Min();
	void SetSeed(LongNatural seed);

private:
	LongInteger seed;
	valarray<LongNatural> mt;
	LongNatural mti;
};

#endif
